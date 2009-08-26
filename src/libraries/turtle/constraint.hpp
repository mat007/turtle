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
            name + "( " + detail::format( t ) + " )" );
    }

namespace detail
{
    template<>
    struct placeholder< nothing >
    {
        placeholder()
            : desc_( "any" )
        {}
        nothing functor_;
        std::string desc_;
    };
    template<>
    struct placeholder< negation >
    {
        placeholder()
            : desc_( "negate" )
        {}
        negation functor_;
        std::string desc_;
    };
    template<>
    struct placeholder< evaluation >
    {
        placeholder()
            : desc_( "evaluate" )
        {}
        evaluation functor_;
        std::string desc_;
    };
}
    const detail::placeholder< detail::nothing > any;
    const detail::placeholder< detail::negation > negate;
    const detail::placeholder< detail::evaluation > evaluate;

    template< typename T >
    detail::placeholder< detail::equality< T > > equal( T t )
    {
        return constraint( detail::equality< T >( t ), "equal", t );
    }

    template< typename T >
    detail::placeholder< detail::identity< T > > same( T& t )
    {
        return constraint( detail::identity< T >( boost::ref( t ) ),
            "same", &t );
    }

    template< typename T >
    detail::placeholder< detail::inferiority< T > > less( T t )
    {
        return constraint( detail::inferiority< T >( t ), "less", t );
    }

    template< typename T >
    detail::placeholder< detail::superiority< T > > greater( T t )
    {
        return constraint( detail::superiority< T >( t ), "greater", t );
    }

    template< typename T >
    detail::placeholder<
        detail::or_< detail::inferiority< T >, detail::equality< T > > >
    less_equal( T t )
    {
        return constraint( (less( t ) || equal( t )).functor_,
            "less_equal", t );
    }

    template< typename T >
    detail::placeholder<
        detail::or_< detail::superiority< T >, detail::equality< T > > >
    greater_equal( T t )
    {
        return constraint( (greater( t ) || equal( t )).functor_,
            "greater_equal", t );
    }

    template< typename T >
    detail::placeholder< detail::assignment< T > > assign( T t )
    {
        return constraint( detail::assignment< T >( t ), "assign", t );
    }

    template< typename T >
    detail::placeholder< detail::retrieval< T > > retrieve( T& t )
    {
        return constraint( detail::retrieval< T >( boost::ref( t ) ),
            "retrieve", t );
    }
}

#endif // #ifndef MOCK_CONSTRAINT_HPP_INCLUDED
