//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_SEQUENCE_HPP_INCLUDED
#define MOCK_SEQUENCE_HPP_INCLUDED

#include "orderable.hpp"
#include <vector>
#include <algorithm>

namespace mock
{
    class sequence
    {
    public:
        ~sequence()
        {
            for( elements_it it = elements_.begin();
                it != elements_.end(); ++it )
                (*it)->remove( *this );
            for( elements_it it = called_.begin();
                it != called_.end(); ++it )
                (*it)->remove( *this );
        }

        void add( detail::orderable& e )
        {
            elements_.push_back( &e );
        }
        void remove( detail::orderable& e )
        {
            elements_.erase( std::remove( elements_.begin(),
                elements_.end(), &e ), elements_.end() );
            called_.erase( std::remove( called_.begin(),
                called_.end(), &e ), called_.end() );
        }

        bool is_valid( const detail::orderable& e ) const
        {
            return std::find( called_.begin(), called_.end(), &e )
                == called_.end();
        }

        void call( const detail::orderable& e )
        {
            elements_it it =
                std::find( elements_.begin(), elements_.end(), &e );
            if( it != elements_.end() )
            {
                std::copy( elements_.begin(), it,
                    std::back_inserter( called_ ) );
                elements_.erase( elements_.begin(), it );
            }
        }

    private:
        typedef std::vector< detail::orderable* > elements_type;
        typedef elements_type::iterator elements_it;

        elements_type elements_;
        elements_type called_;
    };
}

#endif // #ifndef MOCK_SEQUENCE_HPP_INCLUDED
