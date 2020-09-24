// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <turtle/mock.hpp>

namespace
{
//[ limitations_throw_specifier_problem
    struct base_class
    {
        virtual ~base_class() = default;

        virtual void method() throw() = 0;
    };
//]

//[ limitations_throw_specifier_solution
    MOCK_BASE_CLASS( mock_class, base_class )
    {
        void method() throw() override
        {
            method_proxy();
        }
        MOCK_METHOD( method_proxy, 0, void(), method )
    };
//]
}

BOOST_AUTO_TEST_CASE(call_method_proxy)
{
    mock_class b;
    MOCK_EXPECT(b.method).once();
    static_cast<base_class*>(&b)->method();
}
