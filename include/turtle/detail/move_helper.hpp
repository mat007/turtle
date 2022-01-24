// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_MOVE_HELPER_HPP_INCLUDED
#define MOCK_MOVE_HELPER_HPP_INCLUDED

#include <type_traits>

namespace mock { namespace detail {
    template<typename T>
    using ref_arg = std::conditional<std::is_reference<T>::value, T, std::add_rvalue_reference_t<T>>;
}} // namespace mock::detail

#endif // MOCK_MOVE_HELPER_HPP_INCLUDED
