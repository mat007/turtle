// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_SEQUENCE_HPP_INCLUDED
#define MOCK_SEQUENCE_HPP_INCLUDED

#include "detail/sequence_impl.hpp"
#include <boost/shared_ptr.hpp>

namespace mock
{
    class sequence
    {
    public:
        sequence()
            : impl_( new detail::sequence_impl() )
        {}

        boost::shared_ptr< detail::sequence_impl > impl_;
    };
} // mock

#endif // MOCK_SEQUENCE_HPP_INCLUDED
