// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_EXPECTATION_BASE_HPP_INCLUDED
#define MOCK_EXPECTATION_BASE_HPP_INCLUDED

#include "../config.hpp"
#include "../sequence.hpp"
#include "invocation.hpp"
#include <boost/make_shared.hpp>
#include <vector>

namespace mock
{
namespace detail
{
    class expectation_base
    {
    public:
        void set_location( const char* file, int line )
        {
            file_ = file;
            line_ = line;
        }

        bool verify() const
        {
            return i_->verify();
        }

        bool invoke() const
        {
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                if( ! (*it)->is_valid( this ) )
                    return false;
            bool result = i_->invoke();
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                (*it)->invalidate( this );
            return result;
        }

        const char* file() const
        {
            return file_;
        }
        int line() const
        {
            return line_;
        }

    protected:
        expectation_base()
            : i_( boost::make_shared< unlimited >() )
            , file_( "unknown location" )
            , line_( 0 )
        {}
        ~expectation_base()
        {
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                (*it)->remove( this );
        }

        void add( boost::shared_ptr< sequence_impl > s )
        {
            s->add( this );
            sequences_.push_back( s );
        }

        boost::shared_ptr< invocation > i_;

    private:
        typedef std::vector<
            boost::shared_ptr< sequence_impl >
        > sequences_type;
        typedef sequences_type::const_iterator sequences_cit;

        sequences_type sequences_;
        const char* file_;
        int line_;
    };
}
} // mock

#endif // MOCK_EXPECTATION_BASE_HPP_INCLUDED
