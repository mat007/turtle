//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_LAMBDA_HPP_INCLUDED
#define MOCK_LAMBDA_HPP_INCLUDED

#include "config.hpp"
#ifdef MOCK_USE_BOOST_BIND
#   include <boost/bind.hpp>
#else
#   include <boost/spirit/home/phoenix/statement/throw.hpp>
#   include <boost/spirit/home/phoenix/operator/self.hpp>
#   include <boost/spirit/home/phoenix/core/nothing.hpp>
#endif
#include <boost/function.hpp>

namespace mock
{
namespace detail
{

#ifdef MOCK_USE_BOOST_BIND
    template< typename Signature >
    struct lambda
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

        template< typename T >
        static functor_type make_val( T t )
        {
            return boost::bind( &do_identity< T >, t );
        }
        template< typename T >
        static functor_type make_val( boost::reference_wrapper< T > t )
        {
            return boost::bind( &do_ref_identity< T >, t.get_pointer() );
        }
        template< typename T >
        static functor_type make_throw( T t )
        {
            return boost::bind( &do_throw< T >, t );
        }
        static functor_type make_nothing()
        {
            return boost::bind( &do_nothing );
        }

        template< typename T >
        static T do_identity( T t )
        {
            return t;
        }
        template< typename T >
        static T& do_ref_identity( T* t )
        {
            return *t;
        }
        template< typename T >
        static void do_throw( T t )
        {
            throw t;
        }
        static void do_nothing()
        {
        }
    };

#else

    template< typename Signature >
    struct lambda
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

        template< typename T >
        static functor_type make_val( T t )
        {
            return boost::phoenix::val( t );
        }
        template< typename T >
        static functor_type make_val( boost::reference_wrapper< T > t )
        {
            return *boost::phoenix::val( t.get_pointer() );
        }
        template< typename T >
        static functor_type make_throw( T t )
        {
            return boost::phoenix::throw_( t );
        }
        static functor_type make_nothing()
        {
            return boost::phoenix::nothing;
        }
    };
#endif

}
}

#endif // #ifndef MOCK_LAMBDA_HPP_INCLUDED
