// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
// Copyright Alexander Grund 2026
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/function.hpp>
#include <turtle/detail/void_t.hpp>
#include <boost/test/unit_test.hpp>
#include <functional>
#include <type_traits>

namespace {
static_assert(std::is_same<void, decltype(mock::detail::function<void()>{}())>::value, "!");
static_assert(std::is_same<int, decltype(mock::detail::function<int()>{}())>::value, "!");
static_assert(std::is_same<void, decltype(mock::detail::function<void(float)>{}(std::declval<float>()))>::value, "!");
static_assert(std::is_same<int, decltype(mock::detail::function<int(float)>{}(std::declval<float>()))>::value, "!");

template<class TFunction, typename... TParams>
using with_call_t = decltype(std::declval<TFunction>().expect().with(std::declval<TParams>()...));
template<class AlwaysVoid, class TFunction, typename... TParams>
struct with_is_callable : std::false_type
{};
template<class TFunction, class... TParams>
struct with_is_callable<mock::detail::void_t<with_call_t<TFunction, TParams...>>, TFunction, TParams...> :
    std::true_type
{};
// Detect if we can call function.expect().with(params)
template<class TFunction, class... TParams>
using with_is_callable_t = with_is_callable<void, TFunction, TParams...>;

// For function with no parameters `with` can never be called
using function_0_args = mock::detail::function<int()>;
static_assert(!with_is_callable_t<function_0_args>::value, "Can't call with()");
static_assert(!with_is_callable_t<function_0_args, int>::value, "Can't call with(int)");
static_assert(!with_is_callable_t<function_0_args, bool()>::value, "Can't call with custom constraint");

// For functions with parameters `with` can be called with a custom constraint (taking all params)
// or exactly as many parameters as the function has (here 1 and 4 respectively)
using function_1_args = mock::detail::function<int(int)>;
static_assert(!with_is_callable_t<function_1_args>::value, "Can't call with()");
static_assert(with_is_callable_t<function_1_args, int>::value, "Can call with(int)");
static_assert(with_is_callable_t<function_1_args, bool(int)>::value, "Can call with custom constraint");
static_assert(!with_is_callable_t<function_1_args, int, int>::value, "Can't call with(int, int)");

using function_4_args = mock::detail::function<int(int, int, int, int)>;
static_assert(!with_is_callable_t<function_4_args>::value, "Can't call with()");
static_assert(with_is_callable_t<function_4_args, bool(int, int, int, int)>::value, "Can call with custom constraint");
static_assert(!with_is_callable_t<function_4_args, int, int>::value, "Can't call with(int, int)");
static_assert(!with_is_callable_t<function_4_args, int, int, int>::value, "Can't call with(int, int, int)");
static_assert(with_is_callable_t<function_4_args, int, int, int, int>::value, "Can call with(int, int, int, int)");
static_assert(!with_is_callable_t<function_4_args, int, int, int, int, int>::value,
              "Can't call with(int, int, int, int, int)");
} // namespace

BOOST_AUTO_TEST_CASE(std_function_compat)
{
    mock::detail::function<void()> f;

    // mock function can be used as std::function
    {
        std::function<void()> functor = f;
    }

    // mock function can be used with std::ref and std::bind
    {
        std::function<void()> functor = std::bind(std::ref(f));
    }
}
