// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_ACTION_HPP_INCLUDED
#define MOCK_ACTION_HPP_INCLUDED

#include "../config.hpp"
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

namespace mock
{
namespace detail
{
    template< typename Result, typename Signature >
    class action_base
    {
    private:
        typedef boost::function< Signature > functor_type;
        typedef boost::function< Result() > action_type;

    public:
        const functor_type& functor() const
        {
            return f_;
        }
        bool valid() const
        {
            return f_ || a_;
        }
        Result trigger() const
        {
            return a_();
        }

        void calls( const functor_type& f )
        {
            if( ! f )
                throw std::invalid_argument( "null functor" );
            f_ = f;
        }

        template< typename Exception >
        void throws( Exception e )
        {
            a_ = boost::bind( &do_throw< Exception >, e );
        }

    protected:
        void set( const action_type& a )
        {
            a_ = a;
        }
        template< typename Y >
        void set( const boost::reference_wrapper< Y >& r )
        {
            a_ = boost::bind( &do_ref< Y >, r.get_pointer() );
        }

    private:
        template< typename T >
        static T& do_ref( T* t )
        {
            return *t;
        }
        template< typename T >
        static Result do_throw( T t )
        {
            throw t;
        }

        functor_type f_;
        action_type a_;
    };

    template< typename Result, typename Signature >
    class action : public action_base< Result, Signature >
    {
    public:
        template< typename Value >
        void returns( const Value& v )
        {
            this->set( boost::ref( store( v ) ) );
        }
        template< typename Y >
        void returns( const boost::reference_wrapper< Y >& r )
        {
            this->set( r );
        }

        template< typename Value >
        void moves( BOOST_RV_REF( Value ) v )
        {
            this->set(
                boost::bind(
                    &boost::move< BOOST_RV_REF( Value ) >,
                    boost::ref( store( boost::move( v ) ) ) ) );
        }

    private:
        struct value : boost::noncopyable
        {
            virtual ~value()
            {}
        };
        template< typename T >
        struct value_imp : value
        {
            typedef
                typename boost::remove_const<
                    typename boost::remove_reference<
                        T
                    >::type
                >::type value_type;

            value_imp( BOOST_RV_REF( value_type ) t )
                : t_( boost::move( t ) )
            {}
            value_imp( const T& t )
                : t_( t )
            {}
            template< typename Y >
            value_imp( Y* y )
                : t_( y )
            {}
            value_type t_;
        };

        template< typename T >
        T& store( BOOST_RV_REF( T ) t )
        {
            v_.reset( new value_imp< T >( boost::move( t ) ) );
            return static_cast< value_imp< T >& >( *v_ ).t_;
        }
        template< typename T >
        T& store( const T& t )
        {
            v_.reset( new value_imp< T >( t ) );
            return static_cast< value_imp< T >& >( *v_ ).t_;
        }
        template< typename T >
        Result& store( T* t )
        {
            v_.reset( new value_imp< Result >( t ) );
            return static_cast< value_imp< Result >& >( *v_ ).t_;
        }

        boost::shared_ptr< value > v_;
    };

    template< typename Signature >
    class action< void, Signature > : public action_base< void, Signature >
    {
    public:
        action()
        {
            this->set( boost::bind( &do_nothing ) );
        }

    private:
        static void do_nothing()
        {}
    };

    template< typename Result, typename Signature >
    class action< std::auto_ptr< Result >, Signature >
        : public action_base< std::auto_ptr< Result >, Signature >
    {
    public:
        action()
        {}
        action( const action& rhs )
            : v_( rhs.v_.release() )
        {
            if( v_.get() )
                returns( boost::ref( v_ ) );
        }

        template< typename Y >
        void returns( Y* r )
        {
            v_.reset( r );
            this->set( boost::ref( v_ ) );
        }
        template< typename Y >
        void returns( std::auto_ptr< Y > r )
        {
            v_ = r;
            this->set( boost::ref( v_ ) );
        }
        template< typename Y >
        void returns( const boost::reference_wrapper< Y >& r )
        {
            this->set( r );
        }

    private:
        mutable std::auto_ptr< Result > v_;
    };
}
} // mock

#endif // MOCK_ACTION_HPP_INCLUDED
