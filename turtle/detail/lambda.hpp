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
#include <boost/bind.hpp>
#include <boost/move/move.hpp>
#include <boost/function.hpp>

namespace mock
{
namespace detail
{
    template< typename Result >
    struct lambda
    {
        typedef boost::function< Result() > functor_type;

        template< typename T >
        static functor_type make_val( T t )
        {
            return boost::bind( &do_val< T >, t );
        }
        template< typename T >
        static functor_type make_ref( const boost::reference_wrapper< T >& t )
        {
            return boost::bind( &do_ref< T >, t.get_pointer() );
        }
        template< typename T >
        static functor_type make_move( T& t )
        {
            return boost::bind( &do_move< T >, &t );
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

    private:
        template< typename T >
        static T do_val( T t )
        {
            return t;
        }
        template< typename T >
        static T do_move( T* t )
        {
            return boost::move( *t );
        }
        template< typename T >
        static T& do_ref( T* t )
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
