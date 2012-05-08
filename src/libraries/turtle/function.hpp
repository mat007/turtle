//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

#ifndef MOCK_FUNCTION_HPP_INCLUDED
#define MOCK_FUNCTION_HPP_INCLUDED

#include "config.hpp"
#include "error.hpp"
#include "context.hpp"
#include "expectation.hpp"
#include "verifiable.hpp"
#include "log.hpp"
#include "args.hpp"
#include "type_name.hpp"
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>
#include <boost/test/utils/lazy_ostream.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <ostream>
#include <list>

namespace mock
{
    template< typename Signature >
    class function
    {
    public:
        typedef BOOST_DEDUCED_TYPENAME
            boost::function_types::result_type< Signature >::type result_type;

        template< typename Args >
        struct sig
        {
            typedef result_type type;
        };

    private:
        typedef BOOST_DEDUCED_TYPENAME
            boost::function_types::function_arity< Signature > arity;
        typedef BOOST_DEDUCED_TYPENAME
            detail::expectation< Signature, arity::value > expectation_type;

    public:
        function()
            : impl_( new function_impl() )
        {}

        bool verify() const
        {
            return impl_->verify();
        }
        bool verify( const char* file, int line ) const
        {
            function_impl::error_type::checkpoint( file, line );
            return impl_->verify();
        }
        void reset()
        {
            impl_->reset();
        }
        void reset( const char* file, int line )
        {
            function_impl::error_type::checkpoint( file, line );
            return impl_->reset();
        }

        expectation_type& expect( const char* file, int line )
        {
            function_impl::error_type::checkpoint( file, line );
            return impl_->expect();
        }
        expectation_type& expect()
        {
            return impl_->expect();
        }

        void test() const
        {
            impl_->test();
        }

        result_type operator()() const
        {
            return (*impl_)();
        }

#define MOCK_FUNCTION_OPERATOR(z, n, d) \
        MOCK_DECL(operator(), n, Signature, const, BOOST_DEDUCED_TYPENAME) \
        { \
            return (*impl_)( BOOST_PP_ENUM_PARAMS(n, p) ); \
        }

        BOOST_PP_REPEAT_FROM_TO(
            1,
            BOOST_PP_INC(MOCK_MAX_ARGS),
            MOCK_FUNCTION_OPERATOR,
            BOOST_PP_EMPTY)

#undef MOCK_FUNCTION_OPERATOR

        friend std::ostream& operator<<( std::ostream& s, const function& e )
        {
            return s << *e.impl_;
        }

        function& operator()( detail::context& c,
            boost::unit_test::const_string instance )
        {
            if( ! impl_->context_ )
                c.add( *impl_ );
            c.add( impl_.get(), *impl_, instance,
                boost::optional< detail::type_name >(), "" );
            impl_->context_ = &c;
            return *this;
        }

        void configure( detail::context& c, const void* p,
            boost::unit_test::const_string instance,
            const boost::optional< detail::type_name >& type,
            boost::unit_test::const_string name ) const
        {
            if( ! impl_->context_ )
                c.add( *impl_ );
            c.add( p, *impl_, instance, type, name );
            impl_->context_ = &c;
        }

    private:
        class function_impl : public detail::verifiable,
            public boost::enable_shared_from_this< function_impl >
        {
        public:
            typedef MOCK_ERROR_POLICY< result_type > error_type;

        public:
            function_impl()
                : context_( 0 )
                , valid_( true )
            {}
            virtual ~function_impl()
            {
                if( valid_ && ! std::uncaught_exception() )
                    for( expectations_cit it = expectations_.begin();
                        it != expectations_.end(); ++it )
                    {
                        if( ! it->verify() )
                            error_type::untriggered_expectation(
                                boost::unit_test::lazy_ostream::instance()
                                    << lazy_context( this )
                                    << lazy_expectations( this ),
                                it->file(), it->line() );
                        else if( ! it->invoked() )
                            error_type::expected_call(
                                boost::unit_test::lazy_ostream::instance()
                                    << lazy_context( this )
                                    << lazy_expectations( this ),
                                it->file(), it->line() );
                    }
                if( context_ )
                    context_->remove( *this );
            }

            virtual bool verify() const
            {
                for( expectations_cit it = expectations_.begin();
                    it != expectations_.end(); ++it )
                    if( !it->verify() )
                    {
                        valid_ = false;
                        error_type::verification_failed(
                            boost::unit_test::lazy_ostream::instance()
                                    << lazy_context( this )
                                    << lazy_expectations( this ),
                            it->file(), it->line() );
                    }
                return valid_;
            }

            virtual void reset()
            {
                valid_ = true;
                boost::shared_ptr< function_impl > guard =
                    this->shared_from_this();
                expectations_.clear();
            }

            expectation_type& expect( const char* file, int line )
            {
                expectation_type& e = expect();
                e.set_location( file, line );
                return e;
            }
            expectation_type& expect()
            {
                expectations_.push_back( expectation_type() );
                valid_ = true;
                return expectations_.back();
            }

#define MOCK_FUNCTION_FORMAT(z, n, N) \
    << " " << mock::format( p##n ) \
    << BOOST_PP_IF(BOOST_PP_EQUAL(N,n), " ", ",")
#define MOCK_FUNCTION_CONTEXT(n) \
    boost::unit_test::lazy_ostream::instance() \
        << lazy_context( this ) \
        << "(" BOOST_PP_REPEAT(n, MOCK_FUNCTION_FORMAT, BOOST_PP_DEC(n)) \
        << ")" \
        << lazy_expectations( this )
#define MOCK_FUNCTION_INVOKE(z, n, A) \
    { \
        valid_ = false; \
        for( expectations_cit it = expectations_.begin(); \
            it != expectations_.end(); ++it ) \
            if( it->is_valid( BOOST_PP_ENUM_PARAMS(n, p) ) ) \
            { \
                if( ! it->invoke() ) \
                { \
                    error_type::sequence_failed( \
                        MOCK_FUNCTION_CONTEXT(n), it->file(), it->line() ); \
                    return A; \
                } \
                if( ! it->functor() ) \
                { \
                    error_type::missing_action( \
                        MOCK_FUNCTION_CONTEXT(n), it->file(), it->line() ); \
                    return A; \
                } \
                valid_ = true; \
                error_type::expected_call( \
                    MOCK_FUNCTION_CONTEXT(n), it->file(), it->line() ); \
                return it->functor()( BOOST_PP_ENUM_PARAMS(n, p) ); \
            } \
        error_type::unexpected_call( MOCK_FUNCTION_CONTEXT(n) ); \
        return A; \
    }
#define MOCK_FUNCTION_OPERATOR(z, n, P) \
    MOCK_DECL(operator(), n, Signature, const, BOOST_DEDUCED_TYPENAME) \
    MOCK_FUNCTION_INVOKE(z, n, P)

            BOOST_PP_REPEAT(
                BOOST_PP_INC(MOCK_MAX_ARGS),
                MOCK_FUNCTION_OPERATOR,
                error_type::abort())

            void test() const
            MOCK_FUNCTION_INVOKE(, 0,)

#undef MOCK_FUNCTION_FORMAT
#undef MOCK_FUNCTION_OPERATOR
#undef MOCK_FUNCTION_INVOKE
#undef MOCK_FUNCTION_CONTEXT

            friend std::ostream& operator<<(
                std::ostream& s, const function_impl& e )
            {
                return s << lazy_context( &e ) << lazy_expectations( &e );
            }

            struct lazy_context
            {
                lazy_context( const function_impl* impl )
                    : impl_( impl )
                {}
                friend std::ostream& operator<<(
                    std::ostream& s, const lazy_context& c )
                {
                    if( c.impl_->context_ )
                        c.impl_->context_->serialize( s, *c.impl_ );
                    else
                        s << "?";
                    return s;
                }
                const function_impl* impl_;
            };

            struct lazy_expectations
            {
                lazy_expectations( const function_impl* impl )
                    : impl_( impl )
                {}
                friend std::ostream& operator<<(
                    std::ostream& s, const lazy_expectations& e )
                {
                    for( expectations_cit it = e.impl_->expectations_.begin();
                        it != e.impl_->expectations_.end(); ++it )
                        s << std::endl << *it;
                    return s;
                }
                const function_impl* impl_;
            };

            typedef std::list< expectation_type > expectations_type;
            typedef BOOST_DEDUCED_TYPENAME
                expectations_type::const_iterator expectations_cit;

            expectations_type expectations_;
            detail::context* context_;
            mutable bool valid_;
        };

        boost::shared_ptr< function_impl > impl_;
    };

    template< typename Signature >
    bool verify( const function< Signature >& f )
    {
        return f.verify();
    }
    template< typename Signature >
    void reset( function< Signature >& f )
    {
        f.reset();
    }
}

#endif // MOCK_FUNCTION_HPP_INCLUDED
