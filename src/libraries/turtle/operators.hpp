//
//  Copyright Mathieu Champlon 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_OPERATORS_HPP_INCLUDED
#define MOCK_OPERATORS_HPP_INCLUDED

#include "constraint.hpp"

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
        friend std::ostream& operator<<( std::ostream& os, const and_& a )
        {
            return os << "( " << a.f1_ << " && " << a.f2_ << " )";
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
        friend std::ostream& operator<<( std::ostream& os, const or_& o )
        {
            return os << "( " << o.f1_ << " || " << o.f2_ << " )";
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
        friend std::ostream& operator<<( std::ostream& os, const not_& n )
        {
            return os << "! " << n.f_;
        }
    private:
        Functor f_;
    };
}

    template< typename Functor1, typename Functor2 >
    const constraint< detail::or_< Functor1, Functor2 > >
        operator||( const constraint< Functor1 >& lhs,
                    const constraint< Functor2 >& rhs )
    {
        return detail::or_< Functor1, Functor2 >( lhs.f_, rhs.f_ );
    }

    template< typename Functor1, typename Functor2 >
    const constraint< detail::and_< Functor1, Functor2 > >
        operator&&( const constraint< Functor1 >& lhs,
                    const constraint< Functor2 >& rhs )
    {
        return detail::and_< Functor1, Functor2 >( lhs.f_, rhs.f_ );
    }

    template< typename Functor >
    const constraint< detail::not_< Functor > >
        operator!( const constraint< Functor >& ph )
    {
        return detail::not_< Functor >( ph.f_ );
    }
}

#endif // #ifndef MOCK_OPERATORS_HPP_INCLUDED
