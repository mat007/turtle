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
            : f_( f )
            , desc_( desc )
        {}
        Functor f_;
        std::string desc_;
    };

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

    template< typename F1, typename F2 >
    const placeholder< or_< F1, F2 > >
        operator||( const placeholder< F1 >& lhs,
                    const placeholder< F2 >& rhs )
    {
        return placeholder< or_< F1, F2 > >(
            or_< F1, F2 >( lhs.f_, rhs.f_ ),
            "(" + lhs.desc_ + " || " + rhs.desc_ + ")" );
    }

    template< typename F1, typename F2 >
    const placeholder< and_< F1, F2 > >
        operator&&( const placeholder< F1 >& lhs,
                    const placeholder< F2 >& rhs )
    {
        return placeholder< and_< F1, F2 > >(
            and_< F1, F2 >( lhs.f_, rhs.f_ ),
            "(" + lhs.desc_ + " && " + rhs.desc_ + ")" );
    }

    template< typename F >
    const placeholder< not_< F > >
        operator!( const placeholder< F >& ph )
    {
        return placeholder< not_< F > >(
            not_< F >( ph.f_ ), "! " + ph.desc_ );
    }
}
}

#endif // #ifndef MOCK_PLACEHOLDER_HPP_INCLUDED
