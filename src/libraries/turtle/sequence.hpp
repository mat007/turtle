//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_SEQUENCE_HPP_INCLUDED
#define MOCK_SEQUENCE_HPP_INCLUDED

#include "sequenceable.hpp"
#include <vector>
#include <algorithm>

namespace mock
{
    class sequence
    {
    public:
        ~sequence()
        {
            for( orderables_it it = orderables_.begin();
                it != orderables_.end(); ++it )
                (*it)->remove( *this );
            for( orderables_it it = called_.begin();
                it != called_.end(); ++it )
                (*it)->remove( *this );
        }

        void add( detail::sequenceable& o )
        {
            orderables_.push_back( &o );
        }
        void remove( detail::sequenceable& o )
        {
            orderables_.erase( std::remove( orderables_.begin(),
                orderables_.end(), &o ), orderables_.end() );
            called_.erase( std::remove( called_.begin(),
                called_.end(), &o ), called_.end() );
        }

        bool is_valid( const detail::sequenceable& o ) const
        {
            return std::find( called_.begin(), called_.end(), &o )
                == called_.end();
        }

        void call( const detail::sequenceable& o )
        {
            orderables_it it =
                std::find( orderables_.begin(), orderables_.end(), &o );
            if( it != orderables_.end() )
            {
                std::copy( orderables_.begin(), it,
                    std::back_inserter( called_ ) );
                orderables_.erase( orderables_.begin(), it );
            }
        }

    private:
        typedef std::vector< detail::sequenceable* > orderables_type;
        typedef orderables_type::iterator orderables_it;

        orderables_type orderables_, called_;
    };
}

#endif // #ifndef MOCK_SEQUENCE_HPP_INCLUDED
