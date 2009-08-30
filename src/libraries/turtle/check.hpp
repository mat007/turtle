//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_CHECK_HPP_INCLUDED
#define MOCK_CHECK_HPP_INCLUDED

#include "placeholder.hpp"
#include "constraint.hpp"
#include "is_functor.hpp"
#include "format.hpp"
#include <boost/function.hpp>
#include <boost/utility/enable_if.hpp>
#include <stdexcept>
#include <ostream>

namespace mock
{
namespace detail
{
    template< typename Arg >
    class check
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< bool( Arg ) > functor_type;

    public:
        template< typename F >
        explicit check( const F& f,
            BOOST_DEDUCED_TYPENAME boost::enable_if<
                BOOST_DEDUCED_TYPENAME detail::is_functor< F >::type
            >::type* = 0 )
            : functor_( f )
            , desc_   ( "?" )
        {
            if( !functor_ )
                std::invalid_argument( "invalid functor" );
        }
        template< typename T >
        explicit check( const T& t,
            BOOST_DEDUCED_TYPENAME boost::disable_if<
                BOOST_DEDUCED_TYPENAME detail::is_functor< T >::type
            >::type* = 0 )
            : functor_( equal( t ).functor_ )
            , desc_   ( mock::format( t ) )
        {
            if( !functor_ )
                std::invalid_argument( "invalid functor" );
        }
        template< typename Constraint >
        explicit check( const placeholder< Constraint >& p )
            : functor_( p.functor_ )
            , desc_   ( p.desc_ )
        {
            if( !functor_ )
                std::invalid_argument( "invalid functor" );
        }

        template< typename Y >
        bool operator()( Y& y ) const
        {
            return functor_( y );
        }

        friend std::ostream& operator<<( std::ostream& s, const check& c )
        {
            return s << c.desc_;
        }

    private:
        functor_type functor_;
        std::string desc_;
    };
}
}

#endif // #ifndef MOCK_CHECK_HPP_INCLUDED
