//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_CLEANUP_HPP_INCLUDED
#define MOCK_CLEANUP_HPP_INCLUDED

#include "config.hpp"

#ifdef MOCK_USE_BOOST_TEST

#include "root.hpp"
#include <boost/test/unit_test_suite.hpp>

namespace mock
{
namespace detail
{
    struct cleanup
    {
        ~cleanup()
        {
            //mock::verify(); // $$$$ MAT : because of a bug in Boost.Test this will crash if anything needs to be logged
            mock::reset();
        }
    };
    BOOST_GLOBAL_FIXTURE( cleanup );
}
}

#endif

#endif // MOCK_CLEANUP_HPP_INCLUDED
