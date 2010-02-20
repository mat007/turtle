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
    class nothing
    {
    public:
        template< typename Y >
        bool operator()( const Y& ) const
        {
            return true;
        }
    };

    class negation
    {
    public:
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return ! y;
        }
    };

    class evaluation
    {
    public:
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return y();
        }
    };

    template< typename T >
    class equality
    {
    public:
        explicit equality( const T& t )
            : t_( t )
        {}
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return y == t_;
        }
    private:
        T t_;
    };

    template< typename T >
    class identity
    {
    public:
        explicit identity( const boost::reference_wrapper< T >& t )
            : t_( t )
        {}
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return &y == t_.get_pointer();
        }
    private:
        boost::reference_wrapper< T > t_;
    };

    template< typename T >
    class inferiority
    {
    public:
        explicit inferiority( const T& t )
            : t_( t )
        {}
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return y < t_;
        }
    private:
        T t_;
    };

    template< typename T >
    class superiority
    {
    public:
        explicit superiority( const T& t )
            : t_( t )
        {}
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return y > t_;
        }
    private:
        T t_;
    };

    template< typename T >
    class assignment
    {
    public:
        explicit assignment( const T& t )
            : t_( t )
        {}
        template< typename Y >
        bool operator()( Y& y ) const
        {
            y = t_;
            return true;
        }
    private:
        T t_;
    };

    template< typename T >
    class retrieval
    {
    public:
        explicit retrieval( const boost::reference_wrapper< T >& t )
            : t_( t )
        {}
        template< typename Y >
        bool operator()( const Y& y,
            BOOST_DEDUCED_TYPENAME boost::disable_if<
                boost::is_convertible< const Y*, T >, Y >::type* = 0 ) const
        {
            t_.get() = y;
            return true;
        }
        template< typename Y >
        bool operator()( Y& y,
            BOOST_DEDUCED_TYPENAME boost::enable_if<
                boost::is_convertible< Y*, T >, Y >::type* = 0 ) const
        {
            t_.get() = &y;
            return true;
        }
    private:
        boost::reference_wrapper< T > t_;
    };

    template< typename T >
    class container
    {
    public:
        explicit container( const T& t )
            : t_( t )
        {}
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return boost::algorithm::contains( y, t_ );
        }
    private:
        T t_;
    };
}
}

#endif // #ifndef MOCK_FUNCTIONAL_HPP_INCLUDED
