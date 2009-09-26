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
#include <boost/noncopyable.hpp>
#include <functional>
#include <algorithm>
#include <ostream>
#include <vector>
#include <string>

namespace mock
{
    class node : private boost::noncopyable
    {
    public:
        void add( verifiable& e )
        {
            v_.push_back( &e );
        }
        void remove( verifiable& e )
        {
            v_.erase( std::remove( v_.begin(), v_.end(), &e ), v_.end() );
        }

        bool verify() const
        {
            bool valid = true;
            for( verifiables_cit it = v_.begin(); it != v_.end(); ++it )
                if( ! (*it)->verify() )
                    valid = false;
            return valid;
        }
        void reset()
        {
            std::for_each( v_.begin(), v_.end(),
                std::mem_fun( &verifiable::reset ) );
        }

        friend std::ostream& operator<<( std::ostream& s, const node& n )
        {
            n.serialize( s );
            return s;
        }

    protected:
        virtual ~node()
        {}

        virtual void serialize( std::ostream& s ) const = 0;

    private:
        typedef std::vector< verifiable* > verifiables_type;
        typedef verifiables_type::const_iterator verifiables_cit;

        std::vector< verifiable* > v_;
    };
}

#endif // #ifndef MOCK_NODE_HPP_INCLUDED
