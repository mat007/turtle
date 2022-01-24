// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ retrieve_cref_problem
namespace {
class base_class
{
public:
    virtual void method(int value) = 0;
};

class my_class
{
public:
    explicit my_class(base_class&);

    void process(); // the processing will call 'method' two times with the same value, but we don't know what value
                    // beforehand
};
} // namespace
//]

namespace {
static base_class* global_b = nullptr;
my_class::my_class(base_class& b)
{
    global_b = &b;
}
void my_class::process()
{
    int secret_value = 42;
    global_b->method(secret_value);
    global_b->method(secret_value);
}
} // namespace

//[ retrieve_cref_solution
#include <turtle/mock.hpp>
#include <boost/test/unit_test.hpp>

namespace {
MOCK_BASE_CLASS(mock_base_class, base_class)
{
    MOCK_METHOD(method, 1)
};
} // namespace

BOOST_AUTO_TEST_CASE(method_is_called_two_times_with_the_same_value)
{
    mock_base_class mock;
    my_class c(mock);
    int value;
    MOCK_EXPECT(mock.method).once().with(mock::retrieve(value)); // on first call retrieve the value, this expectation
                                                                 // takes precedence because it can never fail
    MOCK_EXPECT(mock.method)
      .once()
      .with(std::cref(value)); // on second call compare the previously retrieved value with the newly received one
    c.process();
}
//]
