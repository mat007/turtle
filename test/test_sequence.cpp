// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "mock_error.hpp"
#include <turtle/detail/function.hpp>
#include <turtle/sequence.hpp>
#include <boost/test/unit_test.hpp>
#include <array>

BOOST_FIXTURE_TEST_CASE(registering_to_a_sequence_and_calling_out_of_order_throws, mock_error_fixture)
{
    mock::sequence s;
    mock::detail::function<void(int)> e;
    e.expect().once().with(1).in(s);
    e.expect().once().with(2).in(s);
    BOOST_CHECK_NO_THROW(e(2));
    CHECK_ERROR(e(1), "sequence failed", 1, "?( 1 )\n. once().with( 1 )\nv once().with( 2 )");
}

BOOST_FIXTURE_TEST_CASE(registering_to_a_sequence_and_calling_out_of_order_multiple_invocations_throws,
                        mock_error_fixture)
{
    mock::sequence s;
    mock::detail::function<void(int)> e;
    e.expect().with(1).in(s);
    e.expect().once().with(2).in(s);
    BOOST_CHECK_NO_THROW(e(1));
    BOOST_CHECK_NO_THROW(e(2));
    CHECK_ERROR(e(1), "sequence failed", 2, "?( 1 )\n. unlimited().with( 1 )\nv once().with( 2 )");
}

BOOST_FIXTURE_TEST_CASE(registering_to_a_sequence_and_calling_in_order_is_valid, mock_error_fixture)
{
    mock::sequence s;
    mock::detail::function<void(int)> e;
    e.expect().once().with(1).in(s);
    e.expect().once().with(2).in(s);
    BOOST_CHECK_NO_THROW(e(1));
    BOOST_CHECK_NO_THROW(e(2));
    BOOST_CHECK(e.verify());
    CHECK_CALLS(2);
}

BOOST_FIXTURE_TEST_CASE(registering_to_a_sequence_and_multiply_calling_in_order_is_valid, mock_error_fixture)
{
    mock::sequence s;
    mock::detail::function<void(int)> e;
    e.expect().exactly(2).with(1).in(s);
    e.expect().exactly(2).with(2).in(s);
    BOOST_CHECK_NO_THROW(e(1));
    BOOST_CHECK_NO_THROW(e(1));
    BOOST_CHECK_NO_THROW(e(2));
    BOOST_CHECK_NO_THROW(e(2));
    BOOST_CHECK(e.verify());
    CHECK_CALLS(4);
}

BOOST_FIXTURE_TEST_CASE(registering_to_a_sequence_enforces_call_order_verification_between_two_different_expectations,
                        mock_error_fixture)
{
    mock::sequence s;
    mock::detail::function<void()> e1, e2;
    e1.expect().once().in(s);
    e2.expect().once().in(s);
    BOOST_CHECK_NO_THROW(e2());
    CHECK_ERROR(e1(), "sequence failed", 1, "?()\n. once()");
}

BOOST_FIXTURE_TEST_CASE(destroying_a_sequence_does_not_remove_order_call_enforcement, mock_error_fixture)
{
    mock::detail::function<void()> e1, e2;
    {
        mock::sequence s;
        e1.expect().once().in(s);
        e2.expect().once().in(s);
    }
    BOOST_CHECK_NO_THROW(e2());
    CHECK_ERROR(e1(), "sequence failed", 1, "?()\n. once()");
}

BOOST_FIXTURE_TEST_CASE(resetting_an_expectation_removes_it_from_order_call_enforcement, mock_error_fixture)
{
    mock::sequence s;
    mock::detail::function<void()> e1, e2;
    e1.expect().once().in(s);
    e2.expect().once().in(s);
    e1.reset();
    BOOST_CHECK_NO_THROW(e2());
    BOOST_CHECK(e1.verify());
    BOOST_CHECK(e2.verify());
    CHECK_CALLS(1);
}

BOOST_FIXTURE_TEST_CASE(an_expectation_can_be_used_in_several_sequences, mock_error_fixture)
{
    std::array<mock::sequence, 20> s;
    mock::detail::function<void()> e;
    // One can use any amount of sequences if wanted
    // clang-format off
    e.expect().once().in(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9],
                         s[10], s[11], s[12], s[13], s[14], s[15], s[16], s[17], s[18], s[19]);
    // clang-format on
    BOOST_CHECK_NO_THROW(e());
    BOOST_CHECK(e.verify());
    CHECK_CALLS(1);
}

BOOST_FIXTURE_TEST_CASE(a_result_specification_is_set_after_a_sequence, mock_error_fixture)
{
    mock::sequence s;
    mock::detail::function<int()> e;
    e.expect().once().in(s).returns(3);
    BOOST_CHECK_EQUAL(3, e());
    BOOST_CHECK(e.verify());
    CHECK_CALLS(1);
}
