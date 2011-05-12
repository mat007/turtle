//
//  Copyright Mathieu Champlon 2009
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/mock.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/bind.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    struct s
    {
        void m1();
        float m2( int );
    };

    BOOST_MPL_ASSERT((
        boost::is_same<
            void(),
            mock::detail::signature< BOOST_TYPEOF( &s::m1 ) >::type
        > ));
    BOOST_MPL_ASSERT((
        boost::is_same<
            float( int ),
            mock::detail::signature< BOOST_TYPEOF( &s::m2 ) >::type
         > ));
}

namespace
{
    template< typename T >
    void my_function( T& t )
    {
        t.my_method( "some parameter" );
    }
    MOCK_CLASS( mock_class )
    {
        MOCK_METHOD_EXT( my_method, 1, void( const std::string& ), my_method )
    };
}

BOOST_AUTO_TEST_CASE( mock_object_for_static_polymorphism )
{
    const mock_class mock;
    MOCK_EXPECT( mock, my_method ).once().with( "some parameter" );
    my_function( mock );
}

namespace
{
    MOCK_CLASS( mock_class_with_operator )
    {
        MOCK_CONST_METHOD_EXT( operator+=, 1, mock_class_with_operator&( int ), addition )
    };
}

BOOST_AUTO_TEST_CASE( mock_addition_operator )
{
    mock_class_with_operator mock;
    MOCK_EXPECT( mock, addition ).once().returns( boost::ref( mock ) );
    mock += 1;
}

namespace
{
    MOCK_CLASS( mock_class_with_conversion_operator )
    {
        MOCK_CONVERSION_OPERATOR( int, conversion )
    };
}

BOOST_AUTO_TEST_CASE( mock_conversion_operator )
{
    mock_class_with_conversion_operator mock;
    MOCK_EXPECT( mock, conversion ).once().returns( 42 );
    int i = mock;
    BOOST_CHECK_EQUAL( 42, i );
}

namespace
{
    MOCK_CLASS( mock_class_with_const_conversion_operator )
    {
        MOCK_CONST_CONVERSION_OPERATOR( int, conversion )
    };
}

BOOST_AUTO_TEST_CASE( mock_const_conversion_operator )
{
    mock_class_with_const_conversion_operator mock;
    MOCK_EXPECT( mock, conversion ).once().returns( 42 );
    int i = mock;
    BOOST_CHECK_EQUAL( 42, i );
}

namespace
{
    MOCK_CLASS( mock_class_with_non_const_conversion_operator )
    {
        MOCK_CONST_CONVERSION_OPERATOR( int, conversion )
    };
}

BOOST_AUTO_TEST_CASE( mock_non_const_conversion_operator )
{
    mock_class_with_non_const_conversion_operator mock;
    MOCK_EXPECT( mock, conversion ).once().returns( 42 );
    int i = mock;
    BOOST_CHECK_EQUAL( 42, i );
}

namespace
{
    MOCK_CLASS( my_mock )
    {
        MOCK_CONST_METHOD_EXT( my_method, 1, void( int ), my_method )
        MOCK_CONST_METHOD_EXT( my_method_2, 1, void( int ), my_method_2 )
    };
}

BOOST_AUTO_TEST_CASE( MOCK_CONST_METHOD_EXT_macro_defines_a_bindable_method )
{
    my_mock m;
    boost::bind( &my_mock::my_method, &m, 42 );
}

BOOST_AUTO_TEST_CASE( MOCK_VERIFY_macro )
{
    my_mock m;
    MOCK_VERIFY( m, my_method );
}

BOOST_AUTO_TEST_CASE( MOCK_RESET_macro )
{
    my_mock m;
    MOCK_RESET( m, my_method );
}

BOOST_AUTO_TEST_CASE( MOCK_EXPECT_macro )
{
    my_mock m;
    MOCK_EXPECT( m, my_method ).once().with( 42 );
    m.my_method( 42 );
}

namespace
{
    template< typename T >
    std::string to_string( const T& t )
    {
        std::stringstream s;
        s << t;
        return s.str();
    }
}

#define MOCK_ANONYMOUS_MOCKER_EXT(o, t) \
    MOCK_ANONYMOUS_MOCKER( o, t, t )

BOOST_AUTO_TEST_CASE( mock_object_is_named )
{
    my_mock m;
    BOOST_CHECK_EQUAL( "? my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "? my_mock::my_method_2", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method_2", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_object_auto_pointer_is_named )
{
    std::auto_ptr< my_mock > m( new my_mock );
    BOOST_CHECK_EQUAL( "? my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_object_const_auto_pointer_is_named )
{
    const std::auto_ptr< my_mock > m( new my_mock );
    BOOST_CHECK_EQUAL( "? my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_object_shared_pointer_is_named )
{
    boost::shared_ptr< my_mock > m( new my_mock );
    BOOST_CHECK_EQUAL( "? my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_object_const_shared_pointer_is_named )
{
    const boost::shared_ptr< my_mock > m( new my_mock );
    BOOST_CHECK_EQUAL( "? my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
}

namespace
{
    struct my_custom_mock
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_method )
        MOCK_METHOD_EXT( my_method_2, 0, void(), my_method_2 )
    };
}

BOOST_AUTO_TEST_CASE( custom_mock_object_without_macros_and_without_inheriting_from_object_is_named )
{
    my_custom_mock m;
    BOOST_CHECK_EQUAL( "? my_custom_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "? my_custom_mock::my_method_2", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m my_custom_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "? my_custom_mock::my_method_2", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m my_custom_mock::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_custom_mock::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
}

namespace
{
    struct my_custom_mock_object : mock::object
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_method )
        MOCK_METHOD_EXT( my_method_2, 0, void(), my_method_2 )
    };
}

BOOST_AUTO_TEST_CASE( custom_mock_object_without_macros_is_named )
{
    my_custom_mock_object m;
    BOOST_CHECK_EQUAL( "? my_custom_mock_object::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "? my_custom_mock_object::my_method_2", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m my_custom_mock_object::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_custom_mock_object::my_method_2", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m my_custom_mock_object::my_method", to_string( MOCK_ANONYMOUS_MOCKER_EXT( m, my_method ) ) );
    BOOST_CHECK_EQUAL( "m my_custom_mock_object::my_method", to_string( MOCK_MOCKER( m, my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_functor_is_named )
{
    MOCK_FUNCTOR( void() ) f;
    BOOST_CHECK_EQUAL( "f", to_string( MOCK_MOCKER( f, _ ) ) );
}

BOOST_MPL_ASSERT(( boost::is_same< float, mock::detail::arg< void( float ), 1, 1 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< float, mock::detail::arg< void( float, int ), 1, 2 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< int, mock::detail::arg< void( float, int ), 2, 2 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float ), 1, 2 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float ), 2, 2 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float, int ), 1, 1 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float, int ), 1, 3 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float, int ), 2, 3 >::type > ));

BOOST_AUTO_TEST_CASE( call_selects_proper_form )
{
    {
        mock::function< void() > e;
        e.expect().once();
        mock::detail::call( e );
    }
    {
        mock::function< void( int ) > e;
        e.expect().once();
        mock::detail::call( e, 3 );
    }
    {
        mock::function< int() > e;
        e.expect().once().returns( 0 );
        mock::detail::call( e );
    }
    {
        mock::function< int( int ) > e;
        e.expect().once().returns( 0 );
        mock::detail::call( e, 3 );
    }
}

namespace
{
    struct mock_class_0
    {
        MOCK_METHOD_EXT_ALT( method, void(), method )
    };
    struct mock_class_1
    {
        MOCK_METHOD_EXT_ALT( method, void( float ), method )
    };

    struct mock_class_r0
    {
        MOCK_METHOD_EXT_ALT( method, int(), method )
    };
    struct mock_class_r1
    {
        MOCK_METHOD_EXT_ALT( method, int( float ), method )
    };

    struct forward_declared_only;
    struct defined_but_non_copiable : private boost::noncopyable
    {
    };
    struct base_class
    {
        virtual ~base_class() {}
        virtual void some_method( int, float&, const std::string& ) = 0;
        virtual void some_other_method() = 0;
        virtual int another_method() = 0;
        virtual void yet_another_method( forward_declared_only& ) = 0;
        virtual void yet_again_some_other_method( defined_but_non_copiable& ) = 0;
    };
    MOCK_BASE_CLASS( mock_base_class, base_class )
    {
        MOCK_METHOD_ALT( some_method )
        MOCK_METHOD_ALT( some_other_method )
        MOCK_METHOD_ALT( another_method )
        MOCK_METHOD_ALT( yet_another_method )
        MOCK_METHOD_ALT( yet_again_some_other_method )
    };
}

BOOST_AUTO_TEST_CASE( experimental_macros_slow_as_hell_to_compile )
{
    mock_base_class c;
    MOCK_EXPECT( c, some_method ).once();
    float f = 4.f;
    c.some_method( 3, f, "" );
}
