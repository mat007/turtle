//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_RESULT_HPP_INCLUDED
#define MOCK_RESULT_HPP_INCLUDED

#include "config.hpp"
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <memory>

namespace mock
{
namespace detail
{
    template< typename T, typename Signature >
    class result
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
            f_ = boost::bind( &throw_exception< Exception >, e );
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        void set( T t )
        {
            f_ = boost::bind( &return_value, t );
        }
        template< typename Y >
        void set( const boost::reference_wrapper< Y >& t )
        {
            f_ = boost::bind( &return_value, t );
        }

        static T return_value( T t )
        {
            return t;
        }

        template< typename Exception >
        static T throw_exception( const Exception& e )
        {
            throw e;
        }

        functor_type f_;
    };

    template< typename T, typename Signature >
    class result< T*, Signature >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

    public:
        void returns( T* t )
        {
            f_ = boost::bind( &return_value, t );
        }
        template< typename Y >
        void returns( const boost::reference_wrapper< Y >& t )
        {
            f_ = boost::bind( &return_value, t );
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
            f_ = boost::bind( &throw_exception< Exception >, e );
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        static T* return_value( T* t )
        {
            return t;
        }

        template< typename Exception >
        static T* throw_exception( const Exception& e )
        {
            throw e;
        }

        functor_type f_;
    };

    template< typename Signature >
    class result< void, Signature >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

    public:
        result()
            : f_( boost::bind( &nothing ) )
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
            f_ = boost::bind( &throw_exception< Exception >, e );
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

    template< typename T, typename Signature >
    class result< std::auto_ptr< T >, Signature >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

    public:
        result()
            : t_()
            , f_()
        {}
        result( const result& rhs )
            : t_( const_cast< result& >( rhs ).t_.release() )
            , f_( t_.get() ? boost::bind( &return_value, boost::ref( t_ ) ) : rhs.f_ )
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
            f_ = boost::bind( &throw_exception< Exception >, e );
            t_.reset();
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        template< typename Y >
        void set( std::auto_ptr< Y > t )
        {
            t_ = t;
            f_ = boost::bind( &return_value, boost::ref( t_ ) );
        }
        template< typename Y >
        void set( const boost::reference_wrapper< Y >& t )
        {
            f_ = boost::bind( &return_value, t );
            t_.reset();
        }
        template< typename Y >
        void set( Y* t )
        {
            t_.reset( t );
            f_ = boost::bind( &return_value, boost::ref( t_ ) );
        }

        static std::auto_ptr< T > return_value( std::auto_ptr< T > t )
        {
            return t;
        }

        template< typename Exception >
        static std::auto_ptr< T > throw_exception( const Exception& e )
        {
            throw e;
        }

        mutable std::auto_ptr< T > t_;
        functor_type f_;
    };

}
}

#endif // #ifndef MOCK_RESULT_HPP_INCLUDED
