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

        template< typename T >
        void set_child( T& t ) const
        {
            impl_->set_child( t );
        }
        void tag( const std::string& name ) const
        {
            impl_->tag( name );
        }

        bool verify() const
        {
            return impl_->verify();
        }
        void reset()
        {
            impl_->reset();
        }

    private:
        class object_impl : public node
        {
        public:
            object_impl()
                : parent_( 0 )
            {}
            virtual ~object_impl()
            {
                if( parent_ )
                    parent_->remove( *this );
            }
            template< typename T >
            void set_child( T& t )
            {
                if( ! parent_ )
                {
                    root.add( *this );
                    parent_ = &root;
                }
                t.set_parent( *this );
            }

        protected:
            virtual void untie()
            {
                parent_ = 0;
            }

        private:
            node* parent_;
        };

        boost::shared_ptr< object_impl > impl_;
    };
}

#endif // #ifndef MOCK_OBJECT_HPP_INCLUDED
