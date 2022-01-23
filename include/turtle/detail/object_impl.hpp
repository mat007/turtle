// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_OBJECT_IMPL_HPP_INCLUDED
#define MOCK_OBJECT_IMPL_HPP_INCLUDED

#include "../config.hpp"
#include "root.hpp"
#include "parent.hpp"
#include "type_name.hpp"
#include "context.hpp"
#include "child.hpp"
#include "mutex.hpp"
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>
#include <boost/optional.hpp>

namespace mock
{
namespace detail
{
    class object_impl : public context, public verifiable,
        public std::enable_shared_from_this< object_impl >
    {
    public:
        object_impl()
            : mutex_( std::make_shared< mutex >() )
        {}

        virtual void add( const void* /*p*/, verifiable& v,
            boost::unit_test::const_string instance,
            boost::optional< type_name > type,
            boost::unit_test::const_string name )
        {
            lock _( mutex_ );
            if( children_.empty() )
                detail::root.add( *this );
            children_[ &v ].update( parent_, instance, type, name );
        }
        virtual void add( verifiable& v )
        {
            lock _( mutex_ );
            group_.add( v );
        }
        virtual void remove( verifiable& v )
        {
            lock _( mutex_ );
            group_.remove( v );
            children_.erase( &v );
            if( children_.empty() )
                detail::root.remove( *this );
        }

        virtual void serialize( std::ostream& s, const verifiable& v ) const
        {
            lock _( mutex_ );
            const auto it = children_.find( &v );
            if( it != children_.end() )
                s << it->second;
            else
                s << "?";
        }

        virtual bool verify() const
        {
            lock _( mutex_ );
            return group_.verify();
        }
        virtual void reset()
        {
            lock _( mutex_ );
            std::shared_ptr< object_impl > guard = shared_from_this();
            group_.reset();
        }

    private:
        group group_;
        parent parent_;
        std::map< const verifiable*, child > children_;
        const std::shared_ptr< mutex > mutex_;
    };
}
} // mock

#endif // MOCK_OBJECT_IMPL_HPP_INCLUDED
