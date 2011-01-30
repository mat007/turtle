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
#include <boost/utility/enable_if.hpp>
#include <boost/exception/detail/is_output_streamable.hpp>
#ifndef MOCK_NO_STL_FORMAT
#include <boost/detail/container_fwd.hpp>
#endif // MOCK_NO_STL_FORMAT
#include <sstream>
#include <ostream>

namespace mock
{
namespace detail
{
    template< typename T >
    std::string serialize( const T& t,
        BOOST_DEDUCED_TYPENAME boost::enable_if<
            BOOST_DEDUCED_TYPENAME boost::is_output_streamable< T > >::type* = 0 )
    {
        std::stringstream s;
        s << std::boolalpha << t;
        return s.str();
    }
    template< typename T >
    std::string serialize( const T&,
        BOOST_DEDUCED_TYPENAME boost::disable_if<
            BOOST_DEDUCED_TYPENAME boost::is_output_streamable< T > >::type* = 0 )
    {
        return "?";
    }
}

    template< typename T >
    std::string format( const T& t )
    {
        return detail::serialize( t );
    }

    inline std::string format( const char* s )
    {
        return '"' + std::string( s ) + '"';
    }

#ifndef MOCK_NO_STL_FORMAT
    inline std::string format( const std::string& s )
    {
        return '"' + s + '"';
    }
    template< typename T1, typename T2 >
    inline std::string format( const std::pair< T1, T2 >& p )
    {
        return '(' + mock::format( p.first ) + ',' + mock::format( p.second ) + ')';
    }
    template< typename T >
    std::string format( const T& begin, const T& end )
    {
        std::stringstream s;
        s << '(';
        for( T it = begin; it != end; ++it )
            s << (it == begin ? "" : ",") << mock::format( *it );
        s << ')';
        return s.str();
    }
    template< typename T, typename A >
    std::string format( const std::deque< T, A >& d )
    {
        return format( d.begin(), d.end() );
    }
    template< typename T, typename A >
    std::string format( const std::list< T, A >& l )
    {
        return format( l.begin(), l.end() );
    }
    template< typename T, typename A >
    std::string format( const std::vector< T, A >& v )
    {
        return format( v.begin(), v.end() );
    }
    template< typename K, typename T, typename C, typename A >
    std::string format( const std::map< K, T, C, A >& m )
    {
        return format( m.begin(), m.end() );
    }
    template< typename K, typename T, typename C, typename A >
    std::string format( const std::multimap< K, T, C, A >& m )
    {
        return format( m.begin(), m.end() );
    }
    template< typename T, typename C, typename A >
    std::string format( const std::set< T, C, A >& s )
    {
        return format( s.begin(), s.end() );
    }
    template< typename T, typename C, typename A >
    std::string format( const std::multiset< T, C, A >& s )
    {
        return format( s.begin(), s.end() );
    }
#endif // MOCK_NO_STL_FORMAT
}

#ifndef MOCK_NO_BOOST_FORMAT
namespace boost
{
namespace assign_detail
{
    template< typename T > class generic_list;
}
}
namespace mock
{
    template< typename T >
    std::string format( const boost::assign_detail::generic_list< T >& l )
    {
        return format( l.begin(), l.end() );
    }
}
#endif // MOCK_NO_BOOST_FORMAT

#endif // #ifndef MOCK_FORMAT_HPP_INCLUDED
