// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_TEST_MOCK_ERROR_HPP_INCLUDED
#define MOCK_TEST_MOCK_ERROR_HPP_INCLUDED

#define MOCK_ERROR_POLICY mock_error
#include <turtle/detail/singleton.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <stdexcept>

/// Container to hold data about mocked function calls (and expectation errors)
struct mock_error_data_t : mock::detail::singleton<mock_error_data_t>
{
    void reset()
    {
        call_count = 0;
        error_count = 0;
        last_message.clear();
        last_context.clear();
    }
    bool verify() { return error_count == 0; }

    void call()
    {
        last_context.clear();
        last_message.clear();
        ++call_count;
    }
    void fail(const std::string& message, const std::string& context, const char* file, int line)
    {
        last_context = context;
        last_message = message;
        last_file = file;
        last_line = line;
        ++error_count;
    }

    int call_count = 0;
    int error_count = 0;
    std::string last_message, last_context, last_file;
    int last_line = 0;
    MOCK_SINGLETON_CONS(mock_error_data_t);
};
MOCK_SINGLETON_INST(mock_error_data)

/// Error handler that populates the mock_error_data singleton instead of failing
template<typename Result>
struct mock_error
{
    static Result abort() { throw std::runtime_error("aborted"); }

    static void pass(const char* /*file*/, int /*line*/) {}

    template<typename Context>
    static void call(const Context& /*context*/, const char* /*file*/, int /*line*/)
    {
        mock_error_data.call();
    }

    template<typename Context>
    static void fail(const std::string& message, const Context& context, const char* file = "", int line = 0)
    {
        std::ostringstream s;
        s << context; // Context can be streamed
        mock_error_data.fail(message, s.str(), file, line);
    }
};

/// Fixture to use CHECK_CALLS & CHECK_ERROR: Initializes the mock_error_data singleton
/// Verifies there are no pending verifications on end of the test
struct mock_error_fixture
{
    mock_error_fixture() { mock_error_data.reset(); }
    ~mock_error_fixture()
    {
        BOOST_CHECK(mock_error_data.verify());
        BOOST_CHECK_EQUAL(0, mock_error_data.call_count);
    }
};

/// Check that the number of calls to mocked function equals the given amount
/// and resets them (for the next check and cleanup)
#define CHECK_CALLS(calls)                                \
    BOOST_CHECK_EQUAL(calls, mock_error_data.call_count); \
    mock_error_data.call_count = 0;
/// Similar to BOOST_CHECK_THROW:
/// Checks that running `expr` leads to an error of the set expectations,
/// that the error message equals `error`, with the given `context`
/// and `calls` mocked functions were called.
#define CHECK_ERROR(expr, error, calls, context)              \
    BOOST_CHECK(mock_error_data.verify());                    \
    try                                                       \
    {                                                         \
        expr;                                                 \
    } catch(...)                                              \
    {}                                                        \
    BOOST_CHECK_EQUAL(1, mock_error_data.error_count);        \
    BOOST_CHECK_EQUAL(error, mock_error_data.last_message);   \
    BOOST_CHECK_EQUAL(context, mock_error_data.last_context); \
    CHECK_CALLS(calls);                                       \
    mock_error_data.reset();

#endif // MOCK_TEST_MOCK_ERROR_HPP_INCLUDED
