// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "mock_error.hpp"
#include <turtle/sequence.hpp>
#include <turtle/detail/function.hpp>
#include <boost/test/auto_unit_test.hpp>

BOOST_FIXTURE_TEST_CASE( registering_to_a_sequence_and_calling_out_of_order_throws, mock_error_fixture )
{
    mock::sequence s;
    mock::detail::function< void( int ) > e;
    e.expect().once().with( 1 ).in( s );
    e.expect().once().with( 2 ).in( s );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    CHECK_ERROR( e( 1 ), "sequence failed", 1, "?( 1 )\n. once().with( 1 )\nv once().with( 2 )" );
}

BOOST_FIXTURE_TEST_CASE( registering_to_a_sequence_and_calling_out_of_order_multiple_invocations_throws, mock_error_fixture )
{
    mock::sequence s;
    mock::detail::function< void( int ) > e;
    e.expect().with( 1 ).in( s );
    e.expect().once().with( 2 ).in( s );
    BOOST_CHECK_NO_THROW( e( 1 ) );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    CHECK_ERROR( e( 1 ), "sequence failed", 2, "?( 1 )\n. unlimited().with( 1 )\nv once().with( 2 )" );
}

BOOST_FIXTURE_TEST_CASE( registering_to_a_sequence_and_calling_in_order_is_valid, mock_error_fixture )
{
    mock::sequence s;
    mock::detail::function< void( int ) > e;
    e.expect().once().with( 1 ).in( s );
    e.expect().once().with( 2 ).in( s );
    BOOST_CHECK_NO_THROW( e( 1 ) );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    BOOST_CHECK( e.verify() );
    CHECK_CALLS( 2 );
}

BOOST_FIXTURE_TEST_CASE( registering_to_a_sequence_and_multiply_calling_in_order_is_valid, mock_error_fixture )
{
    mock::sequence s;
    mock::detail::function< void( int ) > e;
    e.expect().exactly( 2 ).with( 1 ).in( s );
    e.expect().exactly( 2 ).with( 2 ).in( s );
    BOOST_CHECK_NO_THROW( e( 1 ) );
    BOOST_CHECK_NO_THROW( e( 1 ) );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    BOOST_CHECK( e.verify() );
    CHECK_CALLS( 4 );
}

BOOST_FIXTURE_TEST_CASE( registering_to_a_sequence_enforces_call_order_verification_between_two_different_expectations, mock_error_fixture )
{
    mock::sequence s;
    mock::detail::function< void() > e1, e2;
    e1.expect().once().in( s );
    e2.expect().once().in( s );
    BOOST_CHECK_NO_THROW( e2() );
    CHECK_ERROR( e1(), "sequence failed", 1, "?()\n. once()" );
}

BOOST_FIXTURE_TEST_CASE( destroying_a_sequence_does_not_remove_order_call_enforcement, mock_error_fixture )
{
    mock::detail::function< void() > e1, e2;
    {
        mock::sequence s;
        e1.expect().once().in( s );
        e2.expect().once().in( s );
    }
    BOOST_CHECK_NO_THROW( e2() );
    CHECK_ERROR( e1(), "sequence failed", 1, "?()\n. once()" );
}

BOOST_FIXTURE_TEST_CASE( resetting_an_expectation_removes_it_from_order_call_enforcement, mock_error_fixture )
{
    mock::sequence s;
    mock::detail::function< void() > e1, e2;
    e1.expect().once().in( s );
    e2.expect().once().in( s );
    e1.reset();
    BOOST_CHECK_NO_THROW( e2() );
    BOOST_CHECK( e1.verify() );
    BOOST_CHECK( e2.verify() );
    CHECK_CALLS( 1 );
}

BOOST_FIXTURE_TEST_CASE( an_expectation_can_be_used_in_several_sequences, mock_error_fixture )
{
    mock::sequence s1, s2;
    mock::detail::function< void() > e;
    e.expect().once().in( s1, s2 );
    BOOST_CHECK_NO_THROW( e() );
    BOOST_CHECK( e.verify() );
    CHECK_CALLS( 1 );
}

BOOST_FIXTURE_TEST_CASE( a_result_specification_is_set_after_a_sequence, mock_error_fixture )
{
    mock::sequence s;
    mock::detail::function< int() > e;
    e.expect().once().in( s ).returns( 3 );
    BOOST_CHECK_EQUAL( 3, e() );
    BOOST_CHECK( e.verify() );
    CHECK_CALLS( 1 );
}
