//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "mock_error.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define MOCK_ERROR_POLICY mock_error
#include <turtle/mock.hpp>

#define CHECK_ERROR( expr, error ) \
    BOOST_CHECK( verify() ); \
    expr; \
    BOOST_CHECK_EQUAL( 1, error##_count ); \
    reset();

namespace
{
    struct error_fixture
    {
        error_fixture()
        {
            reset();
        }
        ~error_fixture()
        {
            BOOST_CHECK( verify() );
        }
        void reset()
        {
            missing_action_count = 0;
            no_match_count = 0;
            sequence_failed_count = 0;
            verification_failed_count = 0;
            untriggered_expectation_count = 0;
        }
        bool verify() const
        {
            return missing_action_count == 0 &&
                no_match_count == 0 &&
                sequence_failed_count == 0 &&
                verification_failed_count == 0 &&
                untriggered_expectation_count == 0;
        }
    };
}

// functor

BOOST_FIXTURE_TEST_CASE( an_expectation_can_be_passed_as_functor, error_fixture )
{
    mock::expectation< void() > e;
    boost::function< void() > f = e;
}

BOOST_FIXTURE_TEST_CASE( an_expectation_can_be_passed_as_functor_using_boost_bind_and_boost_ref, error_fixture )
{
    mock::expectation< void() > e;
    boost::function< void() > f = boost::bind( boost::ref( e ) );
}

// invocations

BOOST_FIXTURE_TEST_CASE( triggering_an_empty_expectation_calls_no_match_error, error_fixture )
{
    {
        mock::expectation< void() > e;
        CHECK_ERROR( e(), no_match );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        CHECK_ERROR( e( 1, "s" ), no_match );
    }
}

BOOST_FIXTURE_TEST_CASE( triggering_a_never_expectation_calls_no_match_error, error_fixture )
{
    {
        mock::expectation< void() > e;
        e.expect().never();
        CHECK_ERROR( e(), no_match );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect().never();
        CHECK_ERROR( e( 1, "s" ), no_match );
    }
}

BOOST_FIXTURE_TEST_CASE( triggering_an_unlimited_expectation_is_valid, error_fixture )
{
    {
        mock::expectation< void() > e;
        e.expect();
        e();
        e();
    }
    {
        mock::expectation< void( int, const std::string& ) > e;
        e.expect();
        e( 1, "s" );
        e( 1, "s" );
    }
}

BOOST_FIXTURE_TEST_CASE( triggering_a_once_expectation_calls_no_match_error_after_one_call, error_fixture )
{
    {
        mock::expectation< void() > e;
        e.expect().once();
        e();
        CHECK_ERROR( e(), no_match );
    }
    {
        mock::expectation< void( int, const std::string& ) > e;
        e.expect().once();
        e( 1, "s" );
        CHECK_ERROR( e( 1, "s" ), no_match );
    }
}

/*
BOOST_FIXTURE_TEST_CASE( literal_zero_can_be_used_in_expectation_operator_call_as_pointers, error_fixture )
{
    mock::expectation< void( int* ) > e;
    e( 0 );
}
*/

// verify

BOOST_FIXTURE_TEST_CASE( verifying_an_empty_expectation_succeeds, error_fixture )
{
    {
        mock::expectation< void() > e;
        BOOST_CHECK( e.verify() );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        BOOST_CHECK( e.verify() );
    }
}

BOOST_FIXTURE_TEST_CASE( verifying_an_unlimited_expectation_succeeds, error_fixture )
{
    {
        mock::expectation< void() > e;
        e.expect();
        BOOST_CHECK( e.verify() );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect();
        BOOST_CHECK( e.verify() );
    }
}

BOOST_FIXTURE_TEST_CASE( verifying_a_once_expectation_after_one_call_succeeds, error_fixture )
{
    {
        mock::expectation< void() > e;
        e.expect().once();
        e();
        BOOST_CHECK( e.verify() );
    }
    {
        mock::expectation< void( int, const std::string& ) > e;
        e.expect().once();
        e( 1, "s" );
        BOOST_CHECK( e.verify() );
    }
}

BOOST_FIXTURE_TEST_CASE( verifying_a_once_expectation_before_the_call_fails, error_fixture )
{
    {
        mock::expectation< void() > e;
        e.expect().once();
        CHECK_ERROR( BOOST_CHECK( ! e.verify() ), verification_failed );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect().once();
        CHECK_ERROR( BOOST_CHECK( ! e.verify() ), verification_failed );
    }
}

// reset

BOOST_FIXTURE_TEST_CASE( triggering_a_reset_expectation_calls_no_match_error, error_fixture )
{
    {
        mock::expectation< void() > e;
        e.expect();
        e.reset();
        CHECK_ERROR( e(), no_match );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect();
        e.reset();
        CHECK_ERROR( e( 1, "s" ), no_match );
    }
}

BOOST_FIXTURE_TEST_CASE( verifying_a_reset_expectation_succeeds, error_fixture )
{
    {
        mock::expectation< void() > e;
        e.expect();
        e.reset();
        BOOST_CHECK( e.verify() );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect();
        e.reset();
        BOOST_CHECK( e.verify() );
    }
}

// constraints

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_with_wrong_parameter_value_in_equal_constraint_calls_no_match_error, error_fixture )
{
    {
        mock::expectation< void( int ) > e;
        e.expect().with( 42 );
        CHECK_ERROR( e( 43 ), no_match );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect().with( 42, "expected" );
        CHECK_ERROR( e( 42, "actual" ), no_match );
    }
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_with_wrong_parameter_value_in_equal_or_less_constraint_calls_no_match_error, error_fixture )
{
    mock::expectation< void( int ) > e;
    e.expect().with( mock::equal( 42 ) || mock::less( 42 ) );
    e( 41 );
    e( 42 );
    CHECK_ERROR( e( 43 ), no_match );
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_with_wrong_parameter_value_in_equal_and_not_less_constraint_calls_no_match_error, error_fixture )
{
    mock::expectation< void( int ) > e;
    e.expect().with( mock::equal( 42 ) && ! mock::less( 41 ) );
    e( 42 );
    CHECK_ERROR( e( 43 ), no_match );
}

namespace
{
    class my_interface : boost::noncopyable
    {
    public:
        virtual ~my_interface() {}
    private:
        virtual void my_method() = 0;
    };
    class my_implementation : public my_interface
    {
        virtual void my_method() {}
    };
}

BOOST_FIXTURE_TEST_CASE( passing_call_values_by_reference_is_transparent, error_fixture )
{
    {
        mock::expectation< void( my_interface& ) > e;
        my_implementation imp;
        e.expect().with( mock::same( imp ) );
        e( imp );
    }
    {
        mock::expectation< void( const my_interface& ) > e;
        my_implementation imp;
        e.expect().with( mock::same( imp ) );
        e( imp );
    }
}

namespace
{
    bool custom_constraint( int )
    {
        return false;
    }
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_with_failing_custom_constraint_calls_no_match_error, error_fixture )
{
    {
        mock::expectation< void( int ) > e;
        e.expect().with( &custom_constraint );
        CHECK_ERROR( e( 42 ), no_match );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect().with( &custom_constraint, "actual" );
        CHECK_ERROR( e( 42, "actual" ), no_match );
    }
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_with_wrong_parameter_value_in_custom_constraint_calls_no_match_error, error_fixture )
{
    mock::expectation< void( int ) > e;
    e.expect().with( mock::constraint( &custom_constraint, "custom constraint" ) );
    CHECK_ERROR( e( 42 ), no_match );
}

/*
BOOST_FIXTURE_TEST_CASE( literal_zero_can_be_used_in_place_of_null_pointers_in_constraints, error_fixture )
{
    mock::expectation< void( int* ) > e;
    e.expect().with( 0 );
    e.reset();
}
*/

// result handling

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_with_no_return_set_calls_missing_action, error_fixture )
{
    {
        mock::expectation< int() > e;
        e.expect();
        CHECK_ERROR( e(), missing_action );
    }
    {
        mock::expectation< int&() > e;
        e.expect();
        CHECK_ERROR( e(), missing_action );
    }
    {
        mock::expectation< const std::string&() > e;
        e.expect();
        CHECK_ERROR( e(), missing_action );
    }
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_returns_the_set_value, error_fixture )
{
    {
        mock::expectation< int() > e;
        e.expect().returns( 42 );
        BOOST_CHECK_EQUAL( 42, e() );
    }
    {
        mock::expectation< int() > e;
        const int i = 42;
        e.expect().returns( i );
        BOOST_CHECK_EQUAL( i, e() );
    }
    {
        mock::expectation< int() > e;
        int i = 42;
        e.expect().returns( boost::ref( i ) );
        i = 43;
        BOOST_CHECK_EQUAL( 43, e() );
    }
    {
        mock::expectation< int&() > e;
        e.expect().returns( 42 );
        BOOST_CHECK_EQUAL( 42, e() );
    }
    {
        mock::expectation< int&() > e;
        const int result = 42;
        e.expect().returns( result );
        BOOST_CHECK_EQUAL( result, e() );
    }
    {
        mock::expectation< int&() > e;
        int i = 42;
        e.expect().returns( i );
        i = 43;
        BOOST_CHECK_EQUAL( 42, e() );
    }
    {
        mock::expectation< int&() > e;
        int i = 42;
        e.expect().returns( boost::ref( i ) );
        i = 43;
        BOOST_CHECK_EQUAL( 43, e() );
        BOOST_CHECK_EQUAL( 12, e() = 12 );
        BOOST_CHECK_EQUAL( 12, i );
    }
    {
        mock::expectation< std::string() > e;
        e.expect().returns( "result" );
        BOOST_CHECK_EQUAL( "result", e() );
    }
    {
        mock::expectation< const std::string&() > e;
        e.expect().returns( "result" );
        BOOST_CHECK_EQUAL( "result", e() );
    }
    {
        mock::expectation< int*() > e;
        e.expect().returns( 0 );
        BOOST_CHECK( ! e() );
    }
    {
        mock::expectation< int() > e;
        e.expect().returns( 0 );
        BOOST_CHECK_EQUAL( 0, e() );
    }
    {
        mock::expectation< int&() > e;
        e.expect().returns( 0 );
        BOOST_CHECK_EQUAL( 0, e() );
    }
}

namespace
{
    struct A {};
    struct B : A {};
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_returns_the_set_auto_ptr_value, error_fixture )
{
    {
        mock::expectation< std::auto_ptr< int >() > e;
        std::auto_ptr< int > ptr( new int( 3 ) );
        e.expect().returns( boost::ref( ptr ) );
        BOOST_CHECK_EQUAL( 3, *ptr );
        BOOST_CHECK_EQUAL( 3, *e() );
        BOOST_CHECK( ! ptr.get() );
        BOOST_CHECK( ! e().get() );
    }
    {
        mock::expectation< std::auto_ptr< int >() > e;
        std::auto_ptr< int > ptr( new int( 3 ) );
        e.expect().returns( ptr );
        BOOST_CHECK( ! ptr.get() );
        BOOST_CHECK_EQUAL( 3, *e() );
        BOOST_CHECK( ! e().get() );
    }
    {
        mock::expectation< std::auto_ptr< int >() > e;
        e.expect().returns( new int( 3 ) );
        BOOST_CHECK_EQUAL( 3, *e() );
        BOOST_CHECK( ! e().get() );
    }
    {
        mock::expectation< std::auto_ptr< int >() > e;
        e.expect().returns( std::auto_ptr< int >( new int( 3 ) ) );
        BOOST_CHECK_EQUAL( 3, *e() );
        BOOST_CHECK( ! e().get() );
    }
    {
        mock::expectation< std::auto_ptr< A >() > e;
        e.expect().returns( new B );
        BOOST_CHECK_NO_THROW( e() );
    }
    {
        mock::expectation< std::auto_ptr< A >() > e;
        e.expect().returns( std::auto_ptr< A >( new B ) );
        BOOST_CHECK_NO_THROW( e() );
    }
    {
        mock::expectation< std::auto_ptr< A >() > e;
        e.expect().returns( std::auto_ptr< B >( new B ) );
        BOOST_CHECK_NO_THROW( e() );
    }
}

namespace
{
    int custom_result()
    {
        return 42;
    }
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_calls_the_custom_functor, error_fixture )
{
    mock::expectation< int() > e;
    e.expect().calls( &custom_result );
    BOOST_CHECK_EQUAL( 42, e() );
}

namespace
{
    int custom_result_with_parameter( int i )
    {
        return i;
    }
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_calls_the_custom_functor_with_parameters, error_fixture )
{
    mock::expectation< int( int ) > e;
    e.expect().calls( &custom_result_with_parameter );
    BOOST_CHECK_EQUAL( 42, e( 42 ) );
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_calls_the_custom_functor_without_parameters_thanks_to_boost_bind, error_fixture )
{
    mock::expectation< int( int ) > e;
    e.expect().calls( boost::bind( &custom_result ) );
    BOOST_CHECK_EQUAL( 42, e( 17 ) );
}

BOOST_FIXTURE_TEST_CASE( triggering_an_expectation_throws_the_set_exception, error_fixture )
{
    mock::expectation< void() > e;
    e.expect().throws( std::runtime_error( "some exception" ) );
    try
    {
        e();
    }
    catch( std::runtime_error& e )
    {
        BOOST_CHECK_EQUAL( "some exception", e.what() );
        return;
    }
    BOOST_FAIL( "should have thrown" );
}

// multiple matchers

BOOST_FIXTURE_TEST_CASE( expecting_twice_a_single_expectation_makes_it_callable_twice, error_fixture )
{
    {
        mock::expectation< void() > e;
        e.expect().once();
        e.expect().once();
        e();
        e();
        CHECK_ERROR( e(), no_match );
    }
    {
        mock::expectation< void( const std::string& ) > e;
        e.expect().once().with( "first" );
        e.expect().once().with( "second" );
        e( "first" );
        e( "second" );
        CHECK_ERROR( e( "third"), no_match );
    }
}

BOOST_FIXTURE_TEST_CASE( best_matcher_is_selected_first, error_fixture )
{
    {
        mock::expectation< void( int ) > e;
        e.expect().once().with( 1 );
        e.expect().once().with( 2 );
        e( 2 );
        e( 1 );
        CHECK_ERROR( e( 3 ), no_match );
    }
    {
        mock::expectation< void( const std::string& ) > e;
        e.expect().once().with( "first" );
        e.expect().once().with( "second" );
        e( "second" );
        e( "first" );
        CHECK_ERROR( e( "third"), no_match );
    }
}

// error report

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

BOOST_FIXTURE_TEST_CASE( expectation_can_be_serialized_to_be_human_readable, error_fixture )
{
    {
        mock::expectation< void( int ) > e;
        e.tag( "my expectation" );
        e.expect().once().with( 1 );
        e.expect().once().with( 2 );
        BOOST_CHECK_NO_THROW( e( 2 ) );
        const std::string expected = "my expectation\n"
                                     ". once().with( 1 )\n"
                                     "v once().with( 2 )";
        BOOST_CHECK_EQUAL( expected, to_string( e ) );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e;
        e.tag( "my expectation" );
        e.expect().never().with( 1 );
        const std::string expected = "my expectation\n"
                                     "v never().with( 1 )";
        BOOST_CHECK_EQUAL( expected, to_string( e ) );
        e.reset();
    }
    {
        mock::expectation< void( const std::string& ) > e;
        e.expect().never().with( mock::less( "first" ) );
        e.expect().exactly( 2 ).with( "second" );
        BOOST_CHECK_NO_THROW( e( "second" ) );
        {
            const std::string expected = "?\n"
                                         "v never().with( less( \"first\" ) )\n"
                                         ". exactly( 1/2 ).with( \"second\" )";
            BOOST_CHECK_EQUAL( expected, to_string( e ) );
        }
        BOOST_CHECK_NO_THROW( e( "second" ) );
        {
            const std::string expected = "?\n"
                                         "v never().with( less( \"first\" ) )\n"
                                         "v exactly( 2/2 ).with( \"second\" )";
            BOOST_CHECK_EQUAL( expected, to_string( e ) );
        }
        e.reset();
    }
    {
        mock::expectation< void( int ) > e;
        e.tag( "my expectation" );
        e.expect().once();
        const std::string expected = "my expectation\n"
                                     ". once().with( any )";
        BOOST_CHECK_EQUAL( expected, to_string( e ) );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e;
        e.tag( "my expectation" );
        e.expect().once().with( mock::any );
        const std::string expected = "my expectation\n"
                                     ". once().with( any )";
        BOOST_CHECK_EQUAL( expected, to_string( e ) );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e;
        e.tag( "my expectation" );
        e.expect().once();
        const std::string expected = "my expectation\n"
                                     ". once().with( any )";
        BOOST_CHECK_EQUAL( expected, to_string( e ) );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e;
        e.tag( "my expectation" );
        e.expect().once().with( &custom_constraint );
        const std::string expected = "my expectation\n"
                                     ". once().with( ? )";
        BOOST_CHECK_EQUAL( expected, to_string( e ) );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e;
        e.tag( "my expectation" );
        e.expect().once().with( mock::constraint( &custom_constraint, "custom constraint" ) );
        const std::string expected = "my expectation\n"
                                     ". once().with( custom constraint )";
        BOOST_CHECK_EQUAL( expected, to_string( e ) );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e;
        e.tag( "my expectation" );
        e.expect().once().with( mock::constraint( &custom_constraint, true ) );
        const std::string expected = "my expectation\n"
                                     ". once().with( true )";
        BOOST_CHECK_EQUAL( expected, to_string( e ) );
        e.reset();
    }
}

BOOST_FIXTURE_TEST_CASE( expectation_with_remaining_untriggered_matches_upon_destruction_calls_untriggered_expectation, error_fixture )
{
    std::auto_ptr< mock::expectation< void() > > e( new mock::expectation< void() > );
    e->expect().once();
    CHECK_ERROR( e.reset(), untriggered_expectation );
}

BOOST_FIXTURE_TEST_CASE( verifying_expectation_with_remaining_matches_disables_the_automatic_verification_upon_destruction, error_fixture )
{
    mock::expectation< void() > e;
    e.expect().once();
    CHECK_ERROR( e.verify(), verification_failed );
}

BOOST_FIXTURE_TEST_CASE( triggering_no_match_call_disables_the_automatic_verification_upon_destruction, error_fixture )
{
    mock::expectation< void() > e;
    CHECK_ERROR( e(), no_match );
}

BOOST_FIXTURE_TEST_CASE( adding_a_matcher_reactivates_the_verification_upon_destruction, error_fixture )
{
    std::auto_ptr< mock::expectation< void() > > e( new mock::expectation< void() > );
    CHECK_ERROR( (*e)(), no_match );
    e->expect().once();
    CHECK_ERROR( e.reset(), untriggered_expectation );
}

BOOST_FIXTURE_TEST_CASE( throwing_an_exception_disables_the_automatic_verification_upon_destruction, error_fixture )
{
    try
    {
        mock::expectation< void() > e;
        e.expect().once();
        throw std::exception();
    }
    catch( std::exception& )
    {}
}
