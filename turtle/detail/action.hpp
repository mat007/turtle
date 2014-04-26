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
#include "lambda.hpp"
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/function.hpp>
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
        typedef lambda< Result, Signature > lambda_type;

    public:
        const functor_type& functor() const
        {
            return f_;
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
            f_ = lambda_type::make_throw( e );
        }

    protected:
        void set( const functor_type& f )
        {
            f_ = f;
        }
        template< typename Y >
        void set( const boost::reference_wrapper< Y >& r )
        {
            f_ = lambda_type::make_ref( r );
        }

        functor_type f_;
    };

    template< typename Result, typename Signature >
    class action : public action_base< Result, Signature >
    {
        typedef lambda< Result, Signature > lambda_type;

    public:
        template< typename Value >
        void returns( const Value& v )
        {
            returns( boost::ref( v_.store( v ) ) );
        }
        template< typename Value >
        void returns( Value* v )
        {
            typedef typename
                boost::remove_reference<
                    typename boost::remove_const< Result >::type
                >::type result_type;
            returns( result_type( v ) );
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
                lambda_type::make_move( v_.store( boost::move( v ) ) ) );
        }

    private:
        struct holder : boost::noncopyable
        {
            virtual ~holder()
            {}
        };
        template< typename T >
        struct holder_imp : holder
        {
            holder_imp( BOOST_RV_REF( T ) t )
                : t_( boost::move( t ) )
            {}
            holder_imp( const T& t )
                : t_( t )
            {}
            T t_;
        };

        struct value
        {
            template< typename T >
            T& store( BOOST_RV_REF( T ) t )
            {
                h_.reset( new holder_imp< T >( boost::move( t ) ) );
                return static_cast< holder_imp< T >* >( h_.get() )->t_;
            }
            template< typename T >
            T& store( const T& t )
            {
                h_.reset( new holder_imp< T >( t ) );
                return static_cast< holder_imp< T >* >( h_.get() )->t_;
            }
            boost::shared_ptr< holder > h_;
        };

        value v_;
    };

    template< typename Result, typename Signature >
    class action< Result*, Signature >
        : public action_base< Result*, Signature >
    {
        typedef lambda< Result*, Signature > lambda_type;

    public:
        void returns( Result* r )
        {
            this->set( lambda_type::make_val( r ) );
        }
        template< typename Y >
        void returns( const boost::reference_wrapper< Y >& r )
        {
            this->set( r );
        }
    };

    template< typename Signature >
    class action< void, Signature > : public action_base< void, Signature >
    {
        typedef lambda< void, Signature > lambda_type;

    public:
        action()
        {
            this->set( lambda_type::make_nothing() );
        }
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
            returns( boost::ref( v_ ) );
        }
        template< typename Y >
        void returns( std::auto_ptr< Y > r )
        {
            v_ = r;
            returns( boost::ref( v_ ) );
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
