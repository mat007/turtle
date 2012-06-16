// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/mock.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

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
    const mock_class m;
    MOCK_EXPECT( m.my_method ).once().with( "some parameter" );
    my_function( m );
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
    mock_class_with_operator m;
    MOCK_EXPECT( m.addition ).once().returns( boost::ref( m ) );
    m += 1;
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
    mock_class_with_conversion_operator m;
    MOCK_EXPECT( m.conversion ).once().returns( 42 );
    BOOST_CHECK_EQUAL( 42, static_cast< int >( m ) );
}

namespace
{
    template< typename T >
    MOCK_CLASS( mock_template_class_with_conversion_operator )
    {
        MOCK_CONVERSION_OPERATOR( T, conversion )
    };
}

BOOST_AUTO_TEST_CASE( mock_template_conversion_operator )
{
    mock_template_class_with_conversion_operator< int > m;
    MOCK_EXPECT( m.conversion ).once().returns( 42 );
    BOOST_CHECK_EQUAL( 42, static_cast< int >( m ) );
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
    mock_class_with_const_conversion_operator m;
    MOCK_EXPECT( m.conversion ).once().returns( 42 );
    int i = m;
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
    mock_class_with_non_const_conversion_operator m;
    MOCK_EXPECT( m.conversion ).once().returns( 42 );
    int i = m;
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
    MOCK_VERIFY( m.my_method );
}

BOOST_AUTO_TEST_CASE( MOCK_RESET_macro )
{
    my_mock m;
    MOCK_RESET( m.my_method );
}

BOOST_AUTO_TEST_CASE( MOCK_EXPECT_macro )
{
    my_mock m;
    MOCK_EXPECT( m.my_method ).once().with( 42 );
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

BOOST_AUTO_TEST_CASE( mock_object_is_named )
{
    my_mock m;
    BOOST_CHECK_EQUAL( "?.my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m.my_method ) ) );
    BOOST_CHECK_EQUAL( "?.my_mock::my_method_2", to_string( MOCK_ANONYMOUS_HELPER( m.my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m.my_mock::my_method", to_string( MOCK_HELPER( m.my_method ) ) );
    BOOST_CHECK_EQUAL( "m.my_mock::my_method_2", to_string( MOCK_ANONYMOUS_HELPER( m.my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m.my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m.my_method ) ) );
    BOOST_CHECK_EQUAL( "m.my_mock::my_method", to_string( MOCK_HELPER( m.my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_object_auto_pointer_is_named )
{
    std::auto_ptr< my_mock > m( new my_mock );
    BOOST_CHECK_EQUAL( "?.my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_HELPER( m->my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_object_const_auto_pointer_is_named )
{
    const std::auto_ptr< my_mock > m( new my_mock );
    BOOST_CHECK_EQUAL( "?.my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_HELPER( m->my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_object_shared_pointer_is_named )
{
    boost::shared_ptr< my_mock > m( new my_mock );
    BOOST_CHECK_EQUAL( "?.my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_HELPER( m->my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_object_const_shared_pointer_is_named )
{
    const boost::shared_ptr< my_mock > m( new my_mock );
    BOOST_CHECK_EQUAL( "?.my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m->my_method ) ) );
    BOOST_CHECK_EQUAL( "m->my_mock::my_method", to_string( MOCK_HELPER( m->my_method ) ) );
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
    BOOST_CHECK_EQUAL( "?.my_custom_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m.my_method ) ) );
    BOOST_CHECK_EQUAL( "?.my_custom_mock::my_method_2", to_string( MOCK_ANONYMOUS_HELPER( m.my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m.my_custom_mock::my_method", to_string( MOCK_HELPER( m.my_method ) ) );
    BOOST_CHECK_EQUAL( "m.my_custom_mock::my_method_2", to_string( MOCK_ANONYMOUS_HELPER( m.my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m.my_custom_mock::my_method", to_string( MOCK_ANONYMOUS_HELPER( m.my_method ) ) );
    BOOST_CHECK_EQUAL( "m.my_custom_mock::my_method", to_string( MOCK_HELPER( m.my_method ) ) );
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
    BOOST_CHECK_EQUAL( "?.my_custom_mock_object::my_method", to_string( MOCK_ANONYMOUS_HELPER( m.my_method ) ) );
    BOOST_CHECK_EQUAL( "?.my_custom_mock_object::my_method_2", to_string( MOCK_ANONYMOUS_HELPER( m.my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m.my_custom_mock_object::my_method", to_string( MOCK_HELPER( m.my_method ) ) );
    BOOST_CHECK_EQUAL( "m.my_custom_mock_object::my_method_2", to_string( MOCK_ANONYMOUS_HELPER( m.my_method_2 ) ) );
    BOOST_CHECK_EQUAL( "m.my_custom_mock_object::my_method", to_string( MOCK_ANONYMOUS_HELPER( m.my_method ) ) );
    BOOST_CHECK_EQUAL( "m.my_custom_mock_object::my_method", to_string( MOCK_HELPER( m.my_method ) ) );
}

BOOST_AUTO_TEST_CASE( mock_functor )
{
    MOCK_FUNCTOR( f1, void() );
    MOCK_FUNCTOR( f2, int( const std::string& ) );
}

BOOST_AUTO_TEST_CASE( mock_functor_reset )
{
    MOCK_FUNCTOR( f, void() );
    MOCK_RESET( f );
    mock::reset( f );
}

BOOST_AUTO_TEST_CASE( mock_functor_verify )
{
    MOCK_FUNCTOR( f, void() );
    MOCK_VERIFY( f );
    mock::verify( f );
}

BOOST_AUTO_TEST_CASE( mock_functor_is_named )
{
    MOCK_FUNCTOR( f, void() );
    BOOST_CHECK_EQUAL( "f", to_string( MOCK_HELPER( f ) ) );
}

namespace
{
    MOCK_FUNCTION( mock_function, 1, float( int ), mock_function )
}

BOOST_AUTO_TEST_CASE( mock_function_is_named )
{
    BOOST_CHECK_EQUAL( "mock_function", to_string( MOCK_HELPER( mock_function ) ) );
}

namespace
{
    MOCK_CLASS( static_function_class )
    {
        MOCK_STATIC_METHOD( f, 1, float( int ), f )
    };
}

BOOST_AUTO_TEST_CASE( mock_static_function_is_named )
{
    BOOST_CHECK_EQUAL( "static_function_class::f", to_string( MOCK_HELPER( static_function_class::f ) ) );
}
