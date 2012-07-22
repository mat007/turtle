// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_SEQUENCE_IMPL_HPP_INCLUDED
#define MOCK_SEQUENCE_IMPL_HPP_INCLUDED

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <algorithm>
#include <vector>

namespace mock
{
namespace detail
{
    class sequence_impl : private boost::noncopyable
    {
    public:
        void add( void* e )
        {
            elements_.push_back( e );
        }
        void remove( void* e )
        {
            elements_.erase( std::remove( elements_.begin(),
                elements_.end(), e ), elements_.end() );
        }

        bool is_valid( const void* e ) const
        {
            return std::find( elements_.begin(), elements_.end(), e )
                != elements_.end();
        }

        void invalidate( const void* e )
        {
            elements_type::iterator it =
                std::find( elements_.begin(), elements_.end(), e );
            if( it != elements_.end() )
                elements_.erase( elements_.begin(), it );
        }

    private:
        typedef std::vector< void* > elements_type;

        elements_type elements_;
    };
}
} // mock

#endif // MOCK_SEQUENCE_IMPL_HPP_INCLUDED
