// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_OBJECT_IMPL_HPP_INCLUDED
#define MOCK_OBJECT_IMPL_HPP_INCLUDED

#include "detail/root.hpp"
#include "detail/parent.hpp"
#include "detail/type_name.hpp"
#include "detail/context.hpp"
#include "detail/child.hpp"
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

namespace mock
{
namespace detail
{
    class object_impl : public detail::context, public detail::verifiable,
        public boost::enable_shared_from_this< object_impl >
    {
    public:
        virtual void add( const void* /*p*/, detail::verifiable& v,
            boost::unit_test::const_string instance,
            const boost::optional< detail::type_name >& type,
            boost::unit_test::const_string name )
        {
            if( children_.empty() )
                mock::detail::root.add( *this );
            children_[ &v ].update( parent_, instance, type, name );
        }
        virtual void add( detail::verifiable& v )
        {
            group_.add( v );
        }
        virtual void remove( detail::verifiable& v )
        {
            group_.remove( v );
            children_.erase( &v );
            if( children_.empty() )
                mock::detail::root.remove( *this );
        }

        virtual void serialize( std::ostream& s,
            const detail::verifiable& v ) const
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
        typedef std::map<
            const detail::verifiable*,
            detail::child
        > children_t;
        typedef children_t::const_iterator children_cit;

        detail::group group_;
        detail::parent parent_;
        children_t children_;
    };
}
} // mock

#endif // MOCK_OBJECT_IMPL_HPP_INCLUDED
