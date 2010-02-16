//
//  Copyright Mathieu Champlon 2009
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_IS_FUNCTOR_HPP_INCLUDED
#define MOCK_IS_FUNCTOR_HPP_INCLUDED

#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/mpl/or.hpp>

namespace mock
{
namespace detail
{
    typedef boost::type_traits::yes_type yes_type;
    typedef boost::type_traits::no_type no_type;

#define MOCK_IS_FUNCTION_HELPER(N, M) \
    template< typename T > yes_type& N##_helper( BOOST_DEDUCED_TYPENAME T::M* ); \
    template< typename T > no_type& N##_helper( ... ); \
    template< typename T > struct N \
        : boost::mpl::bool_< sizeof( N##_helper< T >( 0 ) ) == sizeof( yes_type ) > \
    {};

    MOCK_IS_FUNCTION_HELPER( has_result_type, result_type )
    MOCK_IS_FUNCTION_HELPER( has_sig, BOOST_NESTED_TEMPLATE sig< void > )
    MOCK_IS_FUNCTION_HELPER( has_result, BOOST_NESTED_TEMPLATE result< void > )

#undef MOCK_IS_FUNCTION_HELPER

    template< typename T >
    struct is_functor
    {
        typedef BOOST_DEDUCED_TYPENAME boost::mpl::or_<
                    boost::function_types::is_callable_builtin< T >,
                    has_result_type< T >,
                    has_result< T >,
                    has_sig< T >
                >::type type;
    };
}
}

#endif // #ifndef MOCK_IS_FUNCTOR_HPP_INCLUDED
