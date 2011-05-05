//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_ROOT_HPP_INCLUDED
#define MOCK_ROOT_HPP_INCLUDED

#include "config.hpp"
#include "node.hpp"
#include <boost/test/utils/trivial_singleton.hpp>

namespace mock
{
    class root_t : public boost::unit_test::singleton< root_t >, public node
    {
    protected:
        virtual void untie()
        {}
    private:
        BOOST_TEST_SINGLETON_CONS( root_t );
    };
    BOOST_TEST_SINGLETON_INST( root )

    inline bool verify()
    {
        return root.verify();
    }
    inline void reset()
    {
        root.reset();
    }
}

#endif // MOCK_ROOT_HPP_INCLUDED
