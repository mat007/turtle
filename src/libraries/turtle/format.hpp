//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_FORMAT_HPP_INCLUDED
#define MOCK_FORMAT_HPP_INCLUDED

#include <boost/static_assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/detail/container_fwd.hpp>
#include <ostream>
#include <string>

namespace mock
{
    struct stream : boost::noncopyable
    {
        stream( std::ostream& s )
            : s_( s )
        {
            s << std::boolalpha;
        }

#define MOCK_STREAM_OPERATOR(Type) \
        stream& operator<<( Type val ) \
        { \
            s_ << val; \
            return *this; \
        }
        MOCK_STREAM_OPERATOR( bool )
        MOCK_STREAM_OPERATOR( short )
        MOCK_STREAM_OPERATOR( unsigned short )
        MOCK_STREAM_OPERATOR( int )
        MOCK_STREAM_OPERATOR( unsigned int )
        MOCK_STREAM_OPERATOR( long )
        MOCK_STREAM_OPERATOR( unsigned long )
        MOCK_STREAM_OPERATOR( float )
        MOCK_STREAM_OPERATOR( double )
        MOCK_STREAM_OPERATOR( long double )
        MOCK_STREAM_OPERATOR( char )
        MOCK_STREAM_OPERATOR( unsigned char )
#undef MOCK_STREAM_OPERATOR

        std::ostream& s_;
    };

namespace detail
{
namespace protect
{
    template< typename S, typename T >
    char operator<<( S&, const T& );

    template< typename S, typename T >
    struct is_serializable_impl
    {
        BOOST_STATIC_ASSERT( sizeof( S& ) != sizeof( char ) );
        static S* s;
        static T* t;
        enum { value = sizeof( *s << *t ) == sizeof( S& ) };
    };

    template< typename S, typename T >
    struct is_serializable : boost::integral_constant< bool, is_serializable_impl< S, T >::value >
    {};
}

    template< typename T >
    void serialize( std::ostream& s, const T& t,
        BOOST_DEDUCED_TYPENAME boost::enable_if<
            BOOST_DEDUCED_TYPENAME protect::is_serializable< std::ostream, T > >::type* = 0 )
    {
        s << t;
    }
    template< typename T >
    void serialize( std::ostream& s, const T&,
        BOOST_DEDUCED_TYPENAME boost::disable_if<
            BOOST_DEDUCED_TYPENAME protect::is_serializable< std::ostream, T > >::type* = 0 )
    {
        s << "?";
    }

    template< typename T >
    void serialize( mock::stream& s, const T& t,
        BOOST_DEDUCED_TYPENAME boost::enable_if<
            BOOST_DEDUCED_TYPENAME protect::is_serializable< mock::stream, T > >::type* = 0 )
    {
        s << t;
    }
    template< typename T >
    void serialize( mock::stream& s, const T& t,
        BOOST_DEDUCED_TYPENAME boost::disable_if<
            BOOST_DEDUCED_TYPENAME protect::is_serializable< mock::stream, T > >::type* = 0 )
    {
        detail::serialize( s.s_, t );
    }

    template< typename T >
    struct formatter
    {
        explicit formatter( const T& t )
            : t_( &t )
        {}
        friend std::ostream& operator<<( std::ostream& s, const formatter& t )
        {
            mock::stream ms( s );
            detail::serialize( ms, *t.t_ );
            return s;
        }
        friend mock::stream& operator<<( mock::stream& s, const formatter& t )
        {
            detail::serialize( s, *t.t_ );
            return s;
        }
        const T* t_;
    };
    template<>
    struct formatter< std::string >
    {
        explicit formatter( const std::string& s )
            : s_( &s )
        {}
        friend std::ostream& operator<<( std::ostream& s, const formatter& t )
        {
            return s << '"' << *t.s_ << '"';
        }
        friend mock::stream& operator<<( mock::stream& s, const formatter& t )
        {
            s.s_ << '"' << *t.s_ << '"';
            return s;
        }
        const std::string* s_;
    };
    template<>
    struct formatter< const char* >
    {
        explicit formatter( const char* s )
            : s_( s )
        {}
        friend std::ostream& operator<<( std::ostream& s, const formatter& t )
        {
            return s << '"' << t.s_ << '"';
        }
        friend mock::stream& operator<<( mock::stream& s, const formatter& t )
        {
            s.s_ << '"' << t.s_ << '"';
            return s;
        }
        const char* s_;
    };
}

    template< typename T >
    detail::formatter< T > format( const T& t )
    {
        return detail::formatter< T >( t );
    }

    template< typename T >
    mock::stream& serialize( mock::stream& s, const T& begin, const T& end )
    {
        s << '(';
        for( T it = begin; it != end; ++it )
        {
            if( it != begin )
                s << ',';
            s << mock::format( *it );
        }
        return s << ')';
    }

    template< typename T1, typename T2 >
    mock::stream& operator<<( mock::stream& s, const std::pair< T1, T2 >& p )
    {
        return s << '(' << mock::format( p.first ) << ',' << mock::format( p.second ) << ')';
    }
    template< typename T, typename A >
    mock::stream& operator<<( mock::stream& s, const std::deque< T, A >& c )
    {
        return mock::serialize( s, c.begin(), c.end() );
    }
    template< typename T, typename A >
    mock::stream& operator<<( mock::stream& s, const std::list< T, A >& c )
    {
        return mock::serialize( s, c.begin(), c.end() );
    }
    template< typename T, typename A >
    mock::stream& operator<<( mock::stream& s, const std::vector< T, A >& c )
    {
        return mock::serialize( s, c.begin(), c.end() );
    }
    template< typename K, typename T, typename C, typename A >
    mock::stream& operator<<( mock::stream& s, const std::map< K, T, C, A >& c )
    {
        return mock::serialize( s, c.begin(), c.end() );
    }
    template< typename K, typename T, typename C, typename A >
    mock::stream& operator<<( mock::stream& s, const std::multimap< K, T, C, A >& c )
    {
        return mock::serialize( s, c.begin(), c.end() );
    }
    template< typename T, typename C, typename A >
    mock::stream& operator<<( mock::stream& s, const std::set< T, C, A >& c )
    {
        return mock::serialize( s, c.begin(), c.end() );
    }
    template< typename T, typename C, typename A >
    mock::stream& operator<<( mock::stream& s, const std::multiset< T, C, A >& c )
    {
        return mock::serialize( s, c.begin(), c.end() );
    }
}

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
    mock::stream& operator<<( mock::stream& s, const boost::assign_detail::generic_list< T >& c )
    {
        return mock::serialize( s, c.begin(), c.end() );
    }
}

#endif // #ifndef MOCK_FORMAT_HPP_INCLUDED
