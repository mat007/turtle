// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_CONSTRAINT_HPP_INCLUDED
#define MOCK_CONSTRAINT_HPP_INCLUDED

#include "config.hpp"
#include "log.hpp"
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

namespace mock
{
    template< typename Constraint >
    struct constraint
    {
        constraint()
        {}
        constraint( const Constraint& c )
            : c_( c )
        {}
        Constraint c_;
    };

namespace detail
{
    template< typename Lhs, typename Rhs >
    class and_
    {
    public:
        and_( const Lhs& lhs, const Rhs& rhs )
            : lhs_( lhs )
            , rhs_( rhs )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return lhs_( actual ) && rhs_( actual );
        }
        friend std::ostream& operator<<( std::ostream& s, const and_& a )
        {
            return s << "( " << mock::format( a.lhs_ )
                << " && " << mock::format( a.rhs_ ) << " )";
        }
    private:
        Lhs lhs_;
        Rhs rhs_;
    };

    template< typename Lhs, typename Rhs >
    class or_
    {
    public:
        or_( const Lhs& lhs, const Rhs& rhs )
            : lhs_( lhs )
            , rhs_( rhs )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return lhs_( actual ) || rhs_( actual );
        }
        friend std::ostream& operator<<( std::ostream& s, const or_& o )
        {
            return s << "( " << mock::format( o.lhs_ )
                << " || " << mock::format( o.rhs_ )<< " )";
        }
    private:
        Lhs lhs_;
        Rhs rhs_;
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

    template< typename Lhs, typename Rhs >
    const constraint< detail::or_< Lhs, Rhs > >
        operator||( const constraint< Lhs >& lhs,
                    const constraint< Rhs >& rhs )
    {
        return detail::or_< Lhs, Rhs >( lhs.c_, rhs.c_ );
    }

    template< typename Lhs, typename Rhs >
    const constraint< detail::and_< Lhs, Rhs > >
        operator&&( const constraint< Lhs >& lhs,
                    const constraint< Rhs >& rhs )
    {
        return detail::and_< Lhs, Rhs >( lhs.c_, rhs.c_ );
    }

    template< typename Constraint >
    const constraint< detail::not_< Constraint > >
        operator!( const constraint< Constraint >& c )
    {
        return detail::not_< Constraint >( c.c_ );
    }
} // mock

#define MOCK_UNARY_CONSTRAINT(n, Name, Expr) \
    namespace detail \
    { \
        struct Name \
        { \
            template< typename Actual > \
            bool operator()( const Actual& actual ) const \
            { \
                return Expr; \
            } \
            friend std::ostream& operator<<( std::ostream& s, const Name& ) \
            { \
                return s << BOOST_STRINGIZE(Name); \
            } \
        }; \
    } \
    const mock::constraint< detail::Name > Name;

#define MOCK_CONSTRAINT_ASSIGN(z, n, d) \
    expected##n( e##n )

#define MOCK_CONSTRAINT_UNWRAP_REF(z, n, d) \
    boost::unwrap_ref( expected##n )

#define MOCK_CONSTRAINT_FORMAT(z, n, d) \
    BOOST_PP_IF(n, << ", " <<,) mock::format( c.expected##n )

#define MOCK_CONSTRAINT_MEMBER(z, n, d) \
    Expected_##n expected##n;

#define MOCK_NARY_CONSTRAINT(n, Name, Expr) \
    namespace detail \
    { \
        template< BOOST_PP_ENUM_PARAMS(n, typename Expected_) > \
        struct Name \
        { \
            explicit Name( \
                BOOST_PP_ENUM_BINARY_PARAMS(n, const Expected_, & e ) ) \
                : BOOST_PP_ENUM(n, MOCK_CONSTRAINT_ASSIGN, _) \
            {} \
            template< typename Actual > \
            bool operator()( const Actual& actual ) const \
            { \
                return test( actual, \
                    BOOST_PP_ENUM(n, MOCK_CONSTRAINT_UNWRAP_REF, _ ) ); \
            } \
            template< typename Actual, BOOST_PP_ENUM_PARAMS(n, typename T) > \
            bool test( const Actual& actual, \
                BOOST_PP_ENUM_BINARY_PARAMS(n, const T, & expected_ ) ) const \
            { \
                return Expr; \
            } \
            friend std::ostream& operator<<( std::ostream& s, const Name& c ) \
            { \
                return s << BOOST_STRINGIZE(Name) << "( " \
                    << BOOST_PP_REPEAT(n, MOCK_CONSTRAINT_FORMAT, _) \
                    << " )"; \
            } \
            BOOST_PP_REPEAT(n, MOCK_CONSTRAINT_MEMBER, _) \
        }; \
    } \
    template< BOOST_PP_ENUM_PARAMS(n, typename Expected_) > \
    mock::constraint< detail::Name< \
        BOOST_PP_ENUM_PARAMS(n, Expected_) > \
    > Name( BOOST_PP_ENUM_BINARY_PARAMS(n, Expected_, expected_ ) ) \
    { \
        return detail::Name< BOOST_PP_ENUM_PARAMS(n, Expected_) >( \
            BOOST_PP_ENUM_PARAMS(n, expected_ ) ); \
    }

#define MOCK_CONSTRAINT(n, Name, Expr) \
    BOOST_PP_IF(n, \
        MOCK_NARY_CONSTRAINT, \
        MOCK_UNARY_CONSTRAINT)(n, Name, Expr)

#endif // MOCK_CONSTRAINT_HPP_INCLUDED
