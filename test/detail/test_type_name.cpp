// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/type_name.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>

namespace {
template<typename T>
std::string to_string(const T& t)
{
    std::ostringstream s;
    s << mock::detail::make_type_name(t);
    return s.str();
}
} // namespace

BOOST_AUTO_TEST_CASE(name_of_base_type_is_extracted)
{
    BOOST_TEST(to_string('a') == "char");
    BOOST_TEST(to_string(true) == "bool");
    BOOST_TEST(to_string<int>(0) == "int");
    BOOST_TEST(to_string<short>(0) == "short");
    BOOST_TEST(to_string<long>(0) == "long");
    BOOST_TEST(to_string<unsigned int>(0) == "unsigned int");
    BOOST_TEST(to_string<unsigned short>(0) == "unsigned short");
    BOOST_TEST(to_string<unsigned long>(0) == "unsigned long");
}

struct my_type_in_default_namespace
{
    struct inner
    {};
};

BOOST_AUTO_TEST_CASE(name_of_type_in_default_namespace_is_extracted)
{
    BOOST_TEST(to_string(my_type_in_default_namespace()) == "my_type_in_default_namespace");
}

BOOST_AUTO_TEST_CASE(name_of_inner_type_from_type_in_default_namespace_is_extracted)
{
    BOOST_TEST(to_string(my_type_in_default_namespace::inner()) == "inner");
}

template<typename T>
struct my_template_type_in_default_namespace
{
    struct inner
    {};
};

BOOST_AUTO_TEST_CASE(name_of_template_type_in_default_namespace_is_extracted)
{
    BOOST_TEST(to_string(my_template_type_in_default_namespace<int>()) == "my_template_type_in_default_namespace<int>");
}

BOOST_AUTO_TEST_CASE(name_of_inner_type_from_template_type_in_default_namespace_is_extracted)
{
    BOOST_TEST(to_string(my_template_type_in_default_namespace<int>::inner()) == "inner");
}

namespace {
struct my_type_in_anonymous_namespace
{};
} // namespace

BOOST_AUTO_TEST_CASE(name_of_type_in_anonymous_namespace_is_extracted)
{
    BOOST_TEST(to_string(my_type_in_anonymous_namespace()) == "my_type_in_anonymous_namespace");
}

namespace nm {
struct my_type_from_named_namespace
{};
} // namespace nm

BOOST_AUTO_TEST_CASE(name_of_type_from_named_namespace_is_extracted)
{
    BOOST_TEST(to_string(nm::my_type_from_named_namespace()) == "my_type_from_named_namespace");
}

namespace nm { namespace inner {
    struct my_type_in_named_inner_namespace
    {};
}} // namespace nm::inner

BOOST_AUTO_TEST_CASE(name_of_type_in_named_inner_namespace_is_extracted)
{
    BOOST_TEST(to_string(nm::inner::my_type_in_named_inner_namespace()) == "my_type_in_named_inner_namespace");
}

namespace { namespace inner {
    struct my_type_in_unnamed_inner_namespace
    {};
}} // namespace ::inner

BOOST_AUTO_TEST_CASE(name_of_type_in_unnamed_inner_namespace_is_extracted)
{
    BOOST_TEST(to_string(inner::my_type_in_unnamed_inner_namespace()) == "my_type_in_unnamed_inner_namespace");
}

BOOST_AUTO_TEST_CASE(name_of_local_type_is_extracted)
{
    struct my_local_type
    {};
    BOOST_TEST(to_string(my_local_type()) == "my_local_type");
}

namespace {
template<typename T>
struct my_template_type
{
    struct inner
    {};
};
} // namespace

BOOST_AUTO_TEST_CASE(name_of_template_type_in_anonymous_namespace_is_extracted)
{
    BOOST_TEST(to_string(my_template_type<int>()) == "my_template_type<int>");
    BOOST_TEST(to_string(my_template_type<std::exception>()) == "my_template_type<exception>");
    BOOST_TEST(to_string(my_template_type<int const&>()) == "my_template_type<int const&>");
    BOOST_TEST(to_string(my_template_type<std::exception const&>()) == "my_template_type<exception const&>");
    BOOST_TEST(to_string(my_template_type<int const*>()) == "my_template_type<int const*>");
    BOOST_TEST(to_string(my_template_type<std::exception const*>()) == "my_template_type<exception const*>");
    BOOST_TEST(to_string(my_template_type<int const*&>()) == "my_template_type<int const*&>");
    BOOST_TEST(to_string(my_template_type<std::exception const*&>()) == "my_template_type<exception const*&>");
}

BOOST_AUTO_TEST_CASE(name_of_inner_type_from_template_type_in_anonymous_namespace_is_extracted)
{
    BOOST_TEST(to_string(my_template_type<int>::inner()) == "inner");
    BOOST_TEST(to_string(my_template_type<std::exception>::inner()) == "inner");
    BOOST_TEST(to_string(my_template_type<int const&>::inner()) == "inner");
    BOOST_TEST(to_string(my_template_type<std::exception const&>::inner()) == "inner");
    BOOST_TEST(to_string(my_template_type<int const*>::inner()) == "inner");
    BOOST_TEST(to_string(my_template_type<std::exception const*>::inner()) == "inner");
    BOOST_TEST(to_string(my_template_type<int const*&>::inner()) == "inner");
    BOOST_TEST(to_string(my_template_type<std::exception const*&>::inner()) == "inner");
}

namespace nm {
template<typename T>
struct my_template_type
{
    struct inner
    {};
};
} // namespace nm

BOOST_AUTO_TEST_CASE(name_of_template_type_in_named_namespace_is_extracted)
{
    BOOST_TEST(to_string(nm::my_template_type<int>()) == "my_template_type<int>");
    BOOST_TEST(to_string(nm::my_template_type<std::exception>()) == "my_template_type<exception>");
    BOOST_TEST(to_string(nm::my_template_type<int const&>()) == "my_template_type<int const&>");
    BOOST_TEST(to_string(nm::my_template_type<std::exception const&>()) == "my_template_type<exception const&>");
    BOOST_TEST(to_string(nm::my_template_type<int const*>()) == "my_template_type<int const*>");
    BOOST_TEST(to_string(nm::my_template_type<std::exception const*>()) == "my_template_type<exception const*>");
    BOOST_TEST(to_string(nm::my_template_type<int const*&>()) == "my_template_type<int const*&>");
    BOOST_TEST(to_string(nm::my_template_type<std::exception const*&>()) == "my_template_type<exception const*&>");
}

BOOST_AUTO_TEST_CASE(name_of_inner_type_from_template_type_in_named_namespace_is_extracted)
{
    BOOST_TEST(to_string(nm::my_template_type<int>::inner()) == "inner");
    BOOST_TEST(to_string(nm::my_template_type<std::exception>::inner()) == "inner");
    BOOST_TEST(to_string(nm::my_template_type<int const&>::inner()) == "inner");
    BOOST_TEST(to_string(nm::my_template_type<std::exception const&>::inner()) == "inner");
    BOOST_TEST(to_string(nm::my_template_type<int const*>::inner()) == "inner");
    BOOST_TEST(to_string(nm::my_template_type<std::exception const*>::inner()) == "inner");
    BOOST_TEST(to_string(nm::my_template_type<int const*&>::inner()) == "inner");
    BOOST_TEST(to_string(nm::my_template_type<std::exception const*&>::inner()) == "inner");
}

namespace nm2 {
template<typename T>
struct my_template_type
{
    template<typename U>
    struct inner
    {};
};
} // namespace nm2

BOOST_AUTO_TEST_CASE(name_of_template_inner_type_from_template_type_in_named_namespace_is_extracted)
{
    BOOST_TEST(to_string(nm2::my_template_type<int>::inner<int>()) == "inner<int>");
    BOOST_TEST(to_string(nm2::my_template_type<std::exception>::inner<int>()) == "inner<int>");

    BOOST_TEST(to_string(nm2::my_template_type<int const&>::inner<int>()) == "inner<int>");
    BOOST_TEST(to_string(nm2::my_template_type<std::exception const&>::inner<int>()) == "inner<int>");
    BOOST_TEST(to_string(nm2::my_template_type<int const*>::inner<int>()) == "inner<int>");
    BOOST_TEST(to_string(nm2::my_template_type<std::exception const*>::inner<int>()) == "inner<int>");
    BOOST_TEST(to_string(nm2::my_template_type<int const*&>::inner<int>()) == "inner<int>");
    BOOST_TEST(to_string(nm2::my_template_type<std::exception const*&>::inner<int>()) == "inner<int>");

    BOOST_TEST(to_string(nm2::my_template_type<int>::inner<int const&>()) == "inner<int const&>");
    BOOST_TEST(to_string(nm2::my_template_type<std::exception>::inner<int const&>()) == "inner<int const&>");
    BOOST_TEST(to_string(nm2::my_template_type<int>::inner<int const*>()) == "inner<int const*>");
    BOOST_TEST(to_string(nm2::my_template_type<std::exception>::inner<int const*>()) == "inner<int const*>");
    BOOST_TEST(to_string(nm2::my_template_type<int>::inner<int const*&>()) == "inner<int const*&>");
    BOOST_TEST(to_string(nm2::my_template_type<std::exception>::inner<int const*&>()) == "inner<int const*&>");
}

namespace {
template<typename T1, typename T2, typename T3>
struct my_tpl
{};
} // namespace

BOOST_AUTO_TEST_CASE(name_of_nested_template_with_multiple_arguments_is_extracted)
{
    BOOST_TEST(to_string(std::vector<int>()) == "vector<int, allocator<int>>");
    BOOST_TEST(to_string(std::vector<std::vector<int>>()) ==
               "vector<vector<int, allocator<int>>, allocator<vector<int, allocator<int>>>>");
    BOOST_TEST(to_string(my_tpl<my_tpl<int, int, int>, my_tpl<int, int, int>, my_tpl<int, int, int>>()) ==
               "my_tpl<my_tpl<int, int, int>, my_tpl<int, int, int>, my_tpl<int, int, int>>");
}
