//
//  Copyright Mathieu Champlon 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_OPERATORS_HPP_INCLUDED
#define MOCK_OPERATORS_HPP_INCLUDED

#include "placeholder.hpp"

namespace mock
{
namespace detail
{
    template< typename Functor1, typename Functor2 >
    class and_
    {
    public:
        and_( const Functor1& f1, const Functor2& f2 )
            : f1_( f1 )
            , f2_( f2 )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return f1_( actual ) && f2_( actual );
        }
    private:
        Functor1 f1_;
        Functor2 f2_;
    };

    template< typename Functor1, typename Functor2 >
    class or_
    {
    public:
        or_( const Functor1& f1, const Functor2& f2 )
            : f1_( f1 )
            , f2_( f2 )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return f1_( actual ) || f2_( actual );
        }
    private:
        Functor1 f1_;
        Functor2 f2_;
    };

    template< typename Functor >
    class not_
    {
    public:
        explicit not_( const Functor& f )
            : f_( f )
        {}
        template< typename Actual >
        bool operator()( const Actual& actual ) const
        {
            return ! f_( actual );
        }
    private:
        Functor f_;
    };

    template< typename Functor1, typename Functor2 >
    const placeholder< or_< Functor1, Functor2 > >
        operator||( const placeholder< Functor1 >& lhs,
                    const placeholder< Functor2 >& rhs )
    {
        return constraint(
            or_< Functor1, Functor2 >( lhs.f_, rhs.f_ ),
            "(" + lhs.desc_ + " || " + rhs.desc_ + ")" );
    }

    template< typename Functor1, typename Functor2 >
    const placeholder< and_< Functor1, Functor2 > >
        operator&&( const placeholder< Functor1 >& lhs,
                    const placeholder< Functor2 >& rhs )
    {
        return constraint(
            and_< Functor1, Functor2 >( lhs.f_, rhs.f_ ),
            "(" + lhs.desc_ + " && " + rhs.desc_ + ")" );
    }

    template< typename Functor >
    const placeholder< not_< Functor > >
        operator!( const placeholder< Functor >& ph )
    {
        return constraint(
            not_< Functor >( ph.f_ ), "! " + ph.desc_ );
    }
}
}

#endif // #ifndef MOCK_OPERATORS_HPP_INCLUDED
