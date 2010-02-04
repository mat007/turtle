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

#define MOCK_IS_FUNCTION_HELPER(name, check) \
    template< typename T > yes_type& name##_helper( T*, BOOST_DEDUCED_TYPENAME T::check* = 0 ); \
    template< typename T > no_type& name##_helper( T, ... ); \
    template< typename T > struct name \
    { \
        static T* t; \
        enum { value = sizeof( name##_helper( t ) ) == sizeof( yes_type ) }; \
    };

    MOCK_IS_FUNCTION_HELPER( has_result_type, result_type )
    MOCK_IS_FUNCTION_HELPER( has_sig, sig< void > )
    MOCK_IS_FUNCTION_HELPER( has_result, result< void > )

#undef MOCK_IS_FUNCTION_HELPER

    template< typename T >
    struct is_functor
    {
        typedef BOOST_DEDUCED_TYPENAME boost::mpl::or_<
                    BOOST_DEDUCED_TYPENAME boost::function_types::is_callable_builtin< T >::type,
                    BOOST_DEDUCED_TYPENAME boost::integral_constant< bool, has_result_type< T >::value >::type,
                    BOOST_DEDUCED_TYPENAME boost::integral_constant< bool, has_result< T >::value >::type,
                    BOOST_DEDUCED_TYPENAME boost::integral_constant< bool, has_sig< T >::value >::type
                >::type type;
    };
}
}

#endif // #ifndef MOCK_IS_FUNCTOR_HPP_INCLUDED
