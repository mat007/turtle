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
    template< typename Actual >
    class check
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< bool( Actual ) > functor_type;

    public:
        template< typename Functor >
        explicit check( const Functor& functor,
            BOOST_DEDUCED_TYPENAME boost::enable_if<
                BOOST_DEDUCED_TYPENAME detail::is_functor< Functor >
            >::type* = 0 )
            : functor_( functor )
            , desc_   ( "?" )
        {
            if( !functor_ )
                std::invalid_argument( "invalid functor" );
        }
        template< typename Expected >
        explicit check( const Expected& expected,
            BOOST_DEDUCED_TYPENAME boost::disable_if<
                BOOST_DEDUCED_TYPENAME detail::is_functor< Expected >
            >::type* = 0 )
            : functor_( mock::equal( expected ).functor_ )
            , desc_   ( format( expected ) )
        {
            if( !functor_ )
                std::invalid_argument( "invalid functor" );
        }
        template< typename Constraint >
        explicit check( const placeholder< Constraint >& ph )
            : functor_( ph.functor_ )
            , desc_   ( ph.desc_ )
        {
            if( !functor_ )
                std::invalid_argument( "invalid functor" );
        }

        bool operator()( Actual actual ) const
        {
            return functor_( actual );
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
