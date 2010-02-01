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
#include <boost/mpl/or.hpp>

namespace mock
{
namespace detail
{
    typedef char true_type;
    struct false_type
    {
       true_type padding[2];
    };

    template< typename T >
    true_type& has_result_type_tester( T*, BOOST_DEDUCED_TYPENAME T::result_type* = 0 );
    template< typename T >
    false_type& has_result_type_tester( T, ... );

    template< typename T >
    struct has_result_type_impl
    {
        static T* t;
        enum { value = sizeof( has_result_type_tester( t ) ) == sizeof( true_type ) };
    };

    template< typename T >
    struct has_result_type : boost::integral_constant< bool, has_result_type_impl< T >::value >
    {
    };

    template< typename T >
    true_type& has_sig_tester( T*, BOOST_DEDUCED_TYPENAME T::template sig< void >* = 0 );
    template< typename T >
    false_type& has_sig_tester( T, ... );

    template< typename T >
    struct has_sig_impl
    {
        static T* t;
        enum { value = sizeof( has_sig_tester( t ) ) == sizeof( true_type ) };
    };

    template< typename T >
    struct has_sig : boost::integral_constant< bool, has_sig_impl< T >::value >
    {
    };

    template< typename T >
    struct is_functor
    {
        typedef BOOST_DEDUCED_TYPENAME boost::mpl::or_<
                    BOOST_DEDUCED_TYPENAME boost::mpl::or_<
                        BOOST_DEDUCED_TYPENAME boost::function_types::is_callable_builtin< T >::type,
                        BOOST_DEDUCED_TYPENAME has_result_type< T >::type
                    >::type,
                    BOOST_DEDUCED_TYPENAME has_sig< T >::type
                >::type type;
    };
}
}

#endif // #ifndef MOCK_IS_FUNCTOR_HPP_INCLUDED
