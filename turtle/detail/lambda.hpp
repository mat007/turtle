// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2011
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_LAMBDA_HPP_INCLUDED
#define MOCK_LAMBDA_HPP_INCLUDED

#include "../config.hpp"
#ifdef MOCK_USE_BOOST_PHOENIX
#include <boost/spirit/home/phoenix/bind.hpp>
#else
#include <boost/bind.hpp>
#endif
#include <boost/function.hpp>

namespace mock
{
namespace detail
{
#ifdef MOCK_USE_BOOST_PHOENIX
    using boost::phoenix::bind;
#else
    using boost::bind;
#endif

    template< typename Result, typename Signature >
    struct lambda
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

        template< typename T >
        static functor_type make_val( T t )
        {
            return detail::bind( &do_identity< T >, t );
        }
        template< typename T >
        static functor_type make_val( boost::reference_wrapper< T > t )
        {
            return detail::bind(
                &do_ref_identity< T >, t.get_pointer() );
        }
        template< typename T >
        static functor_type make_throw( T t )
        {
            return detail::bind( &do_throw< T >, t );
        }
        static functor_type make_nothing()
        {
            return detail::bind( &do_nothing );
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
        static Result do_throw( T t )
        {
            throw t;
        }
        static void do_nothing()
        {}
    };
}
} // mock

#endif // MOCK_LAMBDA_HPP_INCLUDED
