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
#include <ostream>
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
        void set_parent( T& t ) const
        {
            t.set_parent( *impl_ );
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
            {
                root.add( *this );
            }
            virtual ~object_impl()
            {
                root.remove( *this );
            }
        };

        boost::shared_ptr< object_impl > impl_;
    };
}

#endif // #ifndef MOCK_OBJECT_HPP_INCLUDED
