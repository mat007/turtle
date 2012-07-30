// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2011
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_CLEANUP_HPP_INCLUDED
#define MOCK_CLEANUP_HPP_INCLUDED

#include "../config.hpp"
#ifdef MOCK_USE_BOOST_TEST
#include "../verify.hpp"
#include "../reset.hpp"
#include <boost/test/unit_test_suite.hpp>

namespace mock
{
namespace detail
{
    struct cleanup
    {
        ~cleanup()
        {
            // see https://svn.boost.org/trac/boost/ticket/5563
            //mock::verify();
            mock::reset();
        }
    };
    BOOST_GLOBAL_FIXTURE( cleanup );
}
} // mock

#endif // MOCK_USE_BOOST_TEST

#endif // MOCK_CLEANUP_HPP_INCLUDED
