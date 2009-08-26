//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_PLACEHOLDER_HPP_INCLUDED
#define MOCK_PLACEHOLDER_HPP_INCLUDED

#include <string>

namespace mock
{
namespace detail
{
    template< typename Functor >
    struct placeholder
    {
        placeholder( const Functor& f, const std::string& desc )
            : functor_( f )
            , desc_   ( desc )
        {}
        Functor functor_;
        std::string desc_;
    };

    template< typename Constraint1, typename Constraint2 >
    class and_
    {
    public:
        and_( const Constraint1& c1, const Constraint2& c2 )
            : c1_( c1 )
            , c2_( c2 )
        {}
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return c1_( y ) && c2_( y );
        }
    private:
        Constraint1 c1_;
        Constraint2 c2_;
    };

    template< typename Constraint1, typename Constraint2 >
    class or_
    {
    public:
        or_( const Constraint1& c1, const Constraint2& c2 )
            : c1_( c1 )
            , c2_( c2 )
        {}
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return c1_( y ) || c2_( y );
        }
    private:
        Constraint1 c1_;
        Constraint2 c2_;
    };

    template< typename Constraint >
    class not_
    {
    public:
        explicit not_( const Constraint& c )
            : c_( c )
        {}
        template< typename Y >
        bool operator()( const Y& y ) const
        {
            return ! c_( y );
        }
    private:
        Constraint c_;
    };

    template< typename F1, typename F2 >
    const placeholder< or_< F1, F2 > >
        operator||( const placeholder< F1 >& lhs,
                    const placeholder< F2 >& rhs )
    {
        return placeholder< or_< F1, F2 > >(
            or_< F1, F2 >( lhs.functor_, rhs.functor_ ),
            "(" + lhs.desc_ + " || " + rhs.desc_ + ")" );
    }

    template< typename F1, typename F2 >
    const placeholder< and_< F1, F2 > >
        operator&&( const placeholder< F1 >& lhs,
                    const placeholder< F2 >& rhs )
    {
        return placeholder< and_< F1, F2 > >(
            and_< F1, F2 >( lhs.functor_, rhs.functor_ ),
            "(" + lhs.desc_ + " && " + rhs.desc_ + ")" );
    }

    template< typename F >
    const placeholder< not_< F > >
        operator!( const placeholder< F >& ph )
    {
        return placeholder< not_< F > >(
            not_< F >( ph.functor_ ), "! " + ph.desc_ );
    }
}
}

#endif // #ifndef MOCK_PLACEHOLDER_HPP_INCLUDED
