// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_CONSTRAINTS_HPP_INCLUDED
#define MOCK_CONSTRAINTS_HPP_INCLUDED

#include "constraint.hpp"
#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace mock
{
    MOCK_UNARY_CONSTRAINT( any, true && &actual )
    MOCK_UNARY_CONSTRAINT( affirm, !! actual )
    MOCK_UNARY_CONSTRAINT( negate, ! actual )
    MOCK_UNARY_CONSTRAINT( evaluate, actual() )

    MOCK_BINARY_CONSTRAINT( equal, actual == boost::unwrap_ref( expected_ ) )
    MOCK_BINARY_CONSTRAINT( less, actual < boost::unwrap_ref( expected_ ) )
    MOCK_BINARY_CONSTRAINT( greater, actual > boost::unwrap_ref( expected_ ) )
    MOCK_BINARY_CONSTRAINT(
        less_equal, actual <= boost::unwrap_ref( expected_ ) )
    MOCK_BINARY_CONSTRAINT(
        greater_equal, actual >= boost::unwrap_ref( expected_ ) )

namespace detail
{
    template< typename Expected >
    struct same
    {
        explicit same( const Expected& expected )
            : expected_( boost::addressof( boost::unwrap_ref( expected ) ) )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return boost::addressof( actual ) == expected_;
        }
        friend std::ostream& operator<<( std::ostream& os, const same& s )
        {
            return os << "same( " << mock::format( *s.expected_ ) << " )";
        }
        const BOOST_DEDUCED_TYPENAME
            boost::unwrap_reference< Expected >::type* expected_;
    };

    template< typename Expected >
    struct retrieve
    {
        explicit retrieve( Expected& expected )
            : expected_( boost::addressof( boost::unwrap_ref( expected ) ) )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual,
            BOOST_DEDUCED_TYPENAME boost::disable_if<
                boost::is_convertible<
                    const Actual*,
                    BOOST_DEDUCED_TYPENAME
                        boost::unwrap_reference< Expected >::type
                >
            >::type* = 0 ) const
        {
            *expected_ = actual;
            return true;
        }
        template< typename Actual >
        bool operator()( Actual& actual,
            BOOST_DEDUCED_TYPENAME boost::enable_if<
                boost::is_convertible< Actual*,
                    BOOST_DEDUCED_TYPENAME
                        boost::unwrap_reference< Expected >::type
                >
            >::type* = 0 ) const
        {
            *expected_ = boost::addressof( actual );
            return true;
        }
        friend std::ostream& operator<<( std::ostream& s, const retrieve& r )
        {
            return s << "retrieve( " << mock::format( *r.expected_ ) << " )";
        }
        BOOST_DEDUCED_TYPENAME
            boost::unwrap_reference< Expected >::type* expected_;
    };

    template< typename Expected >
    struct assign
    {
        explicit assign( const Expected& expected )
            : expected_( expected )
        {}
        template< typename Actual >
        bool operator()( Actual& actual ) const
        {
            actual = boost::unwrap_ref( expected_ );
            return true;
        }
        template< typename Actual >
        bool operator()( Actual* actual,
            BOOST_DEDUCED_TYPENAME boost::enable_if<
                boost::is_convertible<
                    BOOST_DEDUCED_TYPENAME
                        boost::unwrap_reference< Expected >::type,
                    Actual
                >
            >::type* = 0 ) const
        {
            *actual = boost::unwrap_ref( expected_ );
            return true;
        }
        friend std::ostream& operator<<( std::ostream& s, const assign& a )
        {
            return s << "assign( " << mock::format( a.expected_ ) << " )";
        }
        Expected expected_;
    };

    template< typename Expected >
    struct contain
    {
        explicit contain( const Expected& expected )
            : expected_( expected )
        {}
        bool operator()( const std::string& actual ) const
        {
            return actual.find( boost::unwrap_ref( expected_ ) )
                != std::string::npos;
        }
        friend std::ostream& operator<<( std::ostream& s, const contain& n )
        {
            return s << "contain( " << mock::format( n.expected_ ) << " )";
        }
        Expected expected_;
    };
}

    template< typename T >
    constraint< detail::same< T > > same( T& t )
    {
        return detail::same< T >( t );
    }
    template< typename T >
    constraint< detail::retrieve< T > > retrieve( T& t )
    {
        return detail::retrieve< T >( t );
    }
    template< typename T >
    constraint< detail::assign< T > > assign( T t )
    {
        return detail::assign< T >( t );
    }
    template< typename T >
    constraint< detail::contain< T > > contain( T t )
    {
        return detail::contain< T >( t );
    }

    template< typename T >
    constraint< T > call( T t )
    {
        return constraint< T >( t );
    }
} // mock

#endif // MOCK_CONSTRAINTS_HPP_INCLUDED
