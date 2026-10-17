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
#ifdef MOCK_THREAD_SAFE
#    include <boost/thread.hpp>
#endif
#include <memory>

BOOST_FIXTURE_TEST_SUITE(mock_function_invocation, mock_error_fixture)

BOOST_AUTO_TEST_CASE(triggering_an_unconfigured_function_calls_unexpected_call_error)
{
    {
        mock::detail::function<void()> f;
        CHECK_ERROR(f(), "unexpected call", 0, "?()");
    }
    {
        mock::detail::function<int(int, const std::string&)> f;
        CHECK_ERROR(f(1, "s"), "unexpected call", 0, "?( 1, \"s\" )");
    }
}

BOOST_AUTO_TEST_CASE(triggering_a_never_expectation_calls_unexpected_call_error)
{
    {
        mock::detail::function<void()> f;
        f.expect().never();
        CHECK_ERROR(f(), "unexpected call", 0, "?()\nv never()");
    }
    {
        mock::detail::function<int(int, const std::string&)> f;
        f.expect().never();
        CHECK_ERROR(f(1, "s"), "unexpected call", 0, "?( 1, \"s\" )\nv never().with( any, any )");
    }
}

BOOST_AUTO_TEST_CASE(triggering_an_unlimited_expectation_is_valid)
{
    {
        mock::detail::function<void()> f;
        f.expect();
        f();
        f();
        CHECK_CALLS(2);
    }
    {
        mock::detail::function<void(int, std::string)> f;
        f.expect();
        f(1, "s1");
        f(2, "s2");
        CHECK_CALLS(2);
    }
    {
        mock::detail::function<void(int, const std::string&)> f;
        f.expect();
        f(1, "s1");
        f(2, "s2");
        CHECK_CALLS(2);
    }
}

BOOST_AUTO_TEST_CASE(triggering_several_once_expectations_is_valid)
{
    {
        mock::detail::function<void()> f;
        f.expect().once();
        f.expect().once();
        f();
        f();
        CHECK_CALLS(2);
    }
    {
        mock::detail::function<void(int, std::string)> f;
        f.expect().once().with(1, "s1");
        f.expect().once().with(2, "s2");
        f(1, "s1");
        f(2, "s2");
        CHECK_CALLS(2);
    }
    {
        mock::detail::function<void(int, const std::string&)> f;
        f.expect().once().with(1, "s1");
        f.expect().once().with(2, "s2");
        f(1, "s1");
        f(2, "s2");
        CHECK_CALLS(2);
    }
}

BOOST_AUTO_TEST_CASE(string_likes_are_matched)
{
    const char* c_string = "value";
    const char c_string2[] = "value";
    BOOST_REQUIRE(c_string != c_string2); // Different pointers
    const std::string string = c_string;
    {
        mock::detail::function<void(std::string)> f;

        f.expect().once().with(string);
        f(string);
        BOOST_TEST(f.verify());

        f.reset();
        f.expect().once().with(c_string);
        f(string);
        BOOST_TEST(f.verify());

        f.reset();
        f.expect().once().with(c_string2);
        f(string);
        BOOST_TEST(f.verify());

        CHECK_CALLS(3);
    }
    {
        mock::detail::function<void(const char*)> f;

        f.expect().once().with(c_string);
        f(c_string);
        BOOST_TEST(f.verify());

        f.reset();
        f.expect().once().with(c_string);
        f(c_string2);
        BOOST_TEST(f.verify());

        f.reset();
        f.expect().once().with(c_string2);
        f(c_string);
        BOOST_TEST(f.verify());

        f.reset();
        f.expect().once().with(string);
        f(c_string);
        BOOST_TEST(f.verify());
        CHECK_CALLS(4);
    }
}

BOOST_AUTO_TEST_CASE(triggering_a_once_expectation_calls_unexpected_call_error_after_one_call)
{
    {
        mock::detail::function<void()> f;
        f.expect().once();
        f();
        CHECK_ERROR(f(), "unexpected call", 1, "?()\nv once()");
    }
    {
        mock::detail::function<void(int, const std::string&)> f;
        f.expect().once();
        f(1, "s");
        CHECK_ERROR(f(1, "s"), "unexpected call", 1, "?( 1, \"s\" )\nv once().with( any, any )");
    }
}

BOOST_AUTO_TEST_CASE(literal_zero_can_be_used_in_function_call_as_pointers)
{
    mock::detail::function<void(int*)> f;
    f.expect().once();
    f(0);
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(verify_expections_on_function, mock_error_fixture)

BOOST_AUTO_TEST_CASE(verifying_an_unconfigured_function_succeeds)
{
    {
        mock::detail::function<void()> f;
        BOOST_TEST(f.verify());
    }
    {
        mock::detail::function<int(int, const std::string&)> f;
        BOOST_TEST(f.verify());
    }
}

BOOST_AUTO_TEST_CASE(verifying_an_unlimited_expectation_succeeds)
{
    {
        mock::detail::function<void()> f;
        f.expect();
        BOOST_TEST(f.verify());
        CHECK_CALLS(0);
    }
    {
        mock::detail::function<int(int, const std::string&)> f;
        f.expect();
        BOOST_TEST(f.verify());
        CHECK_CALLS(0);
    }
}

BOOST_AUTO_TEST_CASE(verifying_a_once_expectation_after_one_call_succeeds)
{
    {
        mock::detail::function<void()> f;
        f.expect().once();
        f();
        BOOST_TEST(f.verify());
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<void(int, const std::string&)> f;
        f.expect().once();
        f(1, "s");
        BOOST_TEST(f.verify());
        CHECK_CALLS(1);
    }
}

BOOST_AUTO_TEST_CASE(verifying_a_once_expectation_before_the_call_fails)
{
    {
        mock::detail::function<void()> f;
        f.expect().once();
        CHECK_ERROR(BOOST_TEST(!f.verify()), "verification failed", 0, "?\n. once()");
    }
    {
        mock::detail::function<int(int, const std::string&)> f;
        f.expect().once();
        CHECK_ERROR(BOOST_TEST(!f.verify()), "verification failed", 0, "?\n. once().with( any, any )");
    }
}

BOOST_AUTO_TEST_CASE(verifying_a_once_expectation_after_a_verify_and_one_call_succeeds)
{
    mock::detail::function<void()> f;
    f.expect().once();
    CHECK_ERROR(BOOST_TEST(!f.verify()), "verification failed", 0, "?\n. once()");
    f();
    BOOST_TEST(f.verify());
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_CASE(triggering_a_reset_function_calls_unexpected_call_error)
{
    {
        mock::detail::function<void()> f;
        f.expect();
        f.reset();
        CHECK_ERROR(f(), "unexpected call", 0, "?()");
    }
    {
        mock::detail::function<int(int, const std::string&)> f;
        f.expect();
        f.reset();
        CHECK_ERROR(f(1, "s"), "unexpected call", 0, "?( 1, \"s\" )");
    }
}

BOOST_AUTO_TEST_CASE(verifying_a_reset_function_succeeds)
{
    {
        mock::detail::function<void()> f;
        f.expect();
        f.reset();
        BOOST_TEST(f.verify());
    }
    {
        mock::detail::function<int(int, const std::string&)> f;
        f.expect();
        f.reset();
        BOOST_TEST(f.verify());
    }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(invocation_constraints, mock_error_fixture)

BOOST_AUTO_TEST_CASE(
  triggering_an_expectation_with_wrong_parameter_value_in_equal_constraint_calls_unexpected_call_error)
{
    {
        mock::detail::function<void(int)> f;
        f.expect().with(42);
        CHECK_ERROR(f(43), "unexpected call", 0, "?( 43 )\n. unlimited().with( 42 )");
    }
    {
        mock::detail::function<int(int, const std::string&)> f;
        f.expect().with(42, "expected");
        CHECK_ERROR(
          f(42, "actual"), "unexpected call", 0, "?( 42, \"actual\" )\n. unlimited().with( 42, \"expected\" )");
    }
}

BOOST_AUTO_TEST_CASE(
  triggering_an_expectation_with_wrong_parameter_value_in_equal_or_less_constraint_calls_unexpected_call_error)
{
    mock::detail::function<void(int)> f;
    f.expect().with(mock::equal(42) || mock::less(42));
    f(41);
    f(42);
    CHECK_ERROR(f(43), "unexpected call", 2, "?( 43 )\n. unlimited().with( ( equal( 42 ) || less( 42 ) ) )");
}

BOOST_AUTO_TEST_CASE(
  triggering_an_expectation_with_wrong_parameter_value_in_equal_and_not_less_constraint_calls_unexpected_call_error)
{
    mock::detail::function<void(int)> f;
    f.expect().with(mock::equal(42) && !mock::less(41));
    f(42);
    CHECK_ERROR(f(43), "unexpected call", 1, "?( 43 )\n. unlimited().with( ( equal( 42 ) && ! less( 41 ) ) )");
}

namespace {
class my_interface
{
public:
    my_interface() = default;
    my_interface(const my_interface&) = delete;
    my_interface& operator=(const my_interface&) = delete;
    virtual ~my_interface() = default;

private:
    virtual void my_method() = 0;
};
class my_implementation : public my_interface
{
    virtual void my_method() override {}
};
} // namespace

BOOST_AUTO_TEST_CASE(passing_call_values_by_reference_is_transparent)
{
    {
        mock::detail::function<void(my_interface&)> f;
        my_implementation imp;
        f.expect().with(mock::same(imp));
        f(imp);
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<void(const my_interface&)> f;
        my_implementation imp;
        f.expect().with(mock::same(imp));
        f(imp);
        CHECK_CALLS(1);
    }
}

namespace {
bool custom_constraint(int)
{
    return false;
}
} // namespace

BOOST_AUTO_TEST_CASE(triggering_an_expectation_with_failing_custom_constraint_calls_unexpected_call_error)
{
    {
        mock::detail::function<void(int)> f;
        f.expect().with(&custom_constraint);
        CHECK_ERROR(f(42), "unexpected call", 0, "?( 42 )\n. unlimited().with( ? )");
    }
    {
        mock::detail::function<int(int, const std::string&)> f;
        f.expect().with(&custom_constraint, "actual");
        CHECK_ERROR(f(42, "actual"), "unexpected call", 0, "?( 42, \"actual\" )\n. unlimited().with( ?, \"actual\" )");
    }
}

// BOOST_AUTO_TEST_CASE( literal_zero_can_be_used_in_place_of_null_pointers_in_constraints )
//{
//   mock::detail::function< void( int* ) > f;
//   f.expect().with( 0 );
//   f.reset();
//   CHECK_CALLS( 1 );
//}

BOOST_AUTO_TEST_CASE(nullptr_can_be_used_in_place_of_null_pointers_in_constraints)
{
    mock::detail::function<void(int*)> f;
    f.expect().with(nullptr);
    f(0);
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_CASE(unique_ptr_is_supported_as_parameter)
{
    mock::detail::function<void(std::unique_ptr<int>)> f;
    f.expect();
    std::unique_ptr<int> p(new int);
    f(std::move(p));
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_SUITE_END()

#ifdef MOCK_THREAD_SAFE
namespace {
void iterate(mock::detail::function<int()>& f)
{
    f.expect().once().returns(0);
    try
    {
        f();
    } catch(...)
    {}
}
} // namespace

BOOST_FIXTURE_TEST_CASE(function_is_thread_safe, mock_error_fixture)
{
    mock::detail::function<int()> f;
    boost::thread_group group;
    for(int i = 0; i < 100; ++i)
        group.create_thread([&f]() { iterate(f); });
    group.join_all();
    CHECK_CALLS(100);
}

#endif // MOCK_THREAD_SAFE
