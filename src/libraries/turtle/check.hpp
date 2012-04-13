//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

#ifndef MOCK_CHECK_HPP_INCLUDED
#define MOCK_CHECK_HPP_INCLUDED

#include "is_functor.hpp"
#include "operators.hpp"
#include "log.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ref.hpp>

namespace mock
{
namespace detail
{
    template< typename Actual >
    class check_base : boost::noncopyable
    {
    public:
        virtual ~check_base() {}

        virtual bool operator()( Actual ) = 0;

        friend std::ostream& operator<<( std::ostream& s, const check_base& c )
        {
            c.serialize( s );
            return s;
        }
    private:
        virtual void serialize( std::ostream& ) const = 0;
    };

    template< typename Actual, typename Expected, typename Enable = void >
    class check : public check_base< Actual >
    {
    public:
        explicit check( Expected expected )
            : expected_( expected )
        {}
    private:
        virtual bool operator()( Actual actual )
        {
            return actual == boost::unwrap_ref( expected_ );
        }
        virtual void serialize( std::ostream& s ) const
        {
            s << mock::format( expected_ );
        }
    private:
        Expected expected_;
    };

    template< typename Actual, typename Constraint >
    class check< Actual, mock::constraint< Constraint > >
        : public check_base< Actual >
    {
    public:
        explicit check( const constraint< Constraint >& c )
            : c_( c.f_ )
        {}
    private:
        virtual bool operator()( Actual actual )
        {
            return c_( actual );
        }
        virtual void serialize( std::ostream& s ) const
        {
            s << mock::format( c_ );
        }
    private:
        Constraint c_;
    };

    template< typename Actual, typename Functor >
    class check< Actual, Functor,
        BOOST_DEDUCED_TYPENAME boost::enable_if<
            detail::is_functor< Functor >
        >::type
    > : public check_base< Actual >
    {
    public:
        explicit check( const Functor& f )
            : f_( f )
        {}
    private:
        virtual bool operator()( Actual actual )
        {
            return f_( actual );
        }
        virtual void serialize( std::ostream& s ) const
        {
            s << mock::format( f_ );
        }
    private:
        Functor f_;
    };
}
}

#endif // MOCK_CHECK_HPP_INCLUDED
