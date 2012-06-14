//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

#ifndef MOCK_OBJECT_HPP_INCLUDED
#define MOCK_OBJECT_HPP_INCLUDED

#include "root.hpp"
#include "context.hpp"
#include "parent.hpp"
#include "child.hpp"
#include "type_name.hpp"
#include <boost/enable_shared_from_this.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/shared_ptr.hpp>

namespace mock
{
    class object
    {
    public:
        object()
            : impl_( new object_impl() )
        {}

    protected:
        ~object()
        {}

    private:
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

    public:
        boost::shared_ptr< object_impl > impl_;
    };

namespace detail
{
    template< typename E >
    E& configure( const object& o, E& e,
        boost::unit_test::const_string instance,
        const boost::optional< type_name >& type,
        boost::unit_test::const_string name )
    {
        e.configure( *o.impl_, o.impl_.get(), instance, type, name );
        return e;
    }

    template< typename E, typename T >
    E& configure( const T& t, E& e,
        boost::unit_test::const_string instance,
        const boost::optional< type_name >& type,
        boost::unit_test::const_string name,
        BOOST_DEDUCED_TYPENAME boost::disable_if<
            BOOST_DEDUCED_TYPENAME boost::is_base_of< object, T >
        >::type* = 0 )
    {
        e.configure( mock::detail::root, &t, instance, type, name );
        return e;
    }
}

    inline bool verify( const object& o )
    {
        return o.impl_->verify();
    }
    inline void reset( object& o )
    {
        o.impl_->reset();
    }
}

#endif // MOCK_OBJECT_HPP_INCLUDED
