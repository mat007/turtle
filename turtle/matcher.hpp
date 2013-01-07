// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_MATCHER_HPP_INCLUDED
#define MOCK_MATCHER_HPP_INCLUDED

#include "log.hpp"
#include "constraint.hpp"
#include "detail/is_functor.hpp"
#include "detail/matcher_base.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/ref.hpp>
#include <cstring>

namespace mock
{
    template< typename Actual, typename Expected, typename Enable = void >
    class matcher : public detail::matcher_base< Actual >
    {
    public:
        explicit matcher( Expected expected )
            : expected_( expected )
        {}
        virtual bool operator()( Actual actual )
        {
            return actual == boost::unwrap_ref( expected_ );
        }
    private:
        virtual void serialize( std::ostream& s ) const
        {
            s << mock::format( expected_ );
        }
    private:
        Expected expected_;
    };

    template<>
    class matcher< const char*, const char* >
        : public detail::matcher_base< const char* >
    {
    public:
        explicit matcher( const char* expected )
            : expected_( expected )
        {}
        virtual bool operator()( const char* actual )
        {
            return std::strcmp( actual, expected_ ) == 0;
        }
    private:
        virtual void serialize( std::ostream& s ) const
        {
            s << mock::format( expected_ );
        }
    private:
        const char* expected_;
    };

    template< typename Actual, typename Constraint >
    class matcher< Actual, mock::constraint< Constraint > >
        : public detail::matcher_base< Actual >
    {
    public:
        explicit matcher( const constraint< Constraint >& c )
            : c_( c.c_ )
        {}
        virtual bool operator()( Actual actual )
        {
            return c_( actual );
        }
    private:
        virtual void serialize( std::ostream& s ) const
        {
            s << mock::format( c_ );
        }
    private:
        Constraint c_;
    };

    template< typename Actual, typename Functor >
    class matcher< Actual, Functor,
        BOOST_DEDUCED_TYPENAME boost::enable_if<
            detail::is_functor< Functor >
        >::type
    > : public detail::matcher_base< Actual >
    {
    public:
        explicit matcher( const Functor& f )
            : c_( f )
        {}
        virtual bool operator()( Actual actual )
        {
            return c_( actual );
        }
    private:
        virtual void serialize( std::ostream& s ) const
        {
            s << mock::format( c_ );
        }
    private:
        Functor c_;
    };
} // mock

#endif // MOCK_MATCHER_HPP_INCLUDED
