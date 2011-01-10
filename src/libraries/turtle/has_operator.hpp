//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_HAS_OPERATOR_HPP_INCLUDED
#define MOCK_HAS_OPERATOR_HPP_INCLUDED

#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

namespace mock
{
namespace detail
{
    typedef boost::type_traits::yes_type yes_type;
    typedef boost::type_traits::no_type no_type;

    template< typename T, typename Actual, bool(T::*)( Actual ) const > struct has_operator_helper_class {};

    template< typename T, typename Actual > yes_type& has_operator_helper( has_operator_helper_class< T, Actual, &T::operator() >* );
    template< typename T, typename Actual > no_type& has_operator_helper( ... );

    template< typename T, typename Actual > struct has_operator
        : boost::mpl::bool_< sizeof( has_operator_helper< T, Actual >( 0 ) ) == sizeof( yes_type ) >
    {};
}
}

#endif // #ifndef MOCK_HAS_OPERATOR_HPP_INCLUDED
