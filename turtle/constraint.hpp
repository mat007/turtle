// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_CONSTRAINT_HPP_INCLUDED
#define MOCK_CONSTRAINT_HPP_INCLUDED

#include <boost/preprocessor/stringize.hpp>
#include "log.hpp"

namespace mock
{
    template< typename Constraint >
    struct constraint
    {
        constraint( const Constraint& c )
            : f_( c )
        {}
        Constraint f_;
    };
} // mock

#define MOCK_UNARY_CONSTRAINT(N,Expr) \
    namespace detail \
    { \
        struct N \
        { \
            template< typename Actual > \
            bool operator()( const Actual& actual ) const \
            { \
                return Expr; \
            } \
            friend std::ostream& operator<<( std::ostream& s, const N& ) \
            { \
                return s << BOOST_STRINGIZE(N); \
            } \
        }; \
    } \
    template<> \
    struct constraint< detail::N > \
    { \
        constraint() \
        {} \
        detail::N f_; \
    }; \
    const constraint< detail::N > N;

#define MOCK_BINARY_CONSTRAINT(N,Expr) \
    namespace detail \
    { \
        template< typename Expected > \
        struct N \
        { \
            explicit N( const Expected& expected ) \
                : expected_( expected ) \
            {} \
            template< typename Actual > \
            bool operator()( const Actual& actual ) const \
            { \
                return Expr; \
            } \
            friend std::ostream& operator<<( std::ostream& s, const N& n ) \
            { \
                return s << BOOST_STRINGIZE(N) \
                    << "( " << mock::format( n.expected_ ) << " )"; \
            } \
            Expected expected_; \
        }; \
    } \
    template< typename T > \
    constraint< detail::N< T > > N( T t ) \
    { \
        return detail::N< T >( t ); \
    }

#endif // MOCK_CONSTRAINT_HPP_INCLUDED
