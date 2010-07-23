//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_ACTION_HPP_INCLUDED
#define MOCK_ACTION_HPP_INCLUDED

#include "config.hpp"
#include <boost/spirit/home/phoenix/bind/bind_function.hpp>
#include <boost/spirit/home/phoenix/statement/sequence.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <memory>

namespace mock
{
namespace detail
{
    template< typename Result, typename Signature >
    class action
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

    public:
        template< typename Value >
        void returns( Value v )
        {
            set( v );
        }

        void calls( const functor_type& f )
        {
            if( !f )
                throw std::invalid_argument( "null functor" );
            f_ = f;
        }

        template< typename Exception >
        void throws( Exception e )
        {
            f_ = boost::phoenix::bind( &throw_exception< Exception >, e );
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        void set( Result r )
        {
            f_ = (boost::phoenix::bind( &nothing ), boost::bind( &identity, r ));
        }
        template< typename Y >
        void set( const boost::reference_wrapper< Y >& r )
        {
            f_ = boost::phoenix::val( r );
        }

        static void nothing()
        {}
        static Result identity( Result r )
        {
            return r;
        }

        template< typename Exception >
        static Result throw_exception( const Exception& e )
        {
            throw e;
        }

        functor_type f_;
    };

    template< typename Result, typename Signature >
    class action< Result*, Signature >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

    public:
        void returns( Result* r )
        {
            f_ = boost::phoenix::val( r );
        }
        template< typename Y >
        void returns( const boost::reference_wrapper< Y >& r )
        {
            f_ = boost::phoenix::val( r );
        }

        void calls( const functor_type& f )
        {
            if( !f )
                throw std::invalid_argument( "null functor" );
            f_ = f;
        }

        template< typename Exception >
        void throws( Exception e )
        {
            f_ = boost::phoenix::bind( &throw_exception< Exception >, e );
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        template< typename Exception >
        static Result* throw_exception( const Exception& e )
        {
            throw e;
        }

        functor_type f_;
    };

    template< typename Signature >
    class action< void, Signature >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

    public:
        action()
            : f_( boost::phoenix::bind( &nothing ) )
        {}

        void calls( const functor_type& f )
        {
            if( !f )
                throw std::invalid_argument( "null functor" );
            f_ = f;
        }

        template< typename Exception >
        void throws( Exception e )
        {
            f_ = boost::phoenix::bind( &throw_exception< Exception >, e );
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        static void nothing()
        {}

        template< typename Exception >
        static void throw_exception( const Exception& e )
        {
            throw e;
        }

        functor_type f_;
    };

    template< typename Result, typename Signature >
    class action< std::auto_ptr< Result >, Signature >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

    public:
        action()
            : r_()
            , f_()
        {}
        action( const action& rhs )
            : r_( const_cast< action& >( rhs ).r_.release() )
            , f_( r_.get() ? boost::phoenix::val( boost::ref( r_ ) ) : rhs.f_ )
        {}

        template< typename Value >
        void returns( Value v )
        {
            set( v );
        }

        void calls( const functor_type& f )
        {
            if( !f )
                throw std::invalid_argument( "null functor" );
            f_ = f;
        }

        template< typename Exception >
        void throws( Exception e )
        {
            f_ = boost::phoenix::bind( &throw_exception< Exception >, e );
            r_.reset();
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        template< typename Y >
        void set( std::auto_ptr< Y > r )
        {
            r_ = r;
            f_ = boost::phoenix::val( boost::ref( r_ ) );
        }
        template< typename Y >
        void set( const boost::reference_wrapper< Y >& r )
        {
            f_ = boost::phoenix::val( r );
            r_.reset();
        }
        template< typename Y >
        void set( Y* r )
        {
            r_.reset( r );
            f_ = boost::phoenix::val( boost::ref( r_ ) );
        }

        template< typename Exception >
        static std::auto_ptr< Result > throw_exception( const Exception& e )
        {
            throw e;
        }

        mutable std::auto_ptr< Result > r_;
        functor_type f_;
    };

}
}

#endif // #ifndef MOCK_ACTION_HPP_INCLUDED
