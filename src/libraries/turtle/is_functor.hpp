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

namespace mock
{
namespace detail
{
    typedef char true_type[2];
    typedef char false_type[1];

    template< typename T >
    true_type& has_result_type_helper( T*, BOOST_DEDUCED_TYPENAME T::result_type* = 0 );
    template< typename T >
    false_type& has_result_type_helper( T, ... );

    template< typename T >
    struct has_result_type
    {
        static T* t();
        enum { value = sizeof( has_result_type_helper( t() ) ) == sizeof( true_type ) };
    };

    template< typename T, bool B = has_result_type< T >::value >
    struct is_functor
    {
        typedef BOOST_DEDUCED_TYPENAME boost::function_types::is_callable_builtin< T >::type type;
    };
    template< typename T >
    struct is_functor< T, true >
    {
        typedef boost::true_type type;
    };
}
}

#endif // #ifndef MOCK_IS_FUNCTOR_HPP_INCLUDED
