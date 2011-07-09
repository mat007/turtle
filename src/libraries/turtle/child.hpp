//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_CHILD_HPP_INCLUDED
#define MOCK_CHILD_HPP_INCLUDED

#include "parent.hpp"
#include <ostream>
#include <string>

namespace mock
{
namespace detail
{
    class child
    {
    public:
        child()
            : parent_( 0 )
        {}
        void update( parent& p, const std::string& instance,
            const std::string& type, const std::string& name )
        {
            if( instance != "?" || name_.empty() )
                p = parent( instance, type );
            parent_ = &p;
            name_ = name;
        }
        friend std::ostream& operator<<( std::ostream& s, const child& c )
        {
            if( c.parent_ )
                s << *c.parent_;
            return s << c.name_;
        }
    private:
        const parent* parent_;
        std::string name_;
    };
}
}

#endif // MOCK_CHILD_HPP_INCLUDED
