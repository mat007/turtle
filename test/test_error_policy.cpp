// http://turtle.sourceforge.net
//
// Copyright Alexander Grund 2026
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/mock.hpp>
#include <boost/test/output/compiler_log_formatter.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_SUITE(test_default_error_policy)

namespace bt = boost::unit_test;

class log_capture
{
    struct saved_state
    {
        bt::output_format fmt;
        std::ostream* stream = nullptr;
        bt::log_level level = bt::invalid_log_level;
    };

    std::ostringstream buf_;
    saved_state saved_[5] = { { bt::OF_CLF },
                              { bt::OF_XML },
                              { bt::OF_JUNIT },
                              { bt::OF_CUSTOM_LOGGER },
                              { bt::OF_DOT } };

public:
    explicit log_capture(bt::log_level lev = bt::log_successful_tests)
    {
        for(auto& s : saved_)
        {
            s.stream = bt::unit_test_log.get_stream(s.fmt);
            bt::unit_test_log.set_stream(s.fmt, buf_);
            s.level = bt::unit_test_log.set_threshold_level(s.fmt, lev);
        }
    }

    ~log_capture()
    {
        for(auto& s : saved_)
        {
            bt::unit_test_log.set_stream(s.fmt, *s.stream);
            bt::unit_test_log.set_threshold_level(s.fmt, s.level);
        }
    }

    log_capture(const log_capture&) = delete;
    log_capture& operator=(const log_capture&) = delete;

    operator std::string() const { return buf_.str(); }
};

BOOST_AUTO_TEST_CASE(calls_are_logged)
{
    std::string log_txt;
    {
        log_capture log_stream;
        MOCK_FUNCTOR(f, void(int));
        MOCK_EXPECT(f).once().with(42);
        f(42); // call() -> success log
        log_txt = log_stream;
    }

    BOOST_TEST(log_txt.find("mock expectation fulfilled") != std::string::npos);
    BOOST_TEST(log_txt.find("v once().with( 42 )") != std::string::npos);
}

// Fail on unfilfilled expectation
BOOST_AUTO_TEST_CASE(fail_on_unfullfilled, *boost::unit_test::expected_failures(1))
{
    log_capture log_stream;
    MOCK_FUNCTOR(f, void());
    MOCK_EXPECT(f).once(); // never made
    MOCK_VERIFY(f);        // -> 1 failed assertion
}

BOOST_AUTO_TEST_CASE(log_on_unfullfilled, *boost::unit_test::expected_failures(1))
{
    // Same as previous but ensure failure is found in log
    // Needs to be separate as otherwise it would be impossible to distinguish
    // which of MOCK_VERIFY or BOOST_TEST(log) failed if exactly one fails.
    std::string log_txt;
    {
        log_capture log_stream;
        MOCK_FUNCTOR(f, void());
        MOCK_EXPECT(f).once(); // never made
        MOCK_VERIFY(f);        // -> 1 failed assertion
        log_txt = log_stream;
    }
    BOOST_TEST(log_txt.find("test_error_policy.cpp(93)") != std::string::npos, "log was: " << log_txt);
    BOOST_TEST(log_txt.find("verification failed: f\n. once()") != std::string::npos, "log was: " << log_txt);
}

BOOST_AUTO_TEST_CASE(fail_on_unfullfilled_exit, *boost::unit_test::expected_failures(1))
{
    MOCK_FUNCTOR(f_unfullfilled, void());
    MOCK_EXPECT(f_unfullfilled).once(); // never made
    // -> Fail during exit
}

BOOST_AUTO_TEST_SUITE_END()
