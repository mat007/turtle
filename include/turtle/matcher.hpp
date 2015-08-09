// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_MATCHER_HPP_INCLUDED
#define MOCK_MATCHER_HPP_INCLUDED

#include "config.hpp"
#include "log.hpp"
#include "constraint.hpp"
#include "detail/is_functor.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/ref.hpp>
#include <cstring>

namespace mock
{
    template< typename Actual, typename Expected, typename Enable = void >
    class matcher
    {
    public:
        explicit matcher( Expected expected )
            : expected_( expected )
        {}
        bool operator()( Actual actual )
        {
            return actual == boost::unwrap_ref( expected_ );
        }
        friend std::ostream& operator<<(
            std::ostream& s, const matcher& m )
        {
            return s << mock::format( m.expected_ );
        }
    private:
        Expected expected_;
    };

    template<>
    class matcher< const char*, const char* >
    {
    public:
        explicit matcher( const char* expected )
            : expected_( expected )
        {}
        bool operator()( const char* actual )
        {
            return actual == expected_
                || actual && expected_
                    && std::strcmp( actual, expected_ ) == 0;
        }
        friend std::ostream& operator<<(
            std::ostream& s, const matcher& m )
        {
            return s << mock::format( m.expected_ );
        }
    private:
        const char* expected_;
    };

    template< typename Actual, typename Constraint >
    class matcher< Actual, mock::constraint< Constraint > >
    {
    public:
        explicit matcher( const constraint< Constraint >& c )
            : c_( c.c_ )
        {}
        bool operator()( Actual actual )
        {
            return c_( actual );
        }
        friend std::ostream& operator<<(
            std::ostream& s, const matcher& m )
        {
            return s << mock::format( m.c_ );
        }
    private:
        Constraint c_;
    };

    template< typename Actual, typename Functor >
    class matcher< Actual, Functor,
        typename boost::enable_if<
            detail::is_functor< Functor, Actual >
        >::type
    >
    {
    public:
        explicit matcher( const Functor& f )
            : c_( f )
        {}
        bool operator()( Actual actual )
        {
            return c_( actual );
        }
        friend std::ostream& operator<<(
            std::ostream& s, const matcher& m )
        {
            return s << mock::format( m.c_ );
        }
    private:
        Functor c_;
    };
} // mock

#endif // MOCK_MATCHER_HPP_INCLUDED
