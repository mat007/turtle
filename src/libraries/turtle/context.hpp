//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_CONTEXT_HPP_INCLUDED
#define MOCK_CONTEXT_HPP_INCLUDED

#include <boost/noncopyable.hpp>
#include <ostream>
#include <string>

namespace mock
{
    class verifiable;

namespace detail
{
    class context : boost::noncopyable
    {
    public:
        context() {}
        virtual ~context() {}

        virtual void add( const void* p, verifiable& v, const std::string& instance,
            const std::string& type, const std::string& name ) = 0;
        virtual void add( verifiable& v ) = 0;
        virtual void remove( verifiable& v ) = 0;

        virtual void serialize( std::ostream& s, const verifiable& v ) const = 0;
    };
}
}

#endif // MOCK_CONTEXT_HPP_INCLUDED
