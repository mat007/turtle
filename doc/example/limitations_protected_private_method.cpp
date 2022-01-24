// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/mock.hpp>
#include <boost/test/unit_test.hpp>

namespace {
//[ limitations_protected_private_method_problem
class base
{
public:
    void call()
    {
        method_1();
        method_2();
    }

protected:
    virtual void method_1() = 0;

private:
    virtual void method_2() = 0;
};
//]

//[ limitations_protected_private_method_solution
MOCK_BASE_CLASS(mock_base, base)
{
    MOCK_METHOD(method_1, 0, void())
    MOCK_METHOD(method_2, 0, void())
};
//]
} // namespace

BOOST_AUTO_TEST_CASE(mocked_methods_are_called)
{
    mock_base b;
    MOCK_EXPECT(b.method_1).once();
    MOCK_EXPECT(b.method_2).once();
    static_cast<base*>(&b)->call();
}
