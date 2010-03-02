//
//  Copyright Mathieu Champlon 2009
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_SEQUENCEABLE_HPP_INCLUDED
#define MOCK_SEQUENCEABLE_HPP_INCLUDED

namespace mock
{
    class sequence;

namespace detail
{
    class sequenceable
    {
    public:
        sequenceable() {}
        virtual ~sequenceable() {}

        virtual void remove( sequence& s ) = 0;
    };
}

}

#endif // #ifndef MOCK_SEQUENCEABLE_HPP_INCLUDED
