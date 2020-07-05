// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_IS_FUNCTOR_HPP_INCLUDED
#define MOCK_IS_FUNCTOR_HPP_INCLUDED

#include "../config.hpp"
#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/utility/declval.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/make_void.hpp>
#include <type_traits>

namespace mock
{
namespace detail
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF( result_type )
    BOOST_MPL_HAS_XXX_TEMPLATE_DEF( sig )
    BOOST_MPL_HAS_XXX_TEMPLATE_DEF( result )

    template< typename F, typename P, class = void >
    struct is_callable : std::false_type
    {};
    template< typename F, typename P >
    struct is_callable< F, P, boost::void_t<decltype( std::declval<F>()( std::declval<P>() ) )> >: std::true_type
    {};

    template< typename T, typename P >
    struct is_functor
        : boost::mpl::or_<
            boost::function_types::is_callable_builtin< T >,
            is_callable< T, P >,
            has_result_type< T >,
            has_result< T >,
            has_sig< T >
        >
    {};
}
} // mock

#endif // MOCK_IS_FUNCTOR_HPP_INCLUDED
