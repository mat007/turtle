//
//  Copyright Mathieu Champlon 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_BENCH_NUMBER_OF_METHODS
#define MOCK_BENCH_NUMBER_OF_METHODS 30
#endif

#ifndef MOCK_BENCH_NUMBER_OF_ARGS
#define MOCK_BENCH_NUMBER_OF_ARGS 9
#endif
//#define MOCK_MAX_ARGS MOCK_BENCH_NUMBER_OF_ARGS

#ifndef MOCK_BENCH_NUMBER_OF_CLASSES
#define MOCK_BENCH_NUMBER_OF_CLASSES 20
#endif

#include <turtle/mock.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#define MOCK_BENCH_IDENTITY(z, n, d) d
#define MOCK_BENCH_METHOD(z, n, d) virtual void f##n( d ) = 0;

namespace
{
    class base_class
    {
    public:
        virtual ~base_class() {}
        BOOST_PP_REPEAT(MOCK_BENCH_NUMBER_OF_METHODS, MOCK_BENCH_METHOD, BOOST_PP_ENUM(MOCK_BENCH_NUMBER_OF_ARGS, MOCK_BENCH_IDENTITY, int))
    };
}

#define MOCK_BENCH_MOCK_METHOD(z, n, d) MOCK_METHOD( f##n, MOCK_BENCH_NUMBER_OF_ARGS )
#define MOCK_BENCH_BASE_CLASS(z, n, d) \
    MOCK_BASE_CLASS( BOOST_PP_CAT( mock_class, n ), base_class ) \
    { \
        BOOST_PP_REPEAT(MOCK_BENCH_NUMBER_OF_METHODS, MOCK_BENCH_MOCK_METHOD,) \
    };

BOOST_PP_REPEAT(MOCK_BENCH_NUMBER_OF_CLASSES, MOCK_BENCH_BASE_CLASS,)

#define MOCK_BENCH_INSTANTIATION(z, n, d) mock_class##n c##n;

int main()
{
    BOOST_PP_REPEAT(MOCK_BENCH_NUMBER_OF_CLASSES, MOCK_BENCH_INSTANTIATION,)
    return 0;
}
