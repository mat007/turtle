// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "function_impl_template.hpp"

#define MOCK_FUNCTION_CALL(z, n, d ) \
    BOOST_PP_COMMA_IF(n) typename \
        boost::call_traits< T##n >::param_type

#define MOCK_FUNCTION_PARAM(z, n, d) \
    MOCK_FUNCTION_CALL(z, n, d) t##n

namespace mock
{
namespace detail
{
    template< typename Signature > class function;

    template< typename R
        BOOST_PP_ENUM_TRAILING_PARAMS(MOCK_NUM_ARGS, typename T) >
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
        typedef function_impl<
            R ( BOOST_PP_REPEAT(MOCK_NUM_ARGS, MOCK_FUNCTION_CALL, _) )
        > impl_type;
        typedef typename impl_type::wrapper_type expectation_type;
        typedef typename impl_type::error_type error_type;

    public:
        function()
            : impl_( boost::make_shared< impl_type >() )
        {}

        bool verify() const
        {
            return impl_->verify();
        }
        bool verify( const char* file, int line ) const
        {
            error_type::pass( file, line );
            return impl_->verify();
        }
        void reset()
        {
            impl_->reset();
        }
        void reset( const char* file, int line )
        {
            error_type::pass( file, line );
            impl_->reset();
        }

        expectation_type expect( const char* file, int line )
        {
            error_type::pass( file, line );
            return impl_->expect( file, line );
        }
        expectation_type expect()
        {
            return impl_->expect();
        }

        R operator()(
            BOOST_PP_REPEAT(MOCK_NUM_ARGS, MOCK_FUNCTION_PARAM, _) ) const
        {
            return (*impl_)( BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, t) );
        }

        friend std::ostream& operator<<( std::ostream& s, const function& f )
        {
            return s << *f.impl_;
        }

        function& operator()( context& c,
            boost::unit_test::const_string instance )
        {
            impl_->add( c, impl_.get(), instance, boost::none, "" );
            return *this;
        }

        void configure( context& c, const void* p,
            boost::unit_test::const_string instance,
            boost::optional< type_name > type,
            boost::unit_test::const_string name ) const
        {
            impl_->add( c, p, instance, type, name );
        }

    private:
        boost::shared_ptr< impl_type > impl_;
    };
}
} // mock

#undef MOCK_FUNCTION_CALL
#undef MOCK_FUNCTION_PARAM
