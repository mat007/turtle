//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_MATCHER_HPP_INCLUDED
#define MOCK_MATCHER_HPP_INCLUDED

#include "config.hpp"
#include "invocation.hpp"
#include "result.hpp"
#include "sequence.hpp"
#include "check.hpp"
#include "constraint.hpp"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <ostream>
#include <vector>

namespace mock
{
namespace detail
{
    class matcher_base : private orderable
    {
    public:
        matcher_base()
            : i_( new detail::unlimited() )
            , file_( "unknown location" )
            , line_( 0 )
        {}
        virtual ~matcher_base()
        {
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                (*it)->remove( *this );
        }

        void set_location( const std::string& file, int line )
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
                if( ! (*it)->is_valid( *this ) )
                    return false;
            bool result = i_->invoke();
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                (*it)->call( *this );
            return result;
        }

        const std::string& file() const
        {
            return file_;
        }
        int line() const
        {
            return line_;
        }

    protected:
        boost::shared_ptr< detail::invocation > i_;

        void expect( detail::invocation* i )
        {
            i_.reset( i );
        }

        void add( sequence& s )
        {
            s.add( *this );
            sequences_.push_back( &s );
        }

    private:
        virtual void remove( sequence& s )
        {
            sequences_.erase( std::remove( sequences_.begin(),
                sequences_.end(), &s ), sequences_.end() );
        }

        typedef std::vector< sequence* > sequences_type;
        typedef sequences_type::const_iterator sequences_cit;

        sequences_type sequences_;
        std::string file_;
        int line_;
    };

    template< typename Result, typename Signature, int >
    class matcher
    {
    };

#define MOCK_MATCHER_METHODS \
        matcher& in( sequence& s ) \
        { \
            add( s ); \
            return *this; \
        } \
        matcher& once() \
        { \
            expect( new detail::once() ); \
            return *this; \
        } \
        matcher& never() \
        { \
            expect( new detail::never() ); \
            return *this; \
        } \
        matcher& exactly( std::size_t count ) \
        { \
            expect( new detail::exactly( count ) ); \
            return *this; \
        } \
        matcher& at_least( std::size_t min ) \
        { \
            expect( new detail::at_least( min ) ); \
            return *this; \
        } \
        matcher& at_most( std::size_t max ) \
        { \
            expect( new detail::at_most( max ) ); \
            return *this; \
        } \
        matcher& between( std::size_t min, std::size_t max ) \
        { \
            expect( new detail::between( min, max ) ); \
            return *this; \
        }

    template< typename Result, typename Signature >
    class matcher< Result, Signature, 0 >
        : public matcher_base, public result< Result, Signature >
    {
    public:
        bool is_valid() const
        {
            return i_->is_valid();
        }

        MOCK_MATCHER_METHODS

        friend std::ostream& operator<<( std::ostream& s, const matcher& m )
        {
            return s << (m.i_->is_valid() ? '.' : 'v') << ' ' << *m.i_;
        }
    };

#define MOCK_MATCHER_TYPEDEF(z, n, d) \
    typedef BOOST_DEDUCED_TYPENAME \
        boost::function< Signature >::BOOST_PP_CAT(BOOST_PP_CAT(arg, BOOST_PP_INC(n)), _type) arg##n##_type; \
    typedef detail::check< arg##n##_type > constraint##n##_type;
#define MOCK_MATCHER_CONSTRUCTOR(z, n, d) BOOST_PP_COMMA_IF(n) c##n##_ ( any )
#define MOCK_MATCHER_WITH(z, n, d) c##n##_ = constraint##n##_type( c##n );
#define MOCK_MATCHER_MEMBER(z, n, d) constraint##n##_type c##n##_;
#define MOCK_MATCHER_IS_VALID_PARAMS(z, n, d) BOOST_PP_COMMA_IF(n) arg##n##_type a##n
#define MOCK_MATCHER_IS_VALID(z, n, d) && c##n##_( a##n )
#define MOCK_MATCHER_SERIALIZE(z, n, d) << ", " << m.c##n##_
#define MOCK_MATCHER(z, n, d) \
    template< typename Result, typename Signature > \
    class matcher< Result, Signature, n > \
        : public matcher_base, public result< Result, Signature > \
    { \
        BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_MATCHER_TYPEDEF, BOOST_PP_EMPTY) \
    public: \
        matcher() \
            : BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_MATCHER_CONSTRUCTOR, BOOST_PP_EMPTY) \
        {} \
        template< BOOST_PP_ENUM_PARAMS(n, typename C) > \
        matcher& with( BOOST_PP_ENUM_BINARY_PARAMS(n, const C, & c) ) \
        { \
            BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_MATCHER_WITH, BOOST_PP_EMPTY) \
            return *this; \
        } \
        bool is_valid( BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_MATCHER_IS_VALID_PARAMS, BOOST_PP_EMPTY) ) const \
        { \
            return i_->is_valid() \
                BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_MATCHER_IS_VALID, BOOST_PP_EMPTY); \
        } \
        MOCK_MATCHER_METHODS \
        friend std::ostream& operator<<( std::ostream& s, const matcher& m ) \
        { \
            return s << (m.i_->is_valid() ? '.' : 'v') << ' ' << *m.i_ << ".with( " \
                << m.c0_ \
                BOOST_PP_REPEAT_FROM_TO(1, n, MOCK_MATCHER_SERIALIZE, BOOST_PP_EMPTY) \
                << " )"; \
        } \
    private: \
        BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_MATCHER_MEMBER, BOOST_PP_EMPTY) \
    };
    BOOST_PP_REPEAT_FROM_TO(1, MOCK_MAX_ARGS, MOCK_MATCHER, BOOST_PP_EMPTY)

#undef MOCK_MATCHER_METHODS
#undef MOCK_MATCHER_TYPEDEF
#undef MOCK_MATCHER_CONSTRUCTOR
#undef MOCK_MATCHER_WITH
#undef MOCK_MATCHER_MEMBER
#undef MOCK_MATCHER_IS_VALID_PARAMS
#undef MOCK_MATCHER_IS_VALID
#undef MOCK_MATCHER_SERIALIZE
#undef MOCK_MATCHER

}
}

#endif // #ifndef MOCK_MATCHER_HPP_INCLUDED
