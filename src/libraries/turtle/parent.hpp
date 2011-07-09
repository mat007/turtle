//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_PARENT_HPP_INCLUDED
#define MOCK_PARENT_HPP_INCLUDED

#include <ostream>
#include <string>

namespace mock
{
namespace detail
{
    class parent
    {
    public:
        parent()
        {}
        parent( const std::string& instance, const std::string& type )
            : instance_( instance )
            , type_( type )
        {}
        friend std::ostream& operator<<( std::ostream& s, const parent& p )
        {
            s << p.instance_;
            if( ! p.type_.empty() )
                s << " " + p.type_ + "::";
            return s;
        }
    private:
        std::string instance_;
        std::string type_;
    };
}
}

#endif // MOCK_PARENT_HPP_INCLUDED
