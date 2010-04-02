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
#include <functional>
#include <algorithm>
#include <vector>
#include <string>

namespace mock
{
    class node : protected verifiable
    {
    public:
        virtual ~node()
        {
            std::for_each( v_.begin(), v_.end(),
                std::mem_fun( &verifiable::untie ) );
        }

        void tag( const std::string& name )
        {
            name_ = name;
        }
        const std::string& tag() const
        {
            return name_;
        }

        void add( verifiable& e )
        {
            v_.push_back( &e );
        }
        void remove( verifiable& e )
        {
            v_.erase( std::remove( v_.begin(), v_.end(), &e ), v_.end() );
        }

        virtual bool verify() const
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

    private:
        typedef std::vector< verifiable* > verifiables_type;
        typedef verifiables_type::const_iterator verifiables_cit;

        std::vector< verifiable* > v_;
        std::string name_;
    };
}

#endif // #ifndef MOCK_NODE_HPP_INCLUDED
