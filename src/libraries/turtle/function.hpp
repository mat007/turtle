//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_FUNCTION_HPP_INCLUDED
#define MOCK_FUNCTION_HPP_INCLUDED

#include "config.hpp"
#include "error.hpp"
#include "expectation.hpp"
#include "root.hpp"
#include "format.hpp"
#include "args.hpp"
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <ostream>
#include <list>

namespace mock
{
    template< typename Signature,
              typename ErrorPolicy = MOCK_ERROR_POLICY<
                  BOOST_DEDUCED_TYPENAME
                      boost::function_types::result_type< Signature >::type > >
    class function
    {
    public:
        typedef BOOST_DEDUCED_TYPENAME
            boost::function_types::result_type< Signature >::type result_type;
        typedef Signature signature_type;
        typedef BOOST_DEDUCED_TYPENAME
            boost::function_types::function_arity< Signature > arity;

        template< typename Args >
        struct sig
        {
            typedef result_type type;
        };

    private:
        typedef detail::expectation< result_type, Signature, arity::value >
            expectation_type;

    public:
        struct function_tag
        {};
        function_tag exp_;

        function()
            : impl_( new function_impl() )
        {}

        void tag( const std::string& name )
        {
            impl_->tag( name );
        }
        const std::string& tag() const
        {
            return impl_->tag();
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

        expectation_type& expect( const std::string& file, int line )
        {
            return impl_->expect( file, line );
        }
        expectation_type& expect()
        {
            return impl_->expect();
        }

        void test() const
        {
            return impl_->test();
        }

        result_type operator()() const
        {
            return (*impl_)();
        }

#define MOCK_EXPECTATION_OPERATOR(z, n, d) \
        MOCK_DECL(operator(), n, Signature, const, BOOST_DEDUCED_TYPENAME) \
        { \
            return (*impl_)( BOOST_PP_ENUM_PARAMS(n, p) ); \
        }
        BOOST_PP_REPEAT_FROM_TO(1, MOCK_NUM_ARGS, MOCK_EXPECTATION_OPERATOR, BOOST_PP_EMPTY)
#undef MOCK_EXPECTATION_OPERATOR

        friend std::ostream& operator<<( std::ostream& s, const function& e )
        {
            return s << *e.impl_;
        }

    private:
        class function_impl : private verifiable
        {
        public:
            function_impl()
                : name_( "?" )
                , parent_( 0 )
                , valid_( true )
            {}
            virtual ~function_impl()
            {
                if( parent_ )
                    parent_->remove( *this );
                if( ! std::uncaught_exception() )
                    for( expectations_cit it = expectations_.begin();
                        it != expectations_.end(); ++it )
                        if( valid_ && ! it->verify() )
                            ErrorPolicy::untriggered_expectation(
                                context(), it->file(), it->line() );
            }

            void tag( const std::string& name )
            {
                name_ = name;
            }
            const std::string& tag() const
            {
                return name_;
            }
            void set_parent( node& parent )
            {
                if( parent_ )
                    parent_->remove( *this );
                parent.add( *this );
                parent_ = &parent;
            }

            virtual bool verify() const
            {
                for( expectations_cit it = expectations_.begin();
                    it != expectations_.end(); ++it )
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
                expectations_.clear();
            }
            virtual void untie()
            {
                parent_ = 0;
            }

            expectation_type& expect( const std::string& file, int line )
            {
                expectation_type& e = expect();
                e.set_location( file, line );
                return e;
            }
            expectation_type& expect()
            {
                if( ! parent_ )
                    set_parent( root );
                expectations_.push_back( expectation_type() );
                valid_ = true;
                return expectations_.back();
            }

            struct no_throw_abort
            {
                static void abort() {}
            };
            void test() const
            {
                invoke< no_throw_abort >();
            }

            result_type operator()() const
            {
                return invoke< ErrorPolicy >();
            }

#define MOCK_EXPECTATION_FORMAT(z, n, d) \
    BOOST_PP_IF(n, + ", " +,) mock::format( p##n )
#define MOCK_EXPECTATION_CONTEXT(n) \
    context( BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_EXPECTATION_FORMAT, BOOST_PP_EMPTY) )
#define MOCK_EXPECTATION_OPERATOR(z, n, d) \
    MOCK_DECL(operator(), n, Signature, const, BOOST_DEDUCED_TYPENAME) \
    { \
        for( expectations_cit it = expectations_.begin(); it != expectations_.end(); ++it ) \
            if( it->is_valid( BOOST_PP_ENUM_PARAMS(n, p) ) ) \
            { \
                if( ! it->invoke() ) \
                { \
                    valid_ = false; \
                    ErrorPolicy::sequence_failed( MOCK_EXPECTATION_CONTEXT(n), it->file(), it->line() ); \
                    return ErrorPolicy::abort(); \
                } \
                if( ! it->functor() ) \
                { \
                    ErrorPolicy::missing_action( MOCK_EXPECTATION_CONTEXT(n), it->file(), it->line() ); \
                    return ErrorPolicy::abort(); \
                } \
                return it->functor()( BOOST_PP_ENUM_PARAMS(n, p) ); \
            } \
        valid_ = false; \
        ErrorPolicy::unexpected_call( MOCK_EXPECTATION_CONTEXT(n) ); \
        return ErrorPolicy::abort(); \
    }
    BOOST_PP_REPEAT_FROM_TO(1, MOCK_NUM_ARGS, MOCK_EXPECTATION_OPERATOR, BOOST_PP_EMPTY)
#undef MOCK_EXPECTATION_CONTEXT
#undef MOCK_EXPECTATION_FORMAT
#undef MOCK_EXPECTATION_OPERATOR

            friend std::ostream& operator<<( std::ostream& s, const function_impl& e )
            {
                return s << e.context();
            }
        private:

            template< typename T >
            result_type invoke() const
            {
                for( expectations_cit it = expectations_.begin();
                    it != expectations_.end(); ++it )
                    if( it->is_valid() )
                    {
                        if( ! it->invoke() )
                        {
                            valid_ = false;
                            ErrorPolicy::sequence_failed( context( "" ),
                                it->file(), it->line() );
                            return T::abort();
                        }
                        if( ! it->functor() )
                        {
                            ErrorPolicy::missing_action( context( "" ),
                                it->file(), it->line() );
                            return T::abort();
                        }
                        return it->functor()();
                    }
                valid_ = false;
                ErrorPolicy::unexpected_call( context( "" ) );
                return T::abort();
            }

        private:
            typedef std::list< expectation_type > expectations_type;
            typedef BOOST_DEDUCED_TYPENAME
                expectations_type::const_iterator expectations_cit;

            std::string context() const
            {
                std::stringstream s;
                serialize( s, "" );
                return s.str();
            }
            std::string context( const std::string& parameters ) const
            {
                std::stringstream s;
                if( parameters.empty() )
                    serialize( s, "()" );
                else
                    serialize( s, "( " + parameters + " )" );
                return s.str();
            }
            void serialize( std::ostream& s,
                const std::string& parameters ) const
            {
                if( parent_ )
                    s << parent_->tag();
                s << name_ << parameters;
                for( expectations_cit it = expectations_.begin();
                    it != expectations_.end(); ++it )
                    s << std::endl << *it;
            }

            std::string name_;
            node* parent_;
            mutable bool valid_;
            expectations_type expectations_;
        };

    private:
        boost::shared_ptr< function_impl > impl_;
    };
}

#endif // #ifndef MOCK_FUNCTION_HPP_INCLUDED
