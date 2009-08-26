//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_FORMAT_HPP_INCLUDED
#define MOCK_FORMAT_HPP_INCLUDED

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <sstream>
#include <ostream>

namespace mock
{
namespace detail
{
    struct eater
    {
        template< typename T >
        eater( const T& ) {}
    };

    struct eaten
    {};

    template< typename S >
    eaten operator<<( S& s, const eater& );

    template< int I >
    struct selector
    {
    };
    template<>
    struct selector< sizeof( std::ostream& ) >
    {
        typedef boost::true_type type;
    };
    template<>
    struct selector< sizeof( eaten ) >
    {
        typedef boost::false_type type;
    };

    template< typename T >
    struct is_serializable
    {
        static std::ostream& s();
        static const T& t();

        typedef BOOST_DEDUCED_TYPENAME
            selector< sizeof( s() << t() ) >::type type;
    };

    template< typename T >
    std::string format( const T& t,
        BOOST_DEDUCED_TYPENAME boost::enable_if<
            BOOST_DEDUCED_TYPENAME detail::is_serializable< T >::type >::type* = 0 )
    {
        std::stringstream s;
        static_cast< std::ostream& >( s ) << std::boolalpha << t;
        return s.str();
    }
    template< typename T >
    std::string format( const T&,
        BOOST_DEDUCED_TYPENAME boost::disable_if<
            BOOST_DEDUCED_TYPENAME detail::is_serializable< T >::type >::type* = 0 )
    {
        return "?";
    }
}
}

#endif // #ifndef MOCK_FORMAT_HPP_INCLUDED
