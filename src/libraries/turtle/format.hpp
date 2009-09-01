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
#include <boost/type_traits/integral_constant.hpp>
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
    eaten operator<<( S&, const eater& );

    template< typename T >
    struct is_serializable_impl
    {
        static std::ostream* s;
        static T* t;
        enum { value = sizeof( *s << *t ) == sizeof( std::ostream& ) };
    };

    template< typename T >
    struct is_serializable : boost::integral_constant< bool, is_serializable_impl< T >::value >
    {};

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

    template< typename T >
    std::string format( const T& t )
    {
        return detail::format( t );
    }
    inline std::string format( const std::string& s )
    {
        return '"' + s + '"';
    }
    inline std::string format( const char* s )
    {
        return '"' + std::string( s ) + '"';
    }
}

#endif // #ifndef MOCK_FORMAT_HPP_INCLUDED
