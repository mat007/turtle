// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "expectation_template.hpp"

namespace mock
{
    template< typename Signature > class function;

    template< typename R BOOST_PP_COMMA_IF(MOCK_NUM_ARGS)
        BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, typename T) >
    class function< R ( BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, T) ) >
    {
    public:
        typedef R result_type;

        template< typename Args >
        struct sig
        {
            typedef R type;
        };

    private:
        typedef detail::expectation<
            R ( BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, T) )
        > expectation_type;

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

        R operator()( BOOST_PP_ENUM_BINARY_PARAMS(MOCK_NUM_ARGS, T, t) ) const
        {
            return (*impl_)( BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, t) );
        }

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
            boost::optional< detail::type_name > type,
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

#define MOCK_FORMAT(z, n, N) \
    << " " << mock::format( t##n ) \
    << BOOST_PP_IF(BOOST_PP_EQUAL(N,n), " ", ",")
#define MOCK_CONTEXT \
    boost::unit_test::lazy_ostream::instance() \
        << lazy_context( this ) \
        << "(" BOOST_PP_REPEAT(MOCK_NUM_ARGS, MOCK_FORMAT, \
            BOOST_PP_DEC(MOCK_NUM_ARGS)) \
        << ")" \
        << lazy_expectations( this )

            result_type operator()(
                BOOST_PP_ENUM_BINARY_PARAMS(MOCK_NUM_ARGS, T, t) ) const
            {
                valid_ = false;
                for( expectations_cit it = expectations_.begin();
                    it != expectations_.end(); ++it )
                    if( it->is_valid( \
                        BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, t) ) )
                    {
                        if( ! it->invoke() )
                        {
                            error_type::sequence_failed(
                                MOCK_CONTEXT, it->file(), it->line() );
                            return error_type::abort();
                        }
                        if( ! it->functor() )
                        {
                            error_type::missing_action(
                                MOCK_CONTEXT, it->file(), it->line() );
                            return error_type::abort();
                        }
                        valid_ = true;
                        error_type::expected_call(
                            MOCK_CONTEXT, it->file(), it->line() );
                        return it->functor()( \
                            BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, t) );
                    }
                error_type::unexpected_call( MOCK_CONTEXT );
                return error_type::abort();
            }

#undef MOCK_FORMAT
#undef MOCK_OPERATOR
#undef MOCK_CONTEXT

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
} // mock
