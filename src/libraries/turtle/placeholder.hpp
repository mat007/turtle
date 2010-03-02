//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_PLACEHOLDER_HPP_INCLUDED
#define MOCK_PLACEHOLDER_HPP_INCLUDED

#include "format.hpp"

namespace mock
{
namespace detail
{
    template< typename Functor >
    struct placeholder
    {
        placeholder( const Functor& f, const std::string& desc )
            : f_( f )
            , desc_( desc )
        {}
        Functor f_;
        std::string desc_;
    };
}

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
}

#endif // #ifndef MOCK_PLACEHOLDER_HPP_INCLUDED
