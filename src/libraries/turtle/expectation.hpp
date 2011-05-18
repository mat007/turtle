//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_EXPECTATION_HPP_INCLUDED
#define MOCK_EXPECTATION_HPP_INCLUDED

#include "config.hpp"
#include "invocation.hpp"
#include "action.hpp"
#include "sequence.hpp"
#include "check.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/mpl/at.hpp>
#include <ostream>
#include <vector>

namespace mock
{
namespace detail
{
    class expectation_base
    {
    public:
        expectation_base()
            : i_( new detail::unlimited() )
            , file_( "unknown location" )
            , line_( 0 )
        {}
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
                if( ! (*it)->is_valid( this ) )
                    return false;
            bool result = i_->invoke();
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                (*it)->invalidate( this );
            return result;
        }

        bool invoked() const
        {
            return i_->invoked();
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
        ~expectation_base()
        {
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                (*it)->remove( this );
        }

        void expect( detail::invocation* i )
        {
            i_.reset( i );
        }

        void add( boost::shared_ptr< detail::sequence_impl > s )
        {
            s->add( this );
            sequences_.push_back( s );
        }

        boost::shared_ptr< detail::invocation > i_;

    private:
        typedef std::vector< boost::shared_ptr< detail::sequence_impl > > sequences_type;
        typedef sequences_type::const_iterator sequences_cit;

        sequences_type sequences_;
        std::string file_;
        int line_;
    };

    template< typename Signature, int >
    class expectation
    {
    };

#define MOCK_EXPECTATION_METHODS \
        expectation& in( sequence& s ) \
        { \
            add( s.impl_ ); \
            return *this; \
        } \
        expectation& once() \
        { \
            expect( new detail::once() ); \
            return *this; \
        } \
        expectation& never() \
        { \
            expect( new detail::never() ); \
            return *this; \
        } \
        expectation& exactly( std::size_t count ) \
        { \
            expect( new detail::exactly( count ) ); \
            return *this; \
        } \
        expectation& at_least( std::size_t min ) \
        { \
            expect( new detail::at_least( min ) ); \
            return *this; \
        } \
        expectation& at_most( std::size_t max ) \
        { \
            expect( new detail::at_most( max ) ); \
            return *this; \
        } \
        expectation& between( std::size_t min, std::size_t max ) \
        { \
            expect( new detail::between( min, max ) ); \
            return *this; \
        }

    template< typename Signature >
    class expectation< Signature, 0 >
        : public expectation_base
        , public action< BOOST_DEDUCED_TYPENAME
            boost::function_types::result_type< Signature >::type, Signature >
    {
    public:
        bool is_valid() const
        {
            return ! i_->exhausted();
        }

        MOCK_EXPECTATION_METHODS

        friend std::ostream& operator<<( std::ostream& s, const expectation& m )
        {
            return s << (m.i_->exhausted() ? 'v' : '.') << ' ' << *m.i_;
        }
    };

#define MOCK_EXPECTATION_TYPEDEF(z, n, d) \
    typedef BOOST_DEDUCED_TYPENAME \
        boost::mpl::at_c< \
            BOOST_DEDUCED_TYPENAME \
                boost::function_types::parameter_types< Signature >, \
            n \
        >::type arg##n##_type;
#define MOCK_EXPECTATION_CONSTRUCTOR(z, n, d) BOOST_PP_COMMA_IF(n) c##n##_( new detail::check< arg##n##_type, constraint< detail::any > >( mock::any ) )
#define MOCK_EXPECTATION_WITH(z, n, d) c##n##_.reset( new detail::check< arg##n##_type, Constraint##n >( c##n ) );
#define MOCK_EXPECTATION_MEMBER(z, n, d) boost::shared_ptr< detail::check_base< arg##n##_type > > c##n##_;
#define MOCK_EXPECTATION_ARGS(z, n, d) BOOST_PP_COMMA_IF(n) arg##n##_type a##n
#define MOCK_EXPECTATION_IS_VALID(z, n, d) && (*c##n##_)( a##n )
#define MOCK_EXPECTATION_SERIALIZE(z, n, d) BOOST_PP_IF(n, << ", " <<,) *m.c##n##_
#define MOCK_EXPECTATION(z, n, d) \
    template< typename Signature > \
    class expectation< Signature, n > \
        : public expectation_base \
        , public action< BOOST_DEDUCED_TYPENAME \
            boost::function_types::result_type< Signature >::type, Signature > \
    { \
        BOOST_PP_REPEAT(n, MOCK_EXPECTATION_TYPEDEF, BOOST_PP_EMPTY) \
    public: \
        expectation() \
            : BOOST_PP_REPEAT(n, MOCK_EXPECTATION_CONSTRUCTOR, BOOST_PP_EMPTY) \
        {} \
        template< BOOST_PP_ENUM_PARAMS(n, typename Constraint) > \
        expectation& with( BOOST_PP_ENUM_BINARY_PARAMS(n, Constraint, c) ) \
        { \
            BOOST_PP_REPEAT(n, MOCK_EXPECTATION_WITH, BOOST_PP_EMPTY) \
            return *this; \
        } \
        bool is_valid( BOOST_PP_REPEAT(n, MOCK_EXPECTATION_ARGS, BOOST_PP_EMPTY) ) const \
        { \
            return ! i_->exhausted() \
                BOOST_PP_REPEAT(n, MOCK_EXPECTATION_IS_VALID, BOOST_PP_EMPTY); \
        } \
        MOCK_EXPECTATION_METHODS \
        friend std::ostream& operator<<( std::ostream& s, const expectation& m ) \
        { \
            return s << (m.i_->exhausted() ? 'v' : '.') << ' ' << *m.i_ << ".with( " \
                << \
                BOOST_PP_REPEAT(n, MOCK_EXPECTATION_SERIALIZE, BOOST_PP_EMPTY) \
                << " )"; \
        } \
    private: \
        BOOST_PP_REPEAT(n, MOCK_EXPECTATION_MEMBER, BOOST_PP_EMPTY) \
    };
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(MOCK_MAX_ARGS), MOCK_EXPECTATION, BOOST_PP_EMPTY)

#undef MOCK_EXPECTATION_METHODS
#undef MOCK_EXPECTATION_TYPEDEF
#undef MOCK_EXPECTATION_CONSTRUCTOR
#undef MOCK_EXPECTATION_WITH
#undef MOCK_EXPECTATION_MEMBER
#undef MOCK_EXPECTATION_ARGS
#undef MOCK_EXPECTATION_IS_VALID
#undef MOCK_EXPECTATION_SERIALIZE
#undef MOCK_EXPECTATION

}
}

#endif // MOCK_EXPECTATION_HPP_INCLUDED
