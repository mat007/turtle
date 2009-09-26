//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_EXPECTATION_HPP_INCLUDED
#define MOCK_EXPECTATION_HPP_INCLUDED

#include "config.hpp"
#include "error.hpp"
#include "verifiable.hpp"
#include "matcher.hpp"
#include "node.hpp"
#include "root.hpp"
#include "format.hpp"
#include "invocation.hpp"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <exception>
#include <ostream>
#include <list>

namespace mock
{
    template< typename Signature,
              typename ErrorPolicy = boost_test_error_policy<
                  BOOST_DEDUCED_TYPENAME boost::function<
                      Signature >::result_type > > // $$$$ MAT : concept check Signature is actually a signature
    class expectation
    {
    public:
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature >::result_type result_type;

    private:
        typedef detail::matcher< result_type,
                                 Signature,
                                 ErrorPolicy,
                                 boost::function< Signature >::arity >
                matcher_type;

    public:
        struct expectation_tag
        {};
        expectation_tag operator_exp;

        expectation( const std::string& name = "?" )
            : impl_( new expectation_impl( root, name ) )
        {}

        void set_name( const std::string& name )
        {
            impl_->set_name( name );
        }
        void set_parent( node& parent )
        {
            impl_->set_parent( parent );
        }

        bool verify() const
        {
            return impl_->verify();
        }
        void reset()
        {
            impl_->reset();
        }

        matcher_type& expect( const std::string& file, int line )
        {
            return impl_->expect( file, line );
        }
        matcher_type& expect()
        {
            return impl_->expect();
        }

        result_type operator()() const
        {
            return (*impl_)();
        }

#define MOCK_EXPECTATION_OPERATOR(z, n, d) \
        template< BOOST_PP_ENUM_PARAMS(n, typename A) > \
        result_type operator()( BOOST_PP_ENUM_BINARY_PARAMS(n, const A, & a) ) const \
        { \
            return (*impl_)( BOOST_PP_ENUM_PARAMS(n, a) ); \
        }
        BOOST_PP_REPEAT_FROM_TO(1, MOCK_MAX_ARGS, MOCK_EXPECTATION_OPERATOR, BOOST_PP_EMPTY)
#undef MOCK_EXPECTATION_OPERATOR

        friend std::ostream& operator<<( std::ostream& s, const expectation& e )
        {
            return s << *e.impl_;
        }

    private:
        class expectation_impl : private verifiable
        {
        public:
            expectation_impl( node& parent, const std::string& name )
                : name_( name )
                , parent_( &parent )
                , valid_( true )
            {
                parent_->add( *this );
            }
            virtual ~expectation_impl()
            {
                parent_->remove( *this );
                if( ! std::uncaught_exception() )
                    for( matchers_cit it = matchers_.begin();
                        it != matchers_.end(); ++it )
                        if( valid_ && ! it->verify() )
                            ErrorPolicy::untriggered_expectation(
                                context(), it->file(), it->line() );
            }

            void set_name( const std::string& name )
            {
                name_ = name;
            }
            void set_parent( node& parent )
            {
                parent_->remove( *this );
                parent.add( *this );
                parent_ = &parent;
            }

            virtual bool verify() const
            {
                for( matchers_cit it = matchers_.begin();
                    it != matchers_.end(); ++it )
                    if( !it->verify() )
                    {
                        valid_ = false;
                        ErrorPolicy::verification_failed( context(),
                            it->file(), it->line() );
                    }
                return valid_;
            }
            virtual void reset()
            {
                valid_ = true;
                matchers_.clear();
            }

            matcher_type& expect( const std::string& file, int line )
            {
                matchers_.push_back( matcher_type() );
                matchers_.back().set_location( file, line );
                valid_ = true;
                return matchers_.back();
            }
            matcher_type& expect()
            {
                matchers_.push_back( matcher_type() );
                valid_ = true;
                return matchers_.back();
            }

            result_type operator()() const
            {
                for( matchers_cit it = matchers_.begin();
                    it != matchers_.end(); ++it )
                    if( it->is_valid() )
                    {
                        if( !it->invoke() )
                        {
                            valid_ = false;
                            ErrorPolicy::sequence_failed( context( "" ),
                                it->file(), it->line() );
                        }
                        return it->functor()();
                    }
                valid_ = false;
                return ErrorPolicy::no_match( context( "" ) );
            }

#define MOCK_EXPECTATION_PARAMETER(z, n, d) BOOST_PP_COMMA_IF(n) const_cast< A##n & >( a##n )
#define MOCK_EXPECTATION_DETAIL(z, n, d) + ", " + format( a##n )
#define MOCK_EXPECTATION_PARAMETERS(n) \
    format( a0 ) BOOST_PP_REPEAT_FROM_TO(1, n, MOCK_EXPECTATION_DETAIL, BOOST_PP_EMPTY)
#define MOCK_EXPECTATION_OPERATOR(z, n, d) \
            template< BOOST_PP_ENUM_PARAMS(n, typename A) > \
            result_type operator()( BOOST_PP_ENUM_BINARY_PARAMS(n, const A, & a) ) const \
            { \
                for( matchers_cit it = matchers_.begin(); it != matchers_.end(); ++it ) \
                    if( it->is_valid( BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_EXPECTATION_PARAMETER, BOOST_PP_EMPTY) ) ) \
                    { \
                        if( !it->invoke() ) \
                        { \
                            valid_ = false; \
                            ErrorPolicy::sequence_failed( context( MOCK_EXPECTATION_PARAMETERS(n) ), it->file(), it->line() ); \
                        } \
                        return it->functor()( BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_EXPECTATION_PARAMETER, BOOST_PP_EMPTY) ); \
                    } \
                valid_ = false; \
                return ErrorPolicy::no_match( context( MOCK_EXPECTATION_PARAMETERS(n) ) ); \
            }
            BOOST_PP_REPEAT_FROM_TO(1, MOCK_MAX_ARGS, MOCK_EXPECTATION_OPERATOR, BOOST_PP_EMPTY)
#undef MOCK_EXPECTATION_PARAMETER
#undef MOCK_EXPECTATION_PARAMETERS
#undef MOCK_EXPECTATION_DETAIL
#undef MOCK_EXPECTATION_OPERATOR

            friend std::ostream& operator<<( std::ostream& s, const expectation_impl& e )
            {
                return s << e.context();
            }

        private:
            typedef std::list< matcher_type > matchers_type;
            typedef BOOST_DEDUCED_TYPENAME
                matchers_type::const_iterator matchers_cit;

            void serialize( std::ostream& s ) const
            {
                for( matchers_cit it = matchers_.begin();
                    it != matchers_.end(); ++it )
                    s << std::endl << *it;
            }

            std::string context() const
            {
                std::stringstream s;
                s << name_;
                serialize( s );
                return s.str();
            }
            std::string context( const std::string& parameters ) const
            {
                std::stringstream s;
                s << name_;
                if( parameters.empty() )
                    s << "()";
                else
                    s << "( " << parameters << " )";
                serialize( s );
                return s.str();
            }

            std::string name_;
            node* parent_;
            mutable bool valid_;
            matchers_type matchers_;
        };

    private:
        boost::shared_ptr< expectation_impl > impl_;
    };
}

#endif // #ifndef MOCK_EXPECTATION_HPP_INCLUDED
