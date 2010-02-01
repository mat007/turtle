//
//  Copyright Mathieu Champlon 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/mock.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#define F(z, n, d) virtual void f##n() = 0;

#define NUMBER_OF_METHODS 20

namespace
{
    class base_class
    {
    public:
        virtual ~base_class() {}
        BOOST_PP_REPEAT_FROM_TO(0, NUMBER_OF_METHODS, F, )
    };
}

#define BOOST_PP_FILENAME_1 "turtle_bench.hpp"
#define BOOST_PP_ITERATION_LIMITS (1, 10)
#include BOOST_PP_ITERATE()

int main()
{}
