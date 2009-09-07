//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_NODE_HPP_INCLUDED
#define MOCK_NODE_HPP_INCLUDED

#include "verifiable.hpp"
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <ostream>

namespace mock
{
    class node : private verifiable
    {
    public:
        node()
            : parent_( 0 )
        {}
        explicit node( node& parent )
            : verifiable()
            , parent_( &parent )
        {
            if( parent_ )
                parent_->add( *this );
        }
        virtual ~node()
        {
            if( parent_ )
                parent_->remove( *this );
        }

        void set_parent( node& parent )
        {
            if( parent_ )
                parent_->remove( *this );
            parent_ = &parent;
            parent_->add( *this );
        }

        void add( verifiable& e )
        {
            v_.push_back( &e );
        }
        void remove( verifiable& e )
        {
            v_.erase( std::remove( v_.begin(), v_.end(), &e ), v_.end() );
        }

        virtual bool verify()
        {
            bool valid = true;
            for( verifiables_cit it = v_.begin(); it != v_.end(); ++it )
                if( ! (*it)->verify() )
                    valid = false;
            return valid;
        }
        virtual void reset()
        {
            std::for_each( v_.begin(), v_.end(),
                std::mem_fun( &verifiable::reset ) );
        }

        friend std::ostream& operator<<( std::ostream& s, const node& n )
        {
            if( n.parent_ )
                s << *n.parent_;
            n.serialize( s );
            return s;
        }

    protected:
        virtual void serialize( std::ostream& s ) const = 0;

    private:
        typedef std::vector< verifiable* > verifiables_type;
        typedef verifiables_type::const_iterator verifiables_cit;

        node* parent_;
        std::vector< verifiable* > v_;
    };
}

#endif // #ifndef MOCK_NODE_HPP_INCLUDED
