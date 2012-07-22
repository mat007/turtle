// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_OBJECT_IMPL_HPP_INCLUDED
#define MOCK_OBJECT_IMPL_HPP_INCLUDED

#include "root.hpp"
#include "parent.hpp"
#include "type_name.hpp"
#include "context.hpp"
#include "child.hpp"
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

namespace mock
{
namespace detail
{
    class object_impl : public context, public verifiable,
        public boost::enable_shared_from_this< object_impl >
    {
    public:
        virtual void add( const void* /*p*/, verifiable& v,
            boost::unit_test::const_string instance,
            boost::optional< type_name > type,
            boost::unit_test::const_string name )
        {
            if( children_.empty() )
                detail::root.add( *this );
            children_[ &v ].update( parent_, instance, type, name );
        }
        virtual void add( verifiable& v )
        {
            group_.add( v );
        }
        virtual void remove( verifiable& v )
        {
            group_.remove( v );
            children_.erase( &v );
            if( children_.empty() )
                detail::root.remove( *this );
        }

        virtual void serialize( std::ostream& s, const verifiable& v ) const
        {
            children_cit it = children_.find( &v );
            if( it != children_.end() )
                s << it->second;
            else
                s << "?";
        }

        virtual bool verify() const
        {
            return group_.verify();
        }
        virtual void reset()
        {
            boost::shared_ptr< object_impl > guard = shared_from_this();
            group_.reset();
        }

    private:
        typedef std::map< const verifiable*, child > children_t;
        typedef children_t::const_iterator children_cit;

        group group_;
        parent parent_;
        children_t children_;
    };
}
} // mock

#endif // MOCK_OBJECT_IMPL_HPP_INCLUDED
