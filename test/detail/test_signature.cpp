// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/mock.hpp>
#include <boost/test/unit_test.hpp>
#include <map>
#include <type_traits>

namespace {
struct base
{
    void method_1();
    float method_2(int);
    // Using templates in result and argument types
    std::map<int, float> method_3(std::map<int, int>, short);

    // Qualified methods
    float qual_1(int) &;
    float qual_2(int) &&;
    float qual_3(int) const;
    float qual_4(int) const&;
    float qual_5(int) const&&;
    float qual_6(int) volatile;
    float qual_7(int) volatile&;
    float qual_8(int) volatile&&;
    float qual_9(int) const volatile;
    float qual_10(int) const volatile&;
    float qual_11(int) const volatile&&;
};
} // namespace

BOOST_AUTO_TEST_CASE(signature_traits_return_correct_values)
{
    using function1 = void();
    static_assert(std::is_same<mock::detail::result_type_t<function1>, void>::value, "!");
    static_assert(mock::detail::function_arity_t<function1>::value == 0, "!");

    using function2 = float(int);
    static_assert(std::is_same<mock::detail::result_type_t<function2>, float>::value, "!");
    static_assert(mock::detail::function_arity_t<function2>::value == 1, "!");
    static_assert(std::is_same<mock::detail::parameter_t<function2, 0>, int>::value, "!");

    using function3 = unsigned(short&, int, const char*, float, double, char, unsigned char, std::map<int, char>);
    static_assert(std::is_same<mock::detail::result_type_t<function3>, unsigned>::value, "!");
    static_assert(mock::detail::function_arity_t<function3>::value == 8, "!");
    static_assert(std::is_same<mock::detail::parameter_t<function3, 0>, short&>::value, "!");
    static_assert(std::is_same<mock::detail::parameter_t<function3, 1>, int>::value, "!");
    static_assert(std::is_same<mock::detail::parameter_t<function3, 2>, const char*>::value, "!");
    static_assert(std::is_same<mock::detail::parameter_t<function3, 3>, float>::value, "!");
    static_assert(std::is_same<mock::detail::parameter_t<function3, 4>, double>::value, "!");
    static_assert(std::is_same<mock::detail::parameter_t<function3, 5>, char>::value, "!");
    static_assert(std::is_same<mock::detail::parameter_t<function3, 6>, unsigned char>::value, "!");
    static_assert(std::is_same<mock::detail::parameter_t<function3, 7>, std::map<int, char>>::value, "!");
}

BOOST_AUTO_TEST_CASE(MOCK_SIGNATURE_generates_signature)
{
    using base_type = base; // MOCK_SIGNATURE requires a visible base_type typedef in the current scope
    static_assert(std::is_same<void(), MOCK_SIGNATURE(method_1)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(method_2)>::value, "!");
    static_assert(std::is_same<std::map<int, float>(std::map<int, int>, short), MOCK_SIGNATURE(method_3)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_1)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_2)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_3)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_4)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_5)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_6)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_7)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_8)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_9)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_10)>::value, "!");
    static_assert(std::is_same<float(int), MOCK_SIGNATURE(qual_11)>::value, "!");
}

BOOST_AUTO_TEST_CASE(MOCK_PROTECT_SIGNATURE_keeps_signature)
{
    // MOCK_PROTECT_SIGNATURE is basically a no-op regarding its argument
    // and only required to get it through a VAR_ARGS macro
    // clang-format off
    static_assert(std::is_same<void(), MOCK_PROTECT_SIGNATURE(
                               void())>::value, "!");
    static_assert(std::is_same<float*(int*), MOCK_PROTECT_SIGNATURE(
                               float*(int*))>::value, "!");
    static_assert(std::is_same<std::map<int, float>(std::map<int, int>, short), MOCK_PROTECT_SIGNATURE(
                               std::map<int, float>(std::map<int, int>, short))>::value, "!");
    // clang-format on
}
