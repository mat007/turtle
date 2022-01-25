// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/mock.hpp>
#include <boost/test/unit_test.hpp>

namespace class_example_1 {
//[ class_example_1
MOCK_CLASS(mock_class)
{};

BOOST_AUTO_TEST_CASE(demonstrates_instantiating_a_mock_class)
{
    mock_class c;
}
//]
} // namespace class_example_1

namespace class_example_2 {
//[ class_example_2
template<typename T>
MOCK_CLASS(mock_class)
{};

BOOST_AUTO_TEST_CASE(demonstrates_instantiating_a_template_mock_class)
{
    mock_class<int> c;
}
//]
} // namespace class_example_2

namespace class_example_3 {
//[ class_example_3
struct base_class
{};

MOCK_BASE_CLASS(mock_class, base_class)
{};

BOOST_AUTO_TEST_CASE(demonstrates_instantiating_a_derived_mock_class)
{
    mock_class c;
}
//]
} // namespace class_example_3

namespace class_example_4 {
//[ class_example_4
template<typename T>
struct base_class
{};

template<typename T>
MOCK_BASE_CLASS(mock_class, base_class<T>)
{};

BOOST_AUTO_TEST_CASE(demonstrates_instantiating_a_template_derived_mock_class)
{
    mock_class<int> c;
}
//]
} // namespace class_example_4

namespace class_example_5 {
//[ class_example_5
struct name : mock::object // equivalent to using MOCK_CLASS
{};
//]
} // namespace class_example_5

namespace class_example_6 {
//[ class_example_6
template<typename T>
struct name : mock::object // equivalent to using MOCK_CLASS
{};
//]
} // namespace class_example_6

namespace class_example_7 {
//[ class_example_7
class base
{};

struct name : base, mock::object // equivalent to using MOCK_BASE_CLASS
{
    // this is required for the shortest form of MOCK_METHOD to work when not using MOCK_BASE_CLASS
    using base_type = base;
};
//]
} // namespace class_example_7

namespace class_example_8 {
//[ class_example_8
template<typename T>
struct base
{};

template<typename T>
struct name : base<T>, mock::object
{
    using base_type = base<T>;
};
//]
} // namespace class_example_8

namespace member_function_example_1 {
//[ member_function_example_1
struct base_class
{
    virtual ~base_class() = default;
    virtual void method(int) = 0;
};

MOCK_BASE_CLASS(mock_class, base_class)
{
    MOCK_METHOD(method, 1) // only possible when referring unambiguously to a base class method
};
//]
} // namespace member_function_example_1

namespace member_function_example_2 {
//[ member_function_example_2
struct base_class
{
    virtual ~base_class() = default;
    virtual void method(int, const std::string&) = 0;
    virtual void method(float) = 0;
};

MOCK_BASE_CLASS(mock_class, base_class)
{
    // both the signature and identifier must be specified because of ambiguity due to overloading
    MOCK_METHOD(method, 2, void(int, const std::string&), identifier_1)
    // the identifier must differ from the previous one in order to fully disambiguate methods
    MOCK_METHOD(method, 1, void(float), identifier_2)
};
//]
} // namespace member_function_example_2

namespace member_function_example_3 {
//[ member_function_example_3
struct base_class
{
    virtual ~base_class() = default;
    virtual void method(float) = 0;
    virtual void method(float) const = 0;
};

MOCK_BASE_CLASS(mock_class, base_class)
{
    MOCK_METHOD(method, 1, void(float)) // this generates both const and non-const versions
};
//]
} // namespace member_function_example_3

namespace member_function_example_4 {
//[ member_function_example_4
struct base_class
{
    virtual ~base_class() = default;
    virtual void method(float) = 0;
    virtual void method(float) const = 0;
};

MOCK_BASE_CLASS(mock_class, base_class)
{
    // this generates only the const version
    MOCK_CONST_METHOD(method, 1, void(float), identifier_1)
    // this generates only the non-const version, with a different identifier
    MOCK_NON_CONST_METHOD(method, 1, void(float), identifier_2)
};
//]
} // namespace member_function_example_4

namespace member_function_example_5 {
//[ member_function_example_5
struct base_class
{
    virtual ~base_class() = default;
    virtual void method(float) = 0;
};

struct mock_class : base_class
{
    using base_type = base_class; // this is required for MOCK_METHOD to work when not using MOCK_BASE_CLASS

    MOCK_METHOD(method, 1)
};
//]
} // namespace member_function_example_5

namespace member_function_example_6 {
//[ member_function_example_6
MOCK_CLASS(mock_class)
{
    MOCK_NON_CONST_METHOD(operator=,
                          1,
                          mock_class &(const mock_class&),
                          assignment) // operators require a custom identifier
};
//]
} // namespace member_function_example_6

namespace member_function_example_7 {
//[ member_function_example_7
template<typename T>
MOCK_CLASS(mock_class)
{
    // includes a template parameter of the class
    MOCK_METHOD(method, 1, void(const T&))
};
//]
} // namespace member_function_example_7

namespace member_function_example_8 {
//[ member_function_example_8
MOCK_CLASS(mock_class)
{
    // the signature must be wrapped in MOCK_PROTECT_SIGNATURE if the return type contains a comma
    MOCK_METHOD(method, 0, MOCK_PROTECT_SIGNATURE(std::map<int, int>()))
};
//]

static_assert(std::is_same<decltype(std::declval<mock_class>().method()), std::map<int, int>>::value,
              "Wrong return value");

} // namespace member_function_example_8

#ifdef BOOST_MSVC
namespace member_function_example_9 {
//[ member_function_example_9
MOCK_CLASS(mock_class)
{
    // all parameters must be provided when specifying a different calling convention
    MOCK_METHOD(__stdcall method, 0, void(), method)
};
//]
} // namespace member_function_example_9
#endif

namespace static_member_function_example_1 {
//[ static_member_function_example_1
MOCK_CLASS(mock_class)
{
    MOCK_STATIC_METHOD(method, 1, float(int))
};
//]
} // namespace static_member_function_example_1

namespace static_member_function_example_2 {
//[ static_member_function_example_2
template<typename T>
MOCK_CLASS(mock_class)
{
    MOCK_STATIC_METHOD(method, 1, void(T)) // includes a template parameter of the class
};
//]
} // namespace static_member_function_example_2

#ifdef BOOST_MSVC
namespace static_member_function_example_3 {
//[ static_member_function_example_3
MOCK_CLASS(mock_class)
{
    // all parameters must be provided when specifying a different calling convention
    MOCK_STATIC_METHOD(__stdcall method, 0, void(), method)
};
//]
} // namespace static_member_function_example_3
#endif

namespace constructor_example_1 {
//[ constructor_example_1
MOCK_CLASS(mock_class)
{
    MOCK_CONSTRUCTOR(mock_class, 2, (int, const std::string&), identifier)
};
//]
} // namespace constructor_example_1

namespace constructor_example_2 {
//[ constructor_example_2
template<typename T>
MOCK_CLASS(mock_class)
{
    MOCK_CONSTRUCTOR(mock_class, 2, (int, const std::string&), identifier)
    MOCK_CONSTRUCTOR(mock_class, 2, (T, const std::string&), identifier_2) // includes a template parameter of the class
};
//]
} // namespace constructor_example_2

#ifdef BOOST_MSVC
namespace constructor_example_3 {
//[ constructor_example_3
MOCK_CLASS(mock_class)
{
    MOCK_CONSTRUCTOR(__stdcall mock_class, 0, (), constructor)
};
//]
} // namespace constructor_example_3
#endif

namespace destructor_example_1 {
//[ destructor_example_1
MOCK_CLASS(mock_class)
{
    MOCK_DESTRUCTOR(~mock_class, destructor)
};
//]
} // namespace destructor_example_1

#ifdef BOOST_MSVC
namespace destructor_example_2 {
//[ destructor_example_2
MOCK_CLASS(mock_class)
{
    MOCK_DESTRUCTOR(__stdcall ~mock_class, destructor)
};
//]
} // namespace destructor_example_2
#endif

namespace conversion_operator_example_1 {
//[ conversion_operator_example_1
MOCK_CLASS(mock_class)
{
    MOCK_CONVERSION_OPERATOR(operator, int, conversion_to_int)
    MOCK_CONST_CONVERSION_OPERATOR(operator, const std::string&, conversion_to_string)
};
//]
} // namespace conversion_operator_example_1

namespace conversion_operator_example_2 {
//[ conversion_operator_example_2
template<typename T>
MOCK_CLASS(mock_class)
{
    MOCK_CONVERSION_OPERATOR(operator, T, conversion_to_T) // includes a template parameter of the class
    MOCK_CONST_CONVERSION_OPERATOR(operator, const std::string&, const_conversion_to_string)
    MOCK_NON_CONST_CONVERSION_OPERATOR(operator, const std::string&, non_const_conversion_to_string)
};
//]
} // namespace conversion_operator_example_2

#ifdef BOOST_MSVC
namespace conversion_operator_example_3 {
//[ conversion_operator_example_3
MOCK_CLASS(mock_class)
{
    MOCK_CONVERSION_OPERATOR(__stdcall operator, int, conversion_to_int)
};
//]
} // namespace conversion_operator_example_3
#endif

namespace function_example_1 {
//[ function_example_1
MOCK_FUNCTION(f, 1, void(int))

BOOST_AUTO_TEST_CASE(demonstrates_instantiating_a_mock_function)
{
    MOCK_EXPECT(f).once().with(3);
    f(3);
}
//]
} // namespace function_example_1

#ifdef BOOST_MSVC
namespace function_example_2 {
//[ function_example_2
// all parameters must be provided when specifying a different calling convention
MOCK_FUNCTION(__stdcall f, 0, void(), f)
//]
} // namespace function_example_2
#endif

namespace functor_example_1 {
//[ functor_example_1
BOOST_AUTO_TEST_CASE(demonstrates_instantiating_a_mock_functor)
{
    MOCK_FUNCTOR(f, void(int));
    MOCK_EXPECT(f).once().with(3);
    f(3);
}
//]
} // namespace functor_example_1

namespace functor_example_2 {
//[ functor_example_2
template<typename T>
struct mock_class
{
    MOCK_FUNCTOR(f, void(T));
};

BOOST_AUTO_TEST_CASE(demonstrates_instantiating_a_mock_functor_inside_a_class)
{
    mock_class<int> c;
    MOCK_EXPECT(c.f).once().with(3);
    c.f(3);
}
//]
} // namespace functor_example_2

namespace expectation_example_1 {
//[ expectation_example_1
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 1, int(int), method)
    MOCK_METHOD(method, 2, void(const std::string&, float), method2)
};

BOOST_AUTO_TEST_CASE(demonstrates_configuring_mock_objects)
{
    mock_class c;
    mock::sequence s;
    MOCK_EXPECT(c.method).once().with(0).in(s).returns(42);
    MOCK_EXPECT(c.method2).never().with("ok", mock::any);
    MOCK_EXPECT(c.method2).at_least(2).in(s).throws(std::runtime_error("error !"));
    BOOST_TEST(c.method(0) == 42);
    BOOST_CHECK_THROW(c.method("not ok", 1.f), std::runtime_error);
    BOOST_CHECK_THROW(c.method("not ok", 2.f), std::runtime_error);
}
//]
} // namespace expectation_example_1

namespace invocation_example_1 {
//[ invocation_example_1
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 2, void(int, const std::string&))
    MOCK_METHOD(method2, 1, void(int))
};

BOOST_AUTO_TEST_CASE(demonstrates_setting_up_invocations_on_a_mock_method)
{
    mock_class c;
    MOCK_EXPECT(c.method).once(); // can only be called once
    MOCK_EXPECT(c.method2);       // can be called an unlimited number of times
    c.method(42, "Hello world!");
    c.method2(42);
    c.method2(42);
    c.method2(42);
}
//]
} // namespace invocation_example_1

namespace invocation_example_2 {
//[ invocation_example_2
BOOST_AUTO_TEST_CASE(demonstrates_setting_up_an_invocation_on_a_mock_functor)
{
    MOCK_FUNCTOR(f, void(int, const std::string&));
    MOCK_EXPECT(f).once();
    f(42, "Hello world!");
}
//]
} // namespace invocation_example_2

namespace invocation_example_3 {
//[ invocation_example_3
MOCK_FUNCTION(f, 1, void(int))

BOOST_AUTO_TEST_CASE(demonstrates_setting_up_an_invocation_on_a_mock_function)
{
    MOCK_EXPECT(f).once();
    f(42);
}
//]
} // namespace invocation_example_3

namespace invocation_example_4 {
//[ invocation_example_4
MOCK_CLASS(mock_class)
{
    MOCK_STATIC_METHOD(method1, 1, void(int))
    MOCK_STATIC_METHOD(method2, 1, void(int))
};

BOOST_AUTO_TEST_CASE(demonstrates_setting_up_an_invocation_on_a_mock_static_method)
{
    mock_class c;
    MOCK_EXPECT(c.method1).once();
    MOCK_EXPECT(mock_class::method2).once(); // does the same (but for the other method)
    c.method1(42);
    c.method2(42);
}
//]
} // namespace invocation_example_4

namespace constraints_example_1 {
//[ constraints_example_1
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method1, 2, void(int, const std::string&))
    MOCK_METHOD(method2, 2, void(int, const std::string&))
};

BOOST_AUTO_TEST_CASE(demonstrates_adding_builtin_constraints)
{
    mock_class c;
    MOCK_EXPECT(c.method1).with(mock::equal(3), mock::equal("some string"));
    MOCK_EXPECT(c.method2).with(3, "some string"); // similar to the previous one using short-cuts
    c.method1(3, "some string");
    c.method2(3, "some string");
}
//]
} // namespace constraints_example_1

namespace constraints_example_2 {
//[ constraints_example_2
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 1, void(int))
};

bool custom_constraint(int actual)
{
    return actual == 42;
}

BOOST_AUTO_TEST_CASE(demonstrates_adding_a_custom_constraint_with_a_free_function)
{
    mock_class c;
    MOCK_EXPECT(c.method).with(&custom_constraint);
    c.method(42);
}
//]
} // namespace constraints_example_2

namespace constraints_example_3 {
//[ constraints_example_3
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 1, void(int))
};

bool custom_constraint(int expected, int actual)
{
    return expected == actual;
}

BOOST_AUTO_TEST_CASE(demonstrates_adding_a_custom_constraint_with_a_standard_library_functor)
{
    mock_class c;
    MOCK_EXPECT(c.method).with(
      std::bind1st(std::ptr_fun(&custom_constraint), 42)); // std::ptr_fun creates an std::unary_function
    c.method(42);
}
//]
} // namespace constraints_example_3

namespace constraints_example_4 {
//[ constraints_example_4
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 1, void(int))
};

bool custom_constraint(int expected, int actual)
{
    return expected == actual;
}

BOOST_AUTO_TEST_CASE(demonstrates_adding_a_custom_constraint_with_std_bind)
{
    mock_class c;
    using namespace std::placeholders;
    MOCK_EXPECT(c.method).with(std::bind(&custom_constraint, 42, _1));
    c.method(42);
}
//]
} // namespace constraints_example_4

#ifndef BOOST_MSVC // this produces an ICE with all versions of MSVC

#    include <boost/lambda/lambda.hpp>

namespace constraints_example_5 {
//[ constraints_example_5
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 1, void(int))
};

BOOST_AUTO_TEST_CASE(demonstrates_adding_a_custom_constraint_with_boost_lambda)
{
    mock_class c;
    MOCK_EXPECT(c.method).with(boost::lambda::_1 == 42);
    c.method(42);
}
//]
} // namespace constraints_example_5

#endif // BOOST_MSVC

#include <boost/phoenix/phoenix.hpp>

namespace constraints_example_6 {
//[ constraints_example_6
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method1, 1, void(int))
    MOCK_METHOD(method2, 1, void(int))
};

BOOST_AUTO_TEST_CASE(demonstrates_adding_a_custom_constraint_with_boost_phoenix)
{
    mock_class c;
    MOCK_EXPECT(c.method1).with(boost::phoenix::arg_names::arg1 == 42);
    MOCK_EXPECT(c.method2).with(boost::phoenix::arg_names::_1 == 42);
    c.method1(42);
    c.method2(42);
}
//]
} // namespace constraints_example_6

namespace constraints_example_7 {
//[ constraints_example_7
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 1, void(int))
};

BOOST_AUTO_TEST_CASE(demonstrates_adding_a_constraint_with_cxx11_lambda)
{
    mock_class c;
    MOCK_EXPECT(c.method).with([](int actual) { return 42 == actual; });
    c.method(42);
}
//]
} // namespace constraints_example_7

namespace constraints_example_8 {
//[ constraints_example_8
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 2, void(int, const std::string&))
};

BOOST_AUTO_TEST_CASE(demonstrates_combining_constraints)
{
    mock_class c;
    MOCK_EXPECT(c.method).with(mock::less(4) && mock::greater(2), !mock::equal(""));
    c.method(3, "Hello World!");
}
//]
} // namespace constraints_example_8

namespace constraints_example_9 {
//[ constraints_example_9
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 2, void(const std::string&, std::size_t))
};

bool custom_constraint(const std::string& actual_1, std::size_t actual_2)
{
    return actual_1.size() <= actual_2;
}

BOOST_AUTO_TEST_CASE(demonstrates_one_constraint_for_all_arguments)
{
    mock_class c;
    MOCK_EXPECT(c.method).with(&custom_constraint);
    c.method("1234", 4);
}
//]
} // namespace constraints_example_9

namespace sequence_example_1 {
//[ sequence_example_1
MOCK_CLASS(mock_class_1)
{
    MOCK_METHOD(method_1, 0, void())
};

MOCK_CLASS(mock_class_2)
{
    MOCK_METHOD(method_2, 0, void())
};

MOCK_CLASS(mock_class_3)
{
    MOCK_METHOD(method_3, 0, void())
};

BOOST_AUTO_TEST_CASE(demonstrates_enforcing_several_expectation_orders)
{
    mock_class_1 c_1;
    mock_class_2 c_2;
    mock_class_3 c_3;
    mock::sequence s_1, s_2;
    MOCK_EXPECT(c_1.method_1).in(s_1);
    MOCK_EXPECT(c_2.method_2)
      .in(s_2); // c_1.method_1 and c_2.method_2 are in different sequences and can be called in any order
    MOCK_EXPECT(c_3.method_3).in(s_1, s_2); // c_3.method_3 must be called after both c_1.method_1 and c_2.method_2
    c_2.method_2();
    c_1.method_1();
    c_3.method_3();
}
//]
} // namespace sequence_example_1

namespace action_example_1 {
//[ action_example_1
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 1, int(int))
};

int function(int i)
{
    return i;
}

BOOST_AUTO_TEST_CASE(demonstrates_configuring_actions)
{
    mock_class c;
    MOCK_EXPECT(c.method).once().returns(42);
    MOCK_EXPECT(c.method).once().moves(42); // returns by moving the value
    MOCK_EXPECT(c.method).once().throws(std::runtime_error("error !"));
    MOCK_EXPECT(c.method).once().calls(&function); // forwards 'method' parameter to 'function'
    MOCK_EXPECT(c.method).once().calls(
      std::bind(&function, 42)); // drops 'method' parameter and binds 42 as parameter to 'function'
    MOCK_EXPECT(c.method).once().calls([](int i) { return i; }); // uses a C++11 lambda
    BOOST_TEST(c.method(0) == 42);
    BOOST_TEST(c.method(1) == 42);
    BOOST_CHECK_THROW(c.method(0), std::runtime_error);
    BOOST_TEST(c.method(2) == 2);
    BOOST_TEST(c.method(3) == 42);
    BOOST_TEST(c.method(4) == 4);
}
//]
} // namespace action_example_1

namespace action_example_2 {
//[ action_example_2
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 0, int&())
};

BOOST_AUTO_TEST_CASE(demonstrates_configuring_actions_with_references)
{
    mock_class c;
    int i = 0;
    MOCK_EXPECT(c.method).returns(std::ref(i)); // wrap i to store a reference
    c.method() = 42;                            // really change i and not just the stored copy
    BOOST_CHECK_EQUAL(42, i);                   // indeed
}
//]
} // namespace action_example_2

namespace verification_example_1 {
//[ verification_example_1
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 0, void())
};

BOOST_AUTO_TEST_CASE(demonstrates_verifying_a_mock_method)
{
    mock_class c;
    MOCK_VERIFY(c.method); // logs an error and returns false if not all expectations are met
    mock::verify(c);       // verifies all expectations set for all methods of 'c'
    mock::verify();        // verifies all existing mock objects, functions and functors
}
//]
} // namespace verification_example_1

namespace verification_example_2 {
//[ verification_example_2
BOOST_AUTO_TEST_CASE(demonstrates_verifying_a_mock_functor)
{
    MOCK_FUNCTOR(f, void(int));
    MOCK_VERIFY(f);  // logs an error and returns false if not all expectations are met
    mock::verify(f); // behaves the same as MOCK_VERIFY
    mock::verify();  // verifies all existing mock objects, functions and functors
}
//]
} // namespace verification_example_2

namespace verification_example_3 {
//[ verification_example_3
MOCK_FUNCTION(f, 1, void(int))

BOOST_AUTO_TEST_CASE(demonstrates_verifying_a_mock_function)
{
    MOCK_VERIFY(f); // logs an error and returns false if not all expectations are met
    mock::verify(); // verifies all existing mock objects, functions and functors
}
//]
} // namespace verification_example_3

namespace verification_example_4 {
//[ verification_example_4
MOCK_CLASS(mock_class)
{
    MOCK_STATIC_METHOD(method, 0, void())
};

BOOST_AUTO_TEST_CASE(demonstrates_verifying_a_static_mock_method)
{
    mock_class c;
    MOCK_VERIFY(c.method);           // logs an error and returns false if not all expectations are met
    MOCK_VERIFY(mock_class::method); // does the same
    mock::verify();                  // verifies all existing mock objects, functions and functors
}
//]
} // namespace verification_example_4

namespace reset_example_1 {
//[ reset_example_1
MOCK_CLASS(mock_class)
{
    MOCK_METHOD(method, 0, void())
};

BOOST_AUTO_TEST_CASE(demonstrates_resetting_a_mock_method)
{
    mock_class c;
    MOCK_RESET(c.method); // resets all expectations set for 'c.method'
    mock::reset(c);       // resets all expectations set on 'c'
    mock::reset();        // resets all existing mock objects, functions and functors
}
//]
} // namespace reset_example_1

namespace reset_example_2 {
//[ reset_example_2
BOOST_AUTO_TEST_CASE(demonstrates_resetting_a_mock_functor)
{
    MOCK_FUNCTOR(f, void(int));
    MOCK_RESET(f);  // resets all expectations set for 'f'
    mock::reset(f); // behaves the same as MOCK_RESET
    mock::reset();  // resets all existing mock objects, functions and functors
}
//]
} // namespace reset_example_2

namespace reset_example_3 {
//[ reset_example_3
MOCK_FUNCTION(f, 1, void(int))

BOOST_AUTO_TEST_CASE(demonstrates_resetting_a_mock_function)
{
    MOCK_RESET(f); // resets all expectations set for 'f'
    mock::reset(); // resets all existing mock objects, functions and functors
}
//]
} // namespace reset_example_3

namespace reset_example_4 {
//[ reset_example_4
MOCK_CLASS(mock_class)
{
    MOCK_STATIC_METHOD(method, 0, void())
};

BOOST_AUTO_TEST_CASE(demonstrates_resetting_a_static_mock_method)
{
    mock_class c;
    MOCK_RESET(c.method);           // resets all expectations set for 'c::method'
    MOCK_RESET(mock_class::method); // resets all expectations set for 'c::method'
    mock::reset();                  // resets all existing mock objects, functions and functors
}
//]
} // namespace reset_example_4

namespace helpers_example_1 {
//[ helpers_example_1
MOCK_CONSTRAINT(any, true)               // this is how mock::any could be defined
MOCK_CONSTRAINT(forty_two, actual == 42) // this defines a 'forty_two' constraint

BOOST_AUTO_TEST_CASE(mock_constraint_0_arity)
{
    MOCK_FUNCTOR(f, void(int));
    MOCK_EXPECT(f).with(forty_two);
    MOCK_EXPECT(f).with(any);
}
//]
} // namespace helpers_example_1

namespace helpers_example_2 {
//[ helpers_example_2
// this is how mock::equal could be defined
MOCK_CONSTRAINT(equal, expected, actual == expected)
// this defines a 'near' constraint which can be used as 'near( 42 )'
MOCK_CONSTRAINT(near, expected, std::abs(actual - expected) < 0.01)

BOOST_AUTO_TEST_CASE(mock_constraint_1_arity)
{
    MOCK_FUNCTOR(f, void(int));
    MOCK_EXPECT(f).with(near(42));
    MOCK_EXPECT(f).with(equal(42));
}
//]
} // namespace helpers_example_2

namespace helpers_example_3 {
//[ helpers_example_3
// this is how mock::near could be defined
MOCK_CONSTRAINT(near, expected, tolerance, std::abs(actual - expected) <= tolerance)

BOOST_AUTO_TEST_CASE(mock_constraint_2_arity)
{
    MOCK_FUNCTOR(f, void(int));
    MOCK_EXPECT(f).with(near(42, 0.001));
}
//]
} // namespace helpers_example_3
