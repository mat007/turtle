// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
// Copyright Alexander Grund 2026
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "../mock_error.hpp"
#include <turtle/constraints.hpp>
#include <turtle/detail/function.hpp>
#include <boost/test/unit_test.hpp>
#include <memory>

namespace {
template<typename T>
std::string to_string(const T& t)
{
    std::stringstream s;
    s << t;
    return s.str();
}
} // namespace

BOOST_FIXTURE_TEST_SUITE(expectation_error_reporting, mock_error_fixture)

namespace {
bool custom_constraint(int)
{
    return false;
}
} // namespace

BOOST_AUTO_TEST_CASE(expectation_can_be_serialized_to_be_human_readable)
{
    {
        mock::detail::function<void(float)> f;
        f.expect().once().with(1);
        f.expect().once().with(mock::close(2, 5));
        float target = 0;
        f.expect().once().with(mock::retrieve(target));
        f.expect().once().with(mock::same(target));
        BOOST_CHECK_NO_THROW(f(2.1f));
        std::string expected = "?\n";                   // Not in a current call context
        expected += ". once().with( 1 )\n";             // Unmet/Unverified expectation with value
        expected += "v once().with( close( 2, 5 ) )\n"; // Verified expectation with constraint
        target = 42;
        // (Unverified) expectation with retrieve/same constraint print current value
        expected += ". once().with( retrieve( 42 ) )\n";
        expected += ". once().with( same( 42 ) )";
        BOOST_CHECK_EQUAL(expected, to_string(f));
        CHECK_CALLS(1);
        f.reset();
    }
    {
        mock::detail::function<void(int)> f;
        f.expect().never().with(1);
        const std::string expected = "?\n"
                                     "v never().with( 1 )";
        BOOST_CHECK_EQUAL(expected, to_string(f));
        f.reset();
    }
    {
        mock::detail::function<void(const std::string&)> f;
        f.expect().never().with(mock::less("first"));
        f.expect().exactly(2).with("second");
        BOOST_CHECK_NO_THROW(f("second"));
        {
            const std::string expected = "?\n"
                                         "v never().with( less( \"first\" ) )\n"
                                         ". exactly( 1/2 ).with( \"second\" )";
            BOOST_CHECK_EQUAL(expected, to_string(f));
        }
        BOOST_CHECK_NO_THROW(f("second"));
        {
            const std::string expected = "?\n"
                                         "v never().with( less( \"first\" ) )\n"
                                         "v exactly( 2/2 ).with( \"second\" )";
            BOOST_CHECK_EQUAL(expected, to_string(f));
            CHECK_CALLS(2);
        }
        f.reset();
    }
    {
        mock::detail::function<void(int)> f;
        f.expect().once();
        const std::string expected = "?\n"
                                     ". once().with( any )";
        BOOST_CHECK_EQUAL(expected, to_string(f));
        f.reset();
    }
    {
        mock::detail::function<void(int)> f;
        f.expect().once().with(mock::any);
        const std::string expected = "?\n"
                                     ". once().with( any )";
        BOOST_CHECK_EQUAL(expected, to_string(f));
        f.reset();
    }
    {
        mock::detail::function<void(int)> f;
        f.expect().once();
        const std::string expected = "?\n"
                                     ". once().with( any )";
        BOOST_CHECK_EQUAL(expected, to_string(f));
        f.reset();
    }
    {
        mock::detail::function<void(int)> f;
        f.expect().once().with(&custom_constraint);
        const std::string expected = "?\n"
                                     ". once().with( ? )";
        BOOST_CHECK_EQUAL(expected, to_string(f));
        f.reset();
    }
}

BOOST_AUTO_TEST_CASE(expectation_with_remaining_untriggered_matches_upon_destruction_calls_untriggered_expectation)
{
    auto f = std::make_unique<mock::detail::function<void()>>();
    f->expect().once();
    CHECK_ERROR(f.reset(), "untriggered expectation", 0, "?\n. once()");
}

BOOST_AUTO_TEST_CASE(verifying_expectation_with_remaining_matches_disables_the_automatic_verification_upon_destruction)
{
    mock::detail::function<void()> f;
    f.expect().once();
    CHECK_ERROR(f.verify(), "verification failed", 0, "?\n. once()");
}

BOOST_AUTO_TEST_CASE(triggering_unexpected_call_call_disables_the_automatic_verification_upon_destruction)
{
    mock::detail::function<void()> f;
    CHECK_ERROR(f(), "unexpected call", 0, "?()");
}

BOOST_AUTO_TEST_CASE(adding_an_expectation_reactivates_the_verification_upon_destruction)
{
    auto f = std::make_unique<mock::detail::function<void()>>();
    CHECK_ERROR((*f)(), "unexpected call", 0, "?()");
    f->expect().once();
    CHECK_ERROR(f.reset(), "untriggered expectation", 0, "?\n. once()");
}

BOOST_AUTO_TEST_CASE(throwing_an_exception_disables_the_automatic_verification_upon_destruction)
{
    try
    {
        mock::detail::function<void()> f;
        f.expect().once();
        throw std::exception();
    } catch(std::exception&)
    {}
}

BOOST_AUTO_TEST_CASE(adding_file_and_line_number_information)
{
    mock::detail::function<void()> f;
    f.expect("file name", 42).once();
    CHECK_ERROR(f.verify(), "verification failed", 0, "?\n. once()");
    BOOST_CHECK_EQUAL("file name", mock_error_data.last_file);
    BOOST_CHECK_EQUAL(42, mock_error_data.last_line);
}

BOOST_AUTO_TEST_SUITE_END()
