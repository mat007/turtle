//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_FUNCTIONAL_HPP_INCLUDED
#define MOCK_FUNCTIONAL_HPP_INCLUDED

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace mock
{
namespace detail
{
    class any
    {
    public:
        template< typename Actual >
        bool operator()( const Actual& ) const
        {
            return true;
        }
    };

    class negate
    {
    public:
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return ! actual;
        }
    };

    class evaluate
    {
    public:
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return actual();
        }
    };

    template< typename Expected >
    class equal
    {
    public:
        explicit equal( const Expected& expected )
            : expected_( expected )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return actual == expected_;
        }
    private:
        Expected expected_;
    };

    template< typename Expected >
    class same
    {
    public:
        explicit same( const boost::reference_wrapper< Expected >& expected )
            : expected_( expected )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return &actual == expected_.get_pointer();
        }
    private:
        boost::reference_wrapper< Expected > expected_;
    };

    template< typename Expected >
    class less
    {
    public:
        explicit less( const Expected& expected )
            : expected_( expected )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return actual < expected_;
        }
    private:
        Expected expected_;
    };

    template< typename Expected >
    class greater
    {
    public:
        explicit greater( const Expected& expected )
            : expected_( expected )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return actual > expected_;
        }
    private:
        Expected expected_;
    };

    template< typename Expected >
    class assign
    {
    public:
        explicit assign( const Expected& expected )
            : expected_( expected )
        {}
        template< typename Actual >
        bool operator()( Actual& actual ) const
        {
            actual = expected_;
            return true;
        }
    private:
        Expected expected_;
    };

    template< typename Expected >
    class retrieve
    {
    public:
        explicit retrieve( const boost::reference_wrapper< Expected >& expected )
            : expected_( expected )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual,
            BOOST_DEDUCED_TYPENAME boost::disable_if<
                boost::is_convertible< const Actual*, Expected >, Actual >::type* = 0 ) const
        {
            expected_.get() = actual;
            return true;
        }
        template< typename Actual >
        bool operator()( Actual& actual,
            BOOST_DEDUCED_TYPENAME boost::enable_if<
                boost::is_convertible< Actual*, Expected >, Actual >::type* = 0 ) const
        {
            expected_.get() = &actual;
            return true;
        }
    private:
        boost::reference_wrapper< Expected > expected_;
    };

    template< typename Expected >
    class contains
    {
    public:
        explicit contains( const Expected& expected )
            : expected_( expected )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return boost::algorithm::contains( actual, expected_ );
        }
    private:
        Expected expected_;
    };
}
}

#endif // #ifndef MOCK_FUNCTIONAL_HPP_INCLUDED
