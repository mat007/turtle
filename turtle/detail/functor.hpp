// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_FUNCTOR_HPP_INCLUDED
#define MOCK_FUNCTOR_HPP_INCLUDED

#include "detail/function.hpp"

namespace mock
{
namespace detail
{
    template< typename Signature >
    struct functor : function< Signature >
    {
        functor()
        {
            static functor* f = 0;
            if( f )
            {
                *this = *f;
                f = 0;
            }
            else
                f = this;
        }
    };
}
} // mock

#define MOCK_FUNCTOR(f, S) \
    mock::detail::functor< S > f, f##_mock

#endif // MOCK_FUNCTOR_HPP_INCLUDED
