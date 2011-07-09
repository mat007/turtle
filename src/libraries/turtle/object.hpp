//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_OBJECT_HPP_INCLUDED
#define MOCK_OBJECT_HPP_INCLUDED

#include "root.hpp"
#include "context.hpp"
#include "parent.hpp"
#include "child.hpp"
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace mock
{
    class object
    {
    public:
        object()
            : impl_( new object_impl() )
        {}

        bool verify() const // $$$$ MAT : to be deprecated
        {
            return impl_->verify();
        }
        void reset() // $$$$ MAT : to be deprecated
        {
            impl_->reset();
        }

    private:
        class object_impl : public detail::context, private verifiable
        {
        public:
            virtual void add( const void* /*p*/, verifiable& v,
                const std::string& instance, const std::string& type,
                const std::string& name )
            {
                if( children_.empty() )
                    mock::detail::root.add( *this );
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
                    mock::detail::root.remove( *this );
            }

            virtual void serialize( std::ostream& s,
                const verifiable& v ) const
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
                group_.reset();
            }

        private:
            typedef std::map< const verifiable*, detail::child > children_t;
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
    E& configure(  const object& o, E& e, const std::string& instance,
        const std::string& type, const std::string& name )
    {
        e.configure( *o.impl_, o.impl_.get(), instance, type, name );
        return e;
    }

    template< typename E, typename T >
    E& configure( const T& t, E& e, const std::string& instance,
        const std::string& type, const std::string& name,
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
