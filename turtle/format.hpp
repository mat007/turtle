// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2011
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_FORMAT_HPP_INCLUDED
#define MOCK_FORMAT_HPP_INCLUDED

#include "stream.hpp"
#include <boost/utility/addressof.hpp>

namespace mock
{
namespace detail
{
    template< typename T >
    struct formatter
    {
        explicit formatter( const T& t )
            : t_( boost::addressof( t ) )
        {}
        void serialize( stream& s ) const
        {
            mock::detail::serialize( s, *t_ );
        }
        const T* t_;
    };

    template< typename T >
    stream& operator<<( stream& s, const formatter< T >& f )
    {
        f.serialize( s );
        return s;
    }

    template< typename T >
    std::ostream& operator<<( std::ostream& s, const formatter< T >& f )
    {
        stream ss( s );
        f.serialize( ss );
        return s;
    }
}

    template< typename T >
    detail::formatter< T > format( const T& t )
    {
        return detail::formatter< T >( t );
    }

} // mock

#endif // MOCK_FORMAT_HPP_INCLUDED
