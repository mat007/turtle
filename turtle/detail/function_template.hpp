// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "function_impl_template.hpp"

namespace mock
{
namespace detail
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
        typedef function_impl<
            R ( BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, T) ) > impl_type;
        typedef BOOST_DEDUCED_TYPENAME
            impl_type::expectation_type expectation_type;
        typedef BOOST_DEDUCED_TYPENAME impl_type::error_type error_type;

    public:
        function()
            : impl_( new impl_type() )
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

        expectation_type& expect( const char* file, int line )
        {
            error_type::pass( file, line );
            return impl_->expect( file, line );
        }
        expectation_type& expect()
        {
            return impl_->expect();
        }

        R operator()( BOOST_PP_ENUM_BINARY_PARAMS(MOCK_NUM_ARGS, T, t) ) const
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
            if( ! impl_->context_ )
                c.add( *impl_ );
            c.add( impl_.get(), *impl_, instance,
                boost::optional< type_name >(), "" );
            impl_->context_ = &c;
            return *this;
        }

        void configure( context& c, const void* p,
            boost::unit_test::const_string instance,
            boost::optional< type_name > type,
            boost::unit_test::const_string name ) const
        {
            if( ! impl_->context_ )
                c.add( *impl_ );
            c.add( p, *impl_, instance, type, name );
            impl_->context_ = &c;
        }

    private:
        boost::shared_ptr< impl_type > impl_;
    };
}
} // mock
