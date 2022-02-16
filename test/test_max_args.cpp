// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "mock_error.hpp"
#include <turtle/mock.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/test/unit_test.hpp>

#define IDENTITY(z, n, text) text
// Number of arguments is (now) unlimited, so take any high value here
#define NUM_TEST_ARGS 100

namespace {
struct my_custom_mock
{
    MOCK_METHOD(method, NUM_TEST_ARGS, void(BOOST_PP_ENUM(NUM_TEST_ARGS, IDENTITY, int)), tag)
    MOCK_METHOD(method2, NUM_TEST_ARGS, int(BOOST_PP_ENUM(NUM_TEST_ARGS, IDENTITY, int)), tag_2)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(call_mock_method_with_max_number_of_args, mock_error_fixture)
{
    my_custom_mock m;
    MOCK_EXPECT(m.tag).once().with(BOOST_PP_ENUM(NUM_TEST_ARGS, IDENTITY, 0));
    m.method(BOOST_PP_ENUM(NUM_TEST_ARGS, IDENTITY, 0));
    CHECK_CALLS(1);
}

BOOST_FIXTURE_TEST_CASE(call_mock_method_with_max_number_of_args_and_a_return_value, mock_error_fixture)
{
    my_custom_mock m;
    MOCK_EXPECT(m.tag_2).once().with(BOOST_PP_ENUM(NUM_TEST_ARGS, IDENTITY, 0)).returns(42);
    BOOST_CHECK_EQUAL(42, m.method2(BOOST_PP_ENUM(NUM_TEST_ARGS, IDENTITY, 0)));
    CHECK_CALLS(1);
}
