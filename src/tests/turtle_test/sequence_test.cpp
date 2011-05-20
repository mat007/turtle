//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "mock_error.hpp"
#include <turtle/sequence.hpp>
#include <turtle/function.hpp>
#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE( registering_to_a_sequence_and_calling_out_of_order_throws )
{
    mock::sequence s;
    mock::function< void( int ) > e;
    e.expect().once().with( 1 ).in( s );
    e.expect().once().with( 2 ).in( s );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    BOOST_CHECK_THROW( e( 1 ), std::exception );
}

BOOST_AUTO_TEST_CASE( registering_to_a_sequence_and_calling_out_of_order_multiple_invocations_throws )
{
    mock::sequence s;
    mock::function< void( int ) > e;
    e.expect().with( 1 ).in( s );
    e.expect().once().with( 2 ).in( s );
    BOOST_CHECK_NO_THROW( e( 1 ) );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    BOOST_CHECK_THROW( e( 1 ), std::exception );
}

BOOST_AUTO_TEST_CASE( registering_to_a_sequence_and_calling_in_order_is_valid )
{
    mock::sequence s;
    mock::function< void( int ) > e;
    e.expect().once().with( 1 ).in( s );
    e.expect().once().with( 2 ).in( s );
    BOOST_CHECK_NO_THROW( e( 1 ) );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    BOOST_CHECK( e.verify() );
}

BOOST_AUTO_TEST_CASE( registering_to_a_sequence_and_multiply_calling_in_order_is_valid )
{
    mock::sequence s;
    mock::function< void( int ) > e;
    e.expect().exactly( 2 ).with( 1 ).in( s );
    e.expect().exactly( 2 ).with( 2 ).in( s );
    BOOST_CHECK_NO_THROW( e( 1 ) );
    BOOST_CHECK_NO_THROW( e( 1 ) );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    BOOST_CHECK_NO_THROW( e( 2 ) );
    BOOST_CHECK( e.verify() );
}

BOOST_AUTO_TEST_CASE( registering_to_a_sequence_enforces_call_order_verification_between_two_different_expectations )
{
    mock::sequence s;
    mock::function< void() > e1, e2;
    e1.expect().once().in( s );
    e2.expect().once().in( s );
    BOOST_CHECK_NO_THROW( e2() );
    BOOST_CHECK_THROW( e1(), std::exception );
}

BOOST_AUTO_TEST_CASE( destroying_a_sequence_does_not_remove_order_call_enforcement )
{
    mock::function< void() > e1, e2;
    {
        mock::sequence s;
        e1.expect().once().in( s );
        e2.expect().once().in( s );
    }
    BOOST_CHECK_NO_THROW( e2() );
    BOOST_CHECK_THROW( e1(), std::exception );
}

BOOST_AUTO_TEST_CASE( resetting_an_expectation_removes_it_from_order_call_enforcement )
{
    mock::sequence s;
    mock::function< void() > e1, e2;
    e1.expect().once().in( s );
    e2.expect().once().in( s );
    e1.reset();
    BOOST_CHECK_NO_THROW( e2() );
    BOOST_CHECK( e1.verify() );
    BOOST_CHECK( e2.verify() );
}

BOOST_AUTO_TEST_CASE( an_expectation_can_be_used_in_several_sequences )
{
    mock::sequence s1, s2;
    mock::function< void() > e;
    e.expect().once().in( s1 ).in( s2 );
    BOOST_CHECK_NO_THROW( e() );
    BOOST_CHECK( e.verify() );
}

BOOST_AUTO_TEST_CASE( a_result_specification_is_set_after_a_sequence )
{
    mock::sequence s;
    mock::function< int() > e;
    e.expect().once().in( s ).returns( 3 );
    BOOST_CHECK_EQUAL( 3, e() );
    BOOST_CHECK( e.verify() );
}
