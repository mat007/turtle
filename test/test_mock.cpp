// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "mock_error.hpp"
#include <turtle/mock.hpp>
#include <boost/test/unit_test.hpp>
#include <functional>

namespace {
template<typename T>
void my_function(T& t)
{
    t.my_method("some parameter");
}
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(my_method, 1, void(const std::string&), my_tag)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_object_for_static_polymorphism, mock_error_fixture)
{
    const mock_class m;
    MOCK_EXPECT(m.my_tag).once().with("some parameter");
    my_function(m);
    CHECK_CALLS(1);
}

namespace {
MOCK_CLASS(mock_class_with_operator)
{
    MOCK_CONST_METHOD(operator+=, 1, mock_class_with_operator &(int), addition)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_addition_operator, mock_error_fixture)
{
    mock_class_with_operator m;
    MOCK_EXPECT(m.addition).once().returns(std::ref(m));
    m += 1;
    CHECK_CALLS(1);
}

namespace {
MOCK_CLASS(mock_class_with_conversion_operator)
{
    MOCK_CONVERSION_OPERATOR(operator, int, conversion)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_conversion_operator, mock_error_fixture)
{
    mock_class_with_conversion_operator m;
    MOCK_EXPECT(m.conversion).once().returns(42);
    BOOST_CHECK_EQUAL(42, static_cast<int>(m));
    CHECK_CALLS(1);
}

namespace {
template<typename T>
MOCK_CLASS(mock_template_class_with_conversion_operator)
{
    MOCK_CONVERSION_OPERATOR(operator, T, conversion)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_template_conversion_operator, mock_error_fixture)
{
    mock_template_class_with_conversion_operator<int> m;
    MOCK_EXPECT(m.conversion).once().returns(42);
    BOOST_CHECK_EQUAL(42, static_cast<int>(m));
    CHECK_CALLS(1);
}

namespace {
MOCK_CLASS(mock_class_with_const_conversion_operator)
{
    MOCK_CONST_CONVERSION_OPERATOR(operator, int, conversion)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_const_conversion_operator, mock_error_fixture)
{
    mock_class_with_const_conversion_operator m;
    MOCK_EXPECT(m.conversion).once().returns(42);
    int i = m;
    BOOST_CHECK_EQUAL(42, i);
    CHECK_CALLS(1);
}

namespace {
MOCK_CLASS(mock_class_with_non_const_conversion_operator)
{
    MOCK_CONST_CONVERSION_OPERATOR(operator, int, conversion)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_non_const_conversion_operator, mock_error_fixture)
{
    mock_class_with_non_const_conversion_operator m;
    MOCK_EXPECT(m.conversion).once().returns(42);
    int i = m;
    BOOST_CHECK_EQUAL(42, i);
    CHECK_CALLS(1);
}

namespace {
template<typename T>
MOCK_CLASS(mock_template_class_with_const_conversion_operator)
{
    MOCK_CONST_CONVERSION_OPERATOR(operator, T, conversion)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_template_const_conversion_operator, mock_error_fixture)
{
    mock_template_class_with_const_conversion_operator<int> m;
    MOCK_EXPECT(m.conversion).once().returns(42);
    BOOST_CHECK_EQUAL(42, static_cast<int>(m));
    CHECK_CALLS(1);
}

namespace {
template<typename T>
MOCK_CLASS(mock_template_class_with_non_const_conversion_operator)
{
    MOCK_NON_CONST_CONVERSION_OPERATOR(operator, T, conversion)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_template_non_const_conversion_operator, mock_error_fixture)
{
    mock_template_class_with_non_const_conversion_operator<int> m;
    MOCK_EXPECT(m.conversion).once().returns(42);
    BOOST_CHECK_EQUAL(42, static_cast<int>(m));
    CHECK_CALLS(1);
}

namespace {
MOCK_CLASS(my_mock)
{
    MOCK_CONST_METHOD(my_method, 1, void(int), my_method)
    MOCK_CONST_METHOD(my_method_2, 1, void(int), my_method_2)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(MOCK_CONST_METHOD_macro_defines_a_bindable_method, mock_error_fixture)
{
    my_mock m;
    const auto f = std::bind(&my_mock::my_method, &m, 42);
    (void)f;
}

BOOST_FIXTURE_TEST_CASE(MOCK_VERIFY_macro, mock_error_fixture)
{
    my_mock m;
    MOCK_VERIFY(m.my_method);
}

BOOST_FIXTURE_TEST_CASE(MOCK_RESET_macro, mock_error_fixture)
{
    my_mock m;
    MOCK_RESET(m.my_method);
}

BOOST_FIXTURE_TEST_CASE(MOCK_EXPECT_macro, mock_error_fixture)
{
    my_mock m;
    MOCK_EXPECT(m.my_method).once().with(42);
    m.my_method(42);
    CHECK_CALLS(1);
}

namespace {
template<typename T>
std::string to_string(const T& t)
{
    std::stringstream s;
    s << t;
    return s.str();
}
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_object_is_named, mock_error_fixture)
{
    my_mock m;
    BOOST_CHECK_EQUAL("?.my_mock::my_method", to_string(MOCK_ANONYMOUS_HELPER(m.my_method)));
    BOOST_CHECK_EQUAL("?.my_mock::my_method_2", to_string(MOCK_ANONYMOUS_HELPER(m.my_method_2)));
    BOOST_CHECK_EQUAL("m.my_mock::my_method", to_string(MOCK_HELPER(m.my_method)));
    BOOST_CHECK_EQUAL("m.my_mock::my_method_2", to_string(MOCK_ANONYMOUS_HELPER(m.my_method_2)));
    BOOST_CHECK_EQUAL("m.my_mock::my_method", to_string(MOCK_ANONYMOUS_HELPER(m.my_method)));
    BOOST_CHECK_EQUAL("m.my_mock::my_method", to_string(MOCK_HELPER(m.my_method)));
}

BOOST_FIXTURE_TEST_CASE(mock_object_shared_pointer_is_named, mock_error_fixture)
{
    std::shared_ptr<my_mock> m(new my_mock);
    BOOST_CHECK_EQUAL("?.my_mock::my_method", to_string(MOCK_ANONYMOUS_HELPER(m->my_method)));
    BOOST_CHECK_EQUAL("m->my_mock::my_method", to_string(MOCK_HELPER(m->my_method)));
    BOOST_CHECK_EQUAL("m->my_mock::my_method", to_string(MOCK_ANONYMOUS_HELPER(m->my_method)));
    BOOST_CHECK_EQUAL("m->my_mock::my_method", to_string(MOCK_HELPER(m->my_method)));
}

BOOST_FIXTURE_TEST_CASE(mock_object_const_shared_pointer_is_named, mock_error_fixture)
{
    const std::shared_ptr<my_mock> m(new my_mock);
    BOOST_CHECK_EQUAL("?.my_mock::my_method", to_string(MOCK_ANONYMOUS_HELPER(m->my_method)));
    BOOST_CHECK_EQUAL("m->my_mock::my_method", to_string(MOCK_HELPER(m->my_method)));
    BOOST_CHECK_EQUAL("m->my_mock::my_method", to_string(MOCK_ANONYMOUS_HELPER(m->my_method)));
    BOOST_CHECK_EQUAL("m->my_mock::my_method", to_string(MOCK_HELPER(m->my_method)));
}

namespace {
struct my_custom_mock
{
    MOCK_METHOD(my_method, 0, void(), my_tag)
    MOCK_METHOD(my_method_2, 0, void(), my_tag_2)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(custom_mock_object_without_macros_and_without_inheriting_from_object_is_named,
                        mock_error_fixture)
{
    my_custom_mock m;
    BOOST_CHECK_EQUAL("?.my_custom_mock::my_tag", to_string(MOCK_ANONYMOUS_HELPER(m.my_tag)));
    BOOST_CHECK_EQUAL("?.my_custom_mock::my_tag_2", to_string(MOCK_ANONYMOUS_HELPER(m.my_tag_2)));
    BOOST_CHECK_EQUAL("m.my_custom_mock::my_tag", to_string(MOCK_HELPER(m.my_tag)));
    BOOST_CHECK_EQUAL("m.my_custom_mock::my_tag_2", to_string(MOCK_ANONYMOUS_HELPER(m.my_tag_2)));
    BOOST_CHECK_EQUAL("m.my_custom_mock::my_tag", to_string(MOCK_ANONYMOUS_HELPER(m.my_tag)));
    BOOST_CHECK_EQUAL("m.my_custom_mock::my_tag", to_string(MOCK_HELPER(m.my_tag)));
}

namespace {
struct my_custom_mock_object : mock::object
{
    MOCK_METHOD(my_method, 0, void(), my_tag)
    MOCK_METHOD(my_method_2, 0, void(), my_tag_2)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(custom_mock_object_without_macros_is_named, mock_error_fixture)
{
    my_custom_mock_object m;
    BOOST_CHECK_EQUAL("?.my_custom_mock_object::my_tag", to_string(MOCK_ANONYMOUS_HELPER(m.my_tag)));
    BOOST_CHECK_EQUAL("?.my_custom_mock_object::my_tag_2", to_string(MOCK_ANONYMOUS_HELPER(m.my_tag_2)));
    BOOST_CHECK_EQUAL("m.my_custom_mock_object::my_tag", to_string(MOCK_HELPER(m.my_tag)));
    BOOST_CHECK_EQUAL("m.my_custom_mock_object::my_tag_2", to_string(MOCK_ANONYMOUS_HELPER(m.my_tag_2)));
    BOOST_CHECK_EQUAL("m.my_custom_mock_object::my_tag", to_string(MOCK_ANONYMOUS_HELPER(m.my_tag)));
    BOOST_CHECK_EQUAL("m.my_custom_mock_object::my_tag", to_string(MOCK_HELPER(m.my_tag)));
}

BOOST_FIXTURE_TEST_CASE(mock_functor, mock_error_fixture)
{
    MOCK_FUNCTOR(f1, void());
    MOCK_FUNCTOR(f2, int(const std::string&));
}

namespace {
template<typename T>
struct tpl_functor_class
{
    MOCK_FUNCTOR(f, void(T));
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_functor_reset, mock_error_fixture)
{
    MOCK_FUNCTOR(f, void());
    MOCK_RESET(f);
    mock::reset(f);
}

BOOST_FIXTURE_TEST_CASE(mock_functor_verify, mock_error_fixture)
{
    MOCK_FUNCTOR(f, void());
    MOCK_VERIFY(f);
    mock::verify(f);
}

BOOST_FIXTURE_TEST_CASE(mock_functor_is_named, mock_error_fixture)
{
    MOCK_FUNCTOR(f, void());
    BOOST_CHECK_EQUAL("f", to_string(MOCK_HELPER(f)));
}

namespace {
MOCK_FUNCTION(mock_function, 1, float(int), mock_function)
}

BOOST_FIXTURE_TEST_CASE(mock_function_is_named, mock_error_fixture)
{
    BOOST_CHECK_EQUAL("mock_function", to_string(MOCK_HELPER(mock_function)));
}

namespace {
MOCK_CLASS(static_function_class)
{
    MOCK_STATIC_METHOD(f, 1, float(int), f)
};
} // namespace

BOOST_FIXTURE_TEST_CASE(mock_static_function_is_named, mock_error_fixture)
{
    BOOST_CHECK_EQUAL("static_function_class::f", to_string(MOCK_HELPER(static_function_class::f)));
}

namespace {
MOCK_CLASS(round_parenthesized_signature)
{
    MOCK_METHOD(m0, 0, MOCK_PROTECT_SIGNATURE(std::map<int, int>()), m0)
    MOCK_STATIC_METHOD(m1, 0, MOCK_PROTECT_SIGNATURE(std::map<int, int>()), m1)
    MOCK_FUNCTOR(f0, MOCK_PROTECT_SIGNATURE(std::map<int, int>()));
};
MOCK_FUNCTION(fun0, 0, MOCK_PROTECT_SIGNATURE(std::map<int, int>()), fun0)
} // namespace

namespace {
struct base
{
    virtual ~base() = default;

    virtual void m1() = 0;
    virtual void m10() const = 0;
    virtual void m11() = 0;
    virtual void m12() noexcept = 0;
    virtual void m13() && = 0;
    virtual void m14() = 0;
    virtual void m14(int, float) = 0;
    virtual void m14(int, int) = 0;
    virtual void m14(int) = 0;
    virtual void m14(int) const noexcept = 0;
    virtual void m15() & = 0;
    virtual void m15() && = 0;
};

MOCK_BASE_CLASS(variadic, base)
{
    MOCK_METHOD(m1, 0)
    MOCK_METHOD(m2, 0, void())
    MOCK_METHOD(m3, 0, void(), m3)
    MOCK_CONST_METHOD(m10, 0)
    MOCK_CONST_METHOD(m4, 0, void())
    MOCK_CONST_METHOD(m5, 0, void(), m5)
    MOCK_NON_CONST_METHOD(m11, 0)
    MOCK_NON_CONST_METHOD(m6, 0, void())
    MOCK_NON_CONST_METHOD(m7, 0, void(), m7)

    MOCK_METHOD_EXT(m12, 0, (noexcept override))
    MOCK_METHOD_EXT(m13, 0, (&&override))
    // Overloaded method
    MOCK_METHOD(m14, 0, void(), m14_empty)
    MOCK_METHOD(m14, 2, void(int, float), m14_int_float, ())
    MOCK_METHOD(m14, 2, void(int, int), m14_int_int, (override))
    MOCK_METHOD(m14, 1, void(int), m14_int, (override, const noexcept override))
    MOCK_METHOD_EXT(m15, 0, (&override, &&override), void())
    MOCK_STATIC_METHOD(m8, 0, void())
    MOCK_STATIC_METHOD(m9, 0, void(), m9)
};
void instantiate_class()
{
    variadic inst; // If this compiles all pure virtual methods were mocked
    const variadic& cinst = inst;
    (void)cinst; // Avoid unused variable warning
    static_assert(noexcept(inst.m12()), "noexcept should be kept");
    static_assert(!noexcept(inst.m14()), "noexcept should not be set");
    static_assert(noexcept(cinst.m14(1)), "noexcept should be kept");
    static_assert(!noexcept(inst.m14(1)), "noexcept should not be set");
    static_assert(noexcept(std::declval<const variadic>().m14(1)), "noexcept should be kept");
}

template<typename T>
MOCK_BASE_CLASS(variadic_tpl, base)
{
    MOCK_METHOD(m1, 0, void())
    MOCK_METHOD(m2, 0, T())
    MOCK_METHOD(m3, 0, T(), m3)
    MOCK_CONST_METHOD(m4, 0, T())
    MOCK_CONST_METHOD(m5, 0, T(), m5)
    MOCK_NON_CONST_METHOD(m6, 0, T())
    MOCK_NON_CONST_METHOD(m7, 0, T(), m7)
    MOCK_STATIC_METHOD(m8, 0, T())
    MOCK_STATIC_METHOD(m9, 0, T(), m9)
};

MOCK_BASE_CLASS(comma_base, std::map<int, int>)
{};

MOCK_FUNCTION(fun1, 0, void())
MOCK_FUNCTION(fun2, 0, void(), fun2)
MOCK_FUNCTION(fun3, 0, MOCK_PROTECT_SIGNATURE(std::map<int, int>()))

MOCK_FUNCTOR(f_variadic, std::map<int, int>());
} // namespace

#ifdef BOOST_MSVC
#    define MOCK_STDCALL __stdcall
#else
#    define MOCK_STDCALL
#endif

namespace stdcall {
struct base
{
    virtual ~base() = default;

    virtual void MOCK_STDCALL m1() = 0;
};

MOCK_BASE_CLASS(derived, base)
{
    MOCK_CONSTRUCTOR(MOCK_STDCALL derived, 0, (), derived)
    MOCK_DESTRUCTOR(MOCK_STDCALL ~derived, derived)
    MOCK_CONVERSION_OPERATOR(MOCK_STDCALL operator, int, to_int)
    MOCK_METHOD(MOCK_STDCALL m1, 0, void(), m1)
    MOCK_METHOD(MOCK_STDCALL m2, 0, void(), m2)
    MOCK_METHOD(MOCK_STDCALL m3, 0, void(), m3)
    MOCK_STATIC_METHOD(MOCK_STDCALL m4, 0, void(), m4)
};

MOCK_FUNCTION(MOCK_STDCALL f, 0, void(), f)
} // namespace stdcall
