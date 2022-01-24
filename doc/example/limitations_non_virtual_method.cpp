// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/mock.hpp>
#include <boost/test/unit_test.hpp>

//[ limitations_non_virtual_method_problem
class base
{
public:
    void method() // the method is not virtual
    {}
};
//]

//[ limitations_non_virtual_method_problem_2
MOCK_BASE_CLASS(mock_base, base)
{
    MOCK_METHOD(method, 0)
};
//]

BOOST_AUTO_TEST_CASE(method_not_called_through_base)
{
    mock_base b;
    MOCK_EXPECT(b.method).never();
    static_cast<base*>(&b)->method(); // Doesn't call the mocked method as asserted above
}
