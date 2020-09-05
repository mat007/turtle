// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_AUTO_TEST_MAIN
#include <turtle/mock.hpp>
#include <boost/test/auto_unit_test.hpp>

namespace {
//[ limitations_throw_specifier_problem
struct base_class
{
    virtual ~base_class() {}

    virtual void method() throw();
};
//]

//[ limitations_throw_specifier_solution
MOCK_BASE_CLASS(mock_class, base_class){void method() throw(){method_proxy();
} // namespace
MOCK_METHOD(method_proxy, 0, void(), method)
}
;
//]
}
