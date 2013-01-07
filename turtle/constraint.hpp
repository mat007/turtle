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
            : c_( c )
        {}
        Constraint c_;
    };

namespace detail
{
    template< typename Constraint1, typename Constraint2 >
    class and_
    {
    public:
        and_( const Constraint1& c1, const Constraint2& c2 )
            : c1_( c1 )
            , c2_( c2 )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return c1_( actual ) && c2_( actual );
        }
        friend std::ostream& operator<<( std::ostream& s, const and_& a )
        {
            return s << "( " << mock::format( a.c1_ )
                << " && " << mock::format( a.c2_ ) << " )";
        }
    private:
        Constraint1 c1_;
        Constraint2 c2_;
    };

    template< typename Constraint1, typename Constraint2 >
    class or_
    {
    public:
        or_( const Constraint1& c1, const Constraint2& c2 )
            : c1_( c1 )
            , c2_( c2 )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return c1_( actual ) || c2_( actual );
        }
        friend std::ostream& operator<<( std::ostream& s, const or_& o )
        {
            return s << "( " << mock::format( o.c1_ )
                << " || " << mock::format( o.c2_ )<< " )";
        }
    private:
        Constraint1 c1_;
        Constraint2 c2_;
    };

    template< typename Constraint >
    class not_
    {
    public:
        explicit not_( const Constraint& c )
            : c_( c )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return ! c_( actual );
        }
        friend std::ostream& operator<<( std::ostream& s, const not_& n )
        {
            return s << "! " << mock::format( n.c_ );
        }
    private:
        Constraint c_;
    };
}

    template< typename Constraint1, typename Constraint2 >
    const constraint< detail::or_< Constraint1, Constraint2 > >
        operator||( const constraint< Constraint1 >& lhs,
                    const constraint< Constraint2 >& rhs )
    {
        return detail::or_< Constraint1, Constraint2 >( lhs.c_, rhs.c_ );
    }

    template< typename Constraint1, typename Constraint2 >
    const constraint< detail::and_< Constraint1, Constraint2 > >
        operator&&( const constraint< Constraint1 >& lhs,
                    const constraint< Constraint2 >& rhs )
    {
        return detail::and_< Constraint1, Constraint2 >( lhs.c_, rhs.c_ );
    }

    template< typename Constraint >
    const constraint< detail::not_< Constraint > >
        operator!( const constraint< Constraint >& c )
    {
        return detail::not_< Constraint >( c.c_ );
    }
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
        detail::N c_; \
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
