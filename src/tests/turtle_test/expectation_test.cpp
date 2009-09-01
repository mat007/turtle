//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_AUTO_TEST_MAIN
#include <turtle/expectation.hpp>
#include <turtle/constraint.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    bool check_exception_contains( const mock::exception& e, const std::string& str )
    {
        return e.what().find( str ) != std::string::npos;
    }
}
#define BOOST_CHECK_EXCEPTION_CONTAINS( S, C ) \
    BOOST_CHECK_EXCEPTION( S, mock::exception, boost::bind( &check_exception_contains, _1, C ) );

// functor

BOOST_AUTO_TEST_CASE( an_expectation_can_be_passed_as_functor )
{
    mock::expectation< void() > e;
    boost::function< void() > f = e;
}

BOOST_AUTO_TEST_CASE( an_expectation_can_be_passed_as_functor_using_boost_bind_and_boost_ref )
{
    mock::expectation< void() > e;
    boost::function< void() > f = boost::bind( boost::ref( e ) );
}

// invocations

BOOST_AUTO_TEST_CASE( triggering_an_empty_expectation_throws )
{
    {
        mock::expectation< void() > e;
        BOOST_CHECK_EXCEPTION_CONTAINS( e(), "unexpected call" );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 1, "s" ), "unexpected call" );
    }
}

BOOST_AUTO_TEST_CASE( triggering_a_never_expectation_throws )
{
    {
        mock::expectation< void() > e;
        e.expect().never();
        BOOST_CHECK_EXCEPTION_CONTAINS( e(), "unexpected call" );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect().never();
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 1, "s" ), "unexpected call" );
    }
}

BOOST_AUTO_TEST_CASE( triggering_an_unlimited_expectation_never_throws )
{
    {
        mock::expectation< void() > e;
        e.expect();
        BOOST_CHECK_NO_THROW( e() );
        BOOST_CHECK_NO_THROW( e() );
    }
    {
        mock::expectation< void( int, const std::string& ) > e;
        e.expect();
        BOOST_CHECK_NO_THROW( e( 1, "s" ) );
        BOOST_CHECK_NO_THROW( e( 1, "s" ) );
    }
}

BOOST_AUTO_TEST_CASE( triggering_a_once_expectation_throws_after_one_call )
{
    {
        mock::expectation< void() > e;
        e.expect().once();
        BOOST_CHECK_NO_THROW( e() );
        BOOST_CHECK_EXCEPTION_CONTAINS( e(), "unexpected call" );
    }
    {
        mock::expectation< void( int, const std::string& ) > e;
        e.expect().once();
        BOOST_CHECK_NO_THROW( e( 1, "s" ) );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 1, "s" ), "unexpected call" );
    }
}

/*
BOOST_AUTO_TEST_CASE( literal_zero_can_be_used_in_expectation_operator_call_as_pointers )
{
    mock::expectation< void( int* ) > e;
    e( 0 );
}
*/

// verify

BOOST_AUTO_TEST_CASE( verifying_an_empty_expectation_succeeds )
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

BOOST_AUTO_TEST_CASE( verifying_an_unlimited_expectation_succeeds )
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

BOOST_AUTO_TEST_CASE( verifying_a_once_expectation_after_one_call_succeeds )
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

namespace
{
    template< typename Result >
    struct silent_error
    {
        static Result no_match( const std::string& context )
        {
            throw std::runtime_error( __FUNCTION__ );
        }
        static void sequence_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
        static void verification_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
        static void untriggered_expectation( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
    };
}

BOOST_AUTO_TEST_CASE( verifying_a_once_expectation_before_the_call_fails )
{
    {
        mock::expectation< void(), silent_error< void > > e;
        e.expect().once();
        BOOST_CHECK( ! e.verify() );
    }
    {
        mock::expectation< int( int, const std::string& ), silent_error< int > > e;
        e.expect().once();
        BOOST_CHECK( ! e.verify() );
    }
}

// reset

BOOST_AUTO_TEST_CASE( triggering_a_reset_expectation_throws )
{
    {
        mock::expectation< void() > e;
        e.expect();
        e.reset();
        BOOST_CHECK_EXCEPTION_CONTAINS( e(), "unexpected call" );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect();
        e.reset();
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 1, "s" ), "unexpected call" );
    }
}

BOOST_AUTO_TEST_CASE( verifying_a_reset_expectation_succeeds )
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

BOOST_AUTO_TEST_CASE( triggering_an_expectation_with_wrong_parameter_value_in_equal_constraint_throws )
{
    {
        mock::expectation< void( int ) > e;
        e.expect().with( 42 );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 43 ), "unexpected call" );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect().with( 42, "expected" );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 42, "actual" ), "unexpected call" );
    }
}

BOOST_AUTO_TEST_CASE( passing_raw_parameter_as_constraint_falls_back_to_using_the_equal_constraint )
{
    {
        mock::expectation< void( int ) > e;
        e.expect().with( 42 );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 43 ), "unexpected call" );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect().with( 42, "expected" );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 42, "actual" ), "unexpected call" );
    }
}

BOOST_AUTO_TEST_CASE( triggering_an_expectation_with_wrong_parameter_value_in_equal_or_less_constraint_throws )
{
    mock::expectation< void( int ) > e;
    e.expect().with( mock::equal( 42 ) || mock::less( 42 ) );
    BOOST_CHECK_NO_THROW( e( 41 ) );
    BOOST_CHECK_NO_THROW( e( 42 ) );
    BOOST_CHECK_EXCEPTION_CONTAINS( e( 43 ), "unexpected call" );
}

BOOST_AUTO_TEST_CASE( triggering_an_expectation_with_wrong_parameter_value_in_equal_and_not_less_constraint_throws )
{
    mock::expectation< void( int ) > e;
    e.expect().with( mock::equal( 42 ) && ! mock::less( 41 ) );
    BOOST_CHECK_NO_THROW( e( 42 ) );
    BOOST_CHECK_EXCEPTION_CONTAINS( e( 43 ), "unexpected call" );
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

BOOST_AUTO_TEST_CASE( passing_call_values_by_reference_is_transparent )
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

BOOST_AUTO_TEST_CASE( triggering_an_expectation_with_failing_custom_constraint_throws )
{
    {
        mock::expectation< void( int ) > e;
        e.expect().with( &custom_constraint );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 42 ), "unexpected call" );
    }
    {
        mock::expectation< int( int, const std::string& ) > e;
        e.expect().with( &custom_constraint, "actual" );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 42, "actual" ), "unexpected call" );
    }
}

BOOST_AUTO_TEST_CASE( triggering_an_expectation_with_wrong_parameter_value_in_custom_constraint_throws )
{
    mock::expectation< void( int ) > e;
    e.expect().with( mock::constraint( &custom_constraint, "custom constraint" ) );
    BOOST_CHECK_EXCEPTION_CONTAINS( e( 42 ), "custom constraint" );
}
/*
BOOST_AUTO_TEST_CASE( literal_zero_can_be_used_in_place_of_null_pointers_in_constraints )
{
    mock::expectation< void( int* ) > e;
    e.expect().with( 0 );
    e.reset();
}
*/

// result handling

BOOST_AUTO_TEST_CASE( triggering_an_expectation_with_no_return_set_throws )
{
    {
        mock::expectation< int() > e;
        e.expect();
        BOOST_CHECK_EXCEPTION_CONTAINS( e(), "missing result specification" );
    }
    {
        mock::expectation< int&() > e;
        e.expect();
        BOOST_CHECK_EXCEPTION_CONTAINS( e(), "missing result specification" );
    }
    {
        mock::expectation< const std::string&() > e;
        e.expect();
        BOOST_CHECK_EXCEPTION_CONTAINS( e(), "missing result specification" );
    }
}

BOOST_AUTO_TEST_CASE( triggering_an_expectation_returns_the_set_value )
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

BOOST_AUTO_TEST_CASE( triggering_an_expectation_returns_the_set_auto_ptr_value )
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

BOOST_AUTO_TEST_CASE( triggering_an_expectation_calls_the_custom_functor )
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

BOOST_AUTO_TEST_CASE( triggering_an_expectation_calls_the_custom_functor_with_parameters )
{
    mock::expectation< int( int ) > e;
    e.expect().calls( &custom_result_with_parameter );
    BOOST_CHECK_EQUAL( 42, e( 42 ) );
}

BOOST_AUTO_TEST_CASE( triggering_an_expectation_calls_the_custom_functor_without_parameters_thanks_to_boost_bind )
{
    mock::expectation< int( int ) > e;
    e.expect().calls( boost::bind( &custom_result ) );
    BOOST_CHECK_EQUAL( 42, e( 17 ) );
}

BOOST_AUTO_TEST_CASE( triggering_an_expectation_throws_the_set_exception )
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

BOOST_AUTO_TEST_CASE( expecting_twice_a_single_expectation_makes_it_callable_twice )
{
    {
        mock::expectation< void() > e;
        e.expect().once();
        e.expect().once();
        BOOST_CHECK_NO_THROW( e() );
        BOOST_CHECK_NO_THROW( e() );
        BOOST_CHECK_EXCEPTION_CONTAINS( e(), "unexpected call" );
    }
    {
        mock::expectation< void( const std::string& ) > e;
        e.expect().once().with( "first" );
        e.expect().once().with( "second" );
        BOOST_CHECK_NO_THROW( e( "first" ) );
        BOOST_CHECK_NO_THROW( e( "second" ) );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( "third" ), "unexpected call" );
    }
}

BOOST_AUTO_TEST_CASE( best_matcher_is_selected_first )
{
    {
        mock::expectation< void( int ) > e;
        e.expect().once().with( 1 );
        e.expect().once().with( 2 );
        BOOST_CHECK_NO_THROW( e( 2 ) );
        BOOST_CHECK_NO_THROW( e( 1 ) );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( 3 ), "unexpected call" );
    }
    {
        mock::expectation< void( const std::string& ) > e;
        e.expect().once().with( "first" );
        e.expect().once().with( "second" );
        BOOST_CHECK_NO_THROW( e( "second" ) );
        BOOST_CHECK_NO_THROW( e( "first" ) );
        BOOST_CHECK_EXCEPTION_CONTAINS( e( "third" ), "unexpected call" );
    }
}

// error report

BOOST_AUTO_TEST_CASE( expectation_can_be_serialized_to_be_human_readable )
{
    {
        mock::expectation< void( int ) > e( "my expectation" );
        e.expect().once().with( 1 );
        e.expect().once().with( 2 );
        BOOST_CHECK_NO_THROW( e( 2 ) );
        std::stringstream s;
        s << e;
        const std::string expected = "my expectation\n"
                                     ". expect( once() ).with( 1 )\n"
                                     "v expect( once() ).with( 2 )";
        BOOST_CHECK_EQUAL( expected, s.str() );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e( "my expectation" );
        e.expect().never().with( 1 );
        std::stringstream s;
        s << e;
        const std::string expected = "my expectation\n"
                                     "v expect( never() ).with( 1 )";
        BOOST_CHECK_EQUAL( expected, s.str() );
        e.reset();
    }
    {
        mock::expectation< void( const std::string& ) > e;
        e.expect().never().with( mock::less( "first" ) );
        e.expect().exactly( 2 ).with( "second" );
        BOOST_CHECK_NO_THROW( e( "second" ) );
        {
            std::stringstream s;
            s << e;
            const std::string expected = "?\n"
                                         "v expect( never() ).with( less( \"first\" ) )\n"
                                         ". expect( exactly( 1/2 ) ).with( \"second\" )";
            BOOST_CHECK_EQUAL( expected, s.str() );
        }
        BOOST_CHECK_NO_THROW( e( "second" ) );
        {
            std::stringstream s;
            s << e;
            const std::string expected = "?\n"
                                         "v expect( never() ).with( less( \"first\" ) )\n"
                                         "v expect( exactly( 2/2 ) ).with( \"second\" )";
            BOOST_CHECK_EQUAL( expected, s.str() );
        }
        e.reset();
    }
    {
        mock::expectation< void( int ) > e( "my expectation" );
        e.expect().once();
        std::stringstream s;
        s << e;
        const std::string expected = "my expectation\n"
                                     ". expect( once() ).with( any )";
        BOOST_CHECK_EQUAL( expected, s.str() );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e( "my expectation" );
        e.expect().once().with( mock::any );
        std::stringstream s;
        s << e;
        const std::string expected = "my expectation\n"
                                     ". expect( once() ).with( any )";
        BOOST_CHECK_EQUAL( expected, s.str() );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e( "my expectation" );
        e.expect().once();
        std::stringstream s;
        s << e;
        const std::string expected = "my expectation\n"
                                     ". expect( once() ).with( any )";
        BOOST_CHECK_EQUAL( expected, s.str() );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e( "my expectation" );
        e.expect().once().with( &custom_constraint );
        std::stringstream s;
        s << e;
        const std::string expected = "my expectation\n"
                                     ". expect( once() ).with( ? )";
        BOOST_CHECK_EQUAL( expected, s.str() );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e( "my expectation" );
        e.expect().once().with( mock::constraint( &custom_constraint, "custom constraint" ) );
        std::stringstream s;
        s << e;
        const std::string expected = "my expectation\n"
                                     ". expect( once() ).with( custom constraint )";
        BOOST_CHECK_EQUAL( expected, s.str() );
        e.reset();
    }
    {
        mock::expectation< void( int ) > e( "my expectation" );
        e.expect().once().with( mock::constraint( &custom_constraint, true ) );
        std::stringstream s;
        s << e;
        const std::string expected = "my expectation\n"
                                     ". expect( once() ).with( true )";
        BOOST_CHECK_EQUAL( expected, s.str() );
        e.reset();
    }
}

namespace
{
    mock::expectation< void() > no_match_exp( "no_match" );
    mock::expectation< void() > sequence_failed_exp( "sequence_failed" );
    mock::expectation< void() > verification_failed_exp( "verification_failed" );
    mock::expectation< void() > untriggered_expectation_exp( "untriggered_expectation" );

    struct mock_error
    {
        static void no_match( const std::string& /*context*/ )
        {
            no_match_exp();
        }
        static void sequence_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            sequence_failed_exp();
        }
        static void verification_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            verification_failed_exp();
        }
        static void untriggered_expectation( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            untriggered_expectation_exp();
        }
    };
    struct error_guard
    {
        ~error_guard()
        {
            no_match_exp.verify();
            no_match_exp.reset();
            sequence_failed_exp.verify();
            sequence_failed_exp.reset();
            verification_failed_exp.verify();
            verification_failed_exp.reset();
            untriggered_expectation_exp.verify();
            untriggered_expectation_exp.reset();
        }
    };
}

BOOST_FIXTURE_TEST_CASE( expectation_with_remaining_untriggered_matches_notifies_an_error_upon_destructions, error_guard )
{
    mock::expectation< void(), mock_error > e;
    e.expect().once();
    untriggered_expectation_exp.expect().once();
}

BOOST_FIXTURE_TEST_CASE( verifying_expectation_with_remaining_matches_disables_the_automatic_verification_upon_destruction, error_guard )
{
    mock::expectation< void(), mock_error > e;
    e.expect().once();
    verification_failed_exp.expect();
    e.verify();
}

BOOST_FIXTURE_TEST_CASE( triggering_no_match_call_disables_the_automatic_verification_upon_destruction, error_guard )
{
    mock::expectation< void(), mock_error > e;
    no_match_exp.expect();
    e();
}

BOOST_FIXTURE_TEST_CASE( adding_a_matcher_reactivates_the_verification_upon_destruction, error_guard )
{
    mock::expectation< void(), mock_error > e;
    no_match_exp.expect();
    e();
    e.expect().once();
    untriggered_expectation_exp.expect().once();
}

BOOST_FIXTURE_TEST_CASE( throwing_an_exception_disables_the_automatic_verification_upon_destruction, error_guard )
{
    try
    {
        mock::expectation< void(), mock_error > e;
        e.expect().once();
        throw std::exception();
    }
    catch( std::exception& )
    {}
}
