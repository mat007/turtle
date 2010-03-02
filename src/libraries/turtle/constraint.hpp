//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_CONSTRAINT_HPP_INCLUDED
#define MOCK_CONSTRAINT_HPP_INCLUDED

#include "placeholder.hpp"
#include "functional.hpp"
#include "format.hpp"
#include <sstream>

namespace mock
{
    template< typename Functor, typename Description >
    const detail::placeholder< Functor > constraint( const Functor& f,
                                                     const Description& desc )
    {
        std::stringstream s;
        s << std::boolalpha << desc;
        return detail::placeholder< Functor >( f, s.str() );
    }
    template< typename Functor >
    const detail::placeholder< Functor > constraint( const Functor& f )
    {
        return detail::placeholder< Functor >( f, "?" );
    }
    template< typename Functor, typename T >
    const detail::placeholder< Functor > constraint( const Functor& f,
                                                     const std::string& name,
                                                     const T& t )
    {
        return detail::placeholder< Functor >( f,
            name + "( " + format( t ) + " )" );
    }

namespace detail
{
    template<>
    struct placeholder< any >
    {
        placeholder()
            : desc_( "any" )
        {}
        any f_;
        std::string desc_;
    };
    template<>
    struct placeholder< negate >
    {
        placeholder()
            : desc_( "negate" )
        {}
        negate f_;
        std::string desc_;
    };
    template<>
    struct placeholder< evaluate >
    {
        placeholder()
            : desc_( "evaluate" )
        {}
        evaluate f_;
        std::string desc_;
    };
}
    const detail::placeholder< detail::any > any;
    const detail::placeholder< detail::negate > negate;
    const detail::placeholder< detail::evaluate > evaluate;

    template< typename T >
    detail::placeholder< detail::equal< T > > equal( T t )
    {
        return constraint( detail::equal< T >( t ), "equal", t );
    }

    template< typename T >
    detail::placeholder< detail::same< T > > same( T& t )
    {
        return constraint( detail::same< T >( boost::ref( t ) ),
            "same", &t );
    }

    template< typename T >
    detail::placeholder< detail::less< T > > less( T t )
    {
        return constraint( detail::less< T >( t ), "less", t );
    }

    template< typename T >
    detail::placeholder< detail::greater< T > > greater( T t )
    {
        return constraint( detail::greater< T >( t ), "greater", t );
    }

    template< typename T >
    detail::placeholder<
        detail::or_< detail::less< T >, detail::equal< T > > >
    less_equal( T t )
    {
        return constraint( (less( t ) || equal( t )).f_,
            "less_equal", t );
    }

    template< typename T >
    detail::placeholder<
        detail::or_< detail::greater< T >, detail::equal< T > > >
    greater_equal( T t )
    {
        return constraint( (greater( t ) || equal( t )).f_,
            "greater_equal", t );
    }

    template< typename T >
    detail::placeholder< detail::assign< T > > assign( T t )
    {
        return constraint( detail::assign< T >( t ), "assign", t );
    }

    template< typename T >
    detail::placeholder< detail::retrieve< T > > retrieve( T& t )
    {
        return constraint( detail::retrieve< T >( boost::ref( t ) ),
            "retrieve", t );
    }

    template< typename T >
    detail::placeholder< detail::contains< T > > contain( T t )
    {
        return constraint( detail::contains< T >( t ), "contain", t );
    }
}

#endif // #ifndef MOCK_CONSTRAINT_HPP_INCLUDED
