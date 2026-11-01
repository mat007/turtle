// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
// Copyright Alexander Grund 2026
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "../mock_error.hpp"
#include "../undefined.hpp"
#include <turtle/constraints.hpp>
#include <turtle/detail/function.hpp>
#include <boost/test/unit_test.hpp>
#include <memory>

BOOST_FIXTURE_TEST_SUITE(return_actions, mock_error_fixture)

BOOST_AUTO_TEST_CASE(triggering_an_expectation_with_no_return_set_calls_missing_action)
{
    {
        mock::detail::function<int()> f;
        f.expect();
        CHECK_ERROR(f(), "missing action", 0, "?()\n. unlimited()");
    }
    {
        mock::detail::function<int&()> f;
        f.expect();
        CHECK_ERROR(f(), "missing action", 0, "?()\n. unlimited()");
    }
    {
        mock::detail::function<const std::string&()> f;
        f.expect();
        CHECK_ERROR(f(), "missing action", 0, "?()\n. unlimited()");
    }
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_returns_the_set_value)
{
    {
        mock::detail::function<int()> f;
        f.expect().returns(42);
        BOOST_TEST(f() == 42);
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<int()> f;
        const int i = 42;
        f.expect().returns(i);
        BOOST_TEST(f() == i);
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<int()> f;
        int i = 42;
        f.expect().returns(std::ref(i));
        i = 43;
        BOOST_TEST(f() == 43);
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<int&()> f;
        f.expect().returns(42);
        BOOST_TEST(f() == 42);
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<int&()> f;
        const int result = 42;
        f.expect().returns(result);
        BOOST_TEST(f() == result);
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<int&()> f;
        int i = 42;
        f.expect().returns(i);
        i = 43;
        BOOST_TEST(f() == 42);
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<int&()> f;
        int i = 42;
        f.expect().returns(std::ref(i));
        i = 43;
        BOOST_TEST(f() == 43);
        BOOST_TEST((f() = 12) == 12);
        BOOST_TEST(i == 12);
        CHECK_CALLS(2);
    }
    {
        mock::detail::function<std::string()> f;
        f.expect().returns("result");
        BOOST_TEST(f() == "result");
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<const std::string&()> f;
        f.expect().returns("result");
        BOOST_TEST(f() == "result");
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<int*()> f;
        f.expect().returns(0);
        BOOST_TEST(!f());
        CHECK_CALLS(1);
    }
#ifdef BOOST_MSVC // this produces a warning with gcc
    {
        mock::detail::function<int* const()> f;
        int i = 0;
        f.expect().returns(&i);
        BOOST_TEST(f() == &i);
        CHECK_CALLS(1);
    }
#endif
    {
        mock::detail::function<int()> f;
        f.expect().returns(0);
        BOOST_TEST(f() == 0);
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<int&()> f;
        f.expect().returns(0);
        BOOST_TEST(f() == 0);
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<unsigned int()> f;
        f.expect().returns(0);
        BOOST_TEST(f() == 0u);
        CHECK_CALLS(1);
    }
}

namespace {
struct base
{
    virtual ~base() = default;
    virtual void f() = 0;
};
struct derived : base
{
    virtual void f() override {}
};
} // namespace

BOOST_AUTO_TEST_CASE(triggering_an_expectation_moves_the_set_lvalue)
{
    mock::detail::function<int()> f;
    int i = 3;
    f.expect().moves(i);
    BOOST_CHECK_NO_THROW(f());
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_moves_the_set_const_lvalue)
{
    mock::detail::function<int()> f;
    const int i = 3;
    f.expect().moves(i);
    BOOST_CHECK_NO_THROW(f());
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_moves_the_set_rvalue)
{
    mock::detail::function<int()> f;
    f.expect().moves(3);
    BOOST_CHECK_NO_THROW(f());
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_moves_the_set_unique_ptr_lvalue)
{
    mock::detail::function<std::unique_ptr<int>()> f;
    std::unique_ptr<int> p(new int);
    f.expect().moves(std::move(p));
    BOOST_CHECK_NO_THROW(f());
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_moves_the_set_unique_ptr_rvalue)
{
    {
        mock::detail::function<std::unique_ptr<int>()> f;
        f.expect().moves(std::unique_ptr<int>(new int));
        BOOST_CHECK_NO_THROW(f());
        CHECK_CALLS(1);
    }
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_returns_the_set_shared_ptr_value)
{
    {
        mock::detail::function<std::shared_ptr<base>()> f;
        f.expect().returns(new derived);
        BOOST_CHECK_NO_THROW(f());
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<const std::shared_ptr<base>&()> f;
        f.expect().returns(new derived);
        BOOST_CHECK_NO_THROW(f());
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<std::shared_ptr<base>&()> f;
        f.expect().returns(new derived);
        BOOST_CHECK_NO_THROW(f());
        CHECK_CALLS(1);
    }
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_returns_by_reference)
{
    {
        mock::detail::function<base&()> f;
        derived b;
        f.expect().returns(std::ref(b));
        BOOST_CHECK_NO_THROW(f());
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<base&()> f;
        derived b;
        f.expect().returns(b);
        BOOST_CHECK_NO_THROW(f());
        CHECK_CALLS(1);
    }
    {
        mock::detail::function<undefined&()> f;
        f.expect().returns(std::ref(get_undefined()));
        f.reset();
    }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(call_actions, mock_error_fixture)

namespace {
int custom_result()
{
    return 42;
}
int custom_result_with_parameter(int i)
{
    return i;
}
} // namespace

BOOST_AUTO_TEST_CASE(triggering_an_expectation_calls_the_custom_functor)
{
    mock::detail::function<int()> f;
    f.expect().calls(&custom_result);
    BOOST_TEST(f() == 42);
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_calls_the_custom_functor_with_parameters)
{
    mock::detail::function<int(int)> f;
    f.expect().calls(&custom_result_with_parameter);
    BOOST_TEST(f(42) == 42);
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_calls_the_custom_functor_without_parameters_thanks_to_std_bind)
{
    mock::detail::function<int(int)> f;
    f.expect().calls(std::bind(&custom_result));
    BOOST_TEST(f(17) == 42);
    CHECK_CALLS(1);
}

BOOST_AUTO_TEST_CASE(triggering_an_expectation_throws_the_set_exception)
{
    mock::detail::function<int()> f;
    f.expect().throws(std::runtime_error("some exception"));
    try
    {
        f();
    } catch(std::runtime_error& f)
    {
        BOOST_TEST(f.what() == "some exception");
        CHECK_CALLS(1);
        return;
    }
    BOOST_FAIL("should have thrown");
}

// multiple expectations

BOOST_AUTO_TEST_CASE(expecting_twice_a_single_expectation_makes_it_callable_twice)
{
    {
        mock::detail::function<void()> f;
        f.expect().once();
        f.expect().once();
        f();
        f();
        CHECK_ERROR(f(), "unexpected call", 2, "?()\nv once()\nv once()");
    }
    {
        mock::detail::function<void(const std::string&)> f;
        f.expect().once().with("first");
        f.expect().once().with("second");
        f("first");
        f("second");
        CHECK_ERROR(
          f("third"), "unexpected call", 2, "?( \"third\" )\nv once().with( \"first\" )\nv once().with( \"second\" )");
    }
}

BOOST_AUTO_TEST_CASE(best_expectation_is_selected_first)
{
    {
        mock::detail::function<void(int)> f;
        f.expect().once().with(1);
        f.expect().once().with(2);
        f(2);
        f(1);
        CHECK_ERROR(f(3), "unexpected call", 2, "?( 3 )\nv once().with( 1 )\nv once().with( 2 )");
    }
    {
        mock::detail::function<void(const std::string&)> f;
        f.expect().once().with("first");
        f.expect().once().with("second");
        f("second");
        f("first");
        CHECK_ERROR(
          f("third"), "unexpected call", 2, "?( \"third\" )\nv once().with( \"first\" )\nv once().with( \"second\" )");
    }
}

BOOST_AUTO_TEST_SUITE_END()
