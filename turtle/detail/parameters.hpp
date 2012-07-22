// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2010
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_PARAMETERS_HPP_INCLUDED
#define MOCK_PARAMETERS_HPP_INCLUDED

#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/mpl/at.hpp>

namespace mock
{
namespace detail
{
    template< typename Signature, int Arity >
    struct parameters
    {
        BOOST_MPL_ASSERT_RELATION( Arity, ==,
            boost::function_types::function_arity< Signature >::value );

        template< int n >
        struct at
        {
            typedef BOOST_DEDUCED_TYPENAME
                boost::mpl::at_c<
                    BOOST_DEDUCED_TYPENAME
                        boost::function_types::parameter_types< Signature >,
                    n
                >::type type;
        };
    };
}
} // mock

#endif // MOCK_PARAMETERS_HPP_INCLUDED
