// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2015
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Payload of the link regression test: a mock function defined with MOCK_FUNCTION in a
// header and included from several translation units must not cause duplicate symbols.

#ifndef MOCK_TEST_DEFINED_HPP_INCLUDED
#define MOCK_TEST_DEFINED_HPP_INCLUDED

#ifdef BOOST_AUTO_TEST_MAIN
#    undef BOOST_AUTO_TEST_MAIN
#endif

#include <turtle/mock.hpp>

MOCK_FUNCTION(f, 0, void(), f)

#endif // MOCK_TEST_DEFINED_HPP_INCLUDED
