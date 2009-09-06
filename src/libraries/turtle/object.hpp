//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_OBJECT_HPP_INCLUDED
#define MOCK_OBJECT_HPP_INCLUDED

#include "node.hpp"
#include "root.hpp"
#include <string>
#include <ostream>

namespace mock
{
    class object : public node
    {
    public:
        explicit object( node& parent = root, const std::string& name = "" )
            : node( parent )
            , name_( name )
        {}
        explicit object( const std::string& name )
            : node( root )
            , name_( name )
        {}

        void set_name( const std::string& name )
        {
            name_ = name;
        }

    private:
        virtual void serialize( std::ostream& s ) const
        {
            s << (name_.empty() ? "?" : name_) << "::";
        }

    private:
        std::string name_;
    };
}

#endif // #ifndef MOCK_OBJECT_HPP_INCLUDED
