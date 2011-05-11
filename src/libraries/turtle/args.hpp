//
//  Copyright Mathieu Champlon 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_ARGS_HPP_INCLUDED
#define MOCK_ARGS_HPP_INCLUDED

#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/mpl/at.hpp>

namespace mock
{
namespace detail
{
    struct invalid_type
    {
    private:
        invalid_type();
    };

    template< typename S, int n, bool B >
    struct arg_imp
    {
        typedef invalid_type type;
    };
    template< typename S, int n >
    struct arg_imp< S, n, true >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::mpl::at_c<
                BOOST_DEDUCED_TYPENAME
                    boost::function_types::parameter_types< S >,
                n - 1
            >::type type;
    };
    template< typename S, int n, int N >
    struct arg :
        arg_imp< S, n, boost::function_types::function_arity< S >::value == N >
    {
        BOOST_MPL_ASSERT_RELATION( n, >, 0 );
        BOOST_MPL_ASSERT_RELATION( n, <=, N );
    };
}
}

#define MOCK_ARG(N, n, S, tpn) \
    BOOST_PP_COMMA_IF(n) tpn mock::detail::arg< S, BOOST_PP_INC(n), N >::type p##n
#define MOCK_ARG_PROXY(z, n, d) \
    MOCK_ARG( \
        BOOST_PP_ARRAY_ELEM(0, d), \
        n, \
        BOOST_PP_ARRAY_ELEM(1, d), \
        BOOST_PP_ARRAY_ELEM(2, d) )

#define MOCK_ARGS(n, S, tpn) \
    BOOST_PP_REPEAT(n, MOCK_ARG_PROXY, (3, (n, S, tpn)))

#define MOCK_DECL(M, n, S, c, tpn) \
    tpn boost::function_types::result_type< S >::type M( \
        MOCK_ARGS(n, S, tpn) ) c

#endif // MOCK_ARGS_HPP_INCLUDED
