// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_CONSTRAINTS_HPP_INCLUDED
#define MOCK_CONSTRAINTS_HPP_INCLUDED

#include "config.hpp"
#include "constraint.hpp"
#include "detail/addressof.hpp"
#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/test/floating_point_comparison.hpp>

namespace mock
{
    MOCK_CONSTRAINT( 0, any, true && &actual )
    MOCK_CONSTRAINT( 0, affirm, !! actual )
    MOCK_CONSTRAINT( 0, negate, ! actual )
    MOCK_CONSTRAINT( 0, evaluate, actual() )

    MOCK_CONSTRAINT( 1, equal, actual == expected_0 )
    MOCK_CONSTRAINT( 1, less, actual < expected_0 )
    MOCK_CONSTRAINT( 1, greater, actual > expected_0 )
    MOCK_CONSTRAINT( 1, less_equal, actual <= expected_0 )
    MOCK_CONSTRAINT( 1, greater_equal, actual >= expected_0 )

    MOCK_CONSTRAINT( 1, small, \
        ( boost::test_tools::check_is_small( actual, expected_0 ) ) )
    MOCK_CONSTRAINT( 2, close, \
        ( boost::test_tools::check_is_close( \
            actual, expected_0, \
            boost::test_tools::percent_tolerance( expected_1 ) ) ) )
    MOCK_CONSTRAINT( 2, close_fraction, \
        ( boost::test_tools::check_is_close( \
            actual, expected_0, \
            boost::test_tools::fraction_tolerance( expected_1 ) ) ) )
    MOCK_CONSTRAINT( 2, near, std::abs( actual - expected_0 ) < expected_1 )

namespace detail
{
    template< typename Expected >
    struct same
    {
        explicit same( const Expected& expected )
            : expected_( detail::addressof( boost::unwrap_ref( expected ) ) )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return detail::addressof( actual ) == expected_;
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
            : expected_( detail::addressof( boost::unwrap_ref( expected ) ) )
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
            *expected_ = detail::addressof( actual );
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
