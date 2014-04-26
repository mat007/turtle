// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define MOCK_EXPECTATION_TYPEDEF(z, n, d) \
    typedef T##n arg##n##_type;

#define MOCK_EXPECTATION_INITIALIZE(z, n, d) \
    BOOST_PP_COMMA_IF(n) c##n##_( \
        boost::make_shared< \
            matcher< arg##n##_type, constraint< any > > >( mock::any ) )

#define MOCK_EXPECTATION_WITH(z, n, d) \
    c##n##_ = boost::make_shared< \
        matcher< arg##n##_type, Constraint_##n > >( c##n );

#define MOCK_EXPECTATION_MEMBER(z, n, d) \
    boost::shared_ptr< matcher_base< arg##n##_type > > c##n##_;

#define MOCK_EXPECTATION_ARGS(z, n, d) \
    BOOST_PP_COMMA_IF(n) arg##n##_type a##n

#define MOCK_EXPECTATION_IS_VALID(z, n, d) \
    && (*c##n##_)( a##n )

#define MOCK_EXPECTATION_SERIALIZE(z, n, d) \
    BOOST_PP_IF(n, << ", " <<,) *m.c##n##_

#define MOCK_EXPECTATION_IN_ADD(z, n, d ) \
    s##n.impl_->add( this ); sequences_.push_back( s##n.impl_ );

#define MOCK_EXPECTATION_IN(z, n, d) \
    expectation& in( BOOST_PP_ENUM_PARAMS(n, sequence& s) ) \
    { \
        BOOST_PP_REPEAT(n, MOCK_EXPECTATION_IN_ADD, _ ) \
        return *this; \
    }

namespace mock
{
namespace detail
{
    template< typename Signature > class expectation;

    template< typename R
        BOOST_PP_ENUM_TRAILING_PARAMS(MOCK_NUM_ARGS, typename T) >
    class expectation< R (BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS,T)) >
        : public action< R, R (BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS,T)) >
    {
        BOOST_PP_REPEAT(MOCK_NUM_ARGS,
            MOCK_EXPECTATION_TYPEDEF, _)

    public:
        expectation()
            : BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_INITIALIZE, _)
            BOOST_PP_COMMA_IF(MOCK_NUM_ARGS)
                i_( boost::make_shared< unlimited >() )
            , file_( "unknown location" )
            , line_( 0 )
        {}
        expectation( const char* file, int line )
            : BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_INITIALIZE, _)
            BOOST_PP_COMMA_IF(MOCK_NUM_ARGS)
                i_( boost::make_shared< unlimited >() )
            , file_( file )
            , line_( line )
        {}

        ~expectation()
        {
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                (*it)->remove( this );
        }

        expectation& once()
        {
            i_ = boost::make_shared< detail::once >();
            return *this;
        }
        expectation& never()
        {
            i_ = boost::make_shared< detail::never >();
            return *this;
        }
        expectation& exactly( std::size_t count )
        {
            i_ = boost::make_shared< detail::exactly >( count );
            return *this;
        }
        expectation& at_least( std::size_t min )
        {
            i_ = boost::make_shared< detail::at_least >( min );
            return *this;
        }
        expectation& at_most( std::size_t max )
        {
            i_ = boost::make_shared< detail::at_most >( max );
            return *this;
        }
        expectation& between( std::size_t min, std::size_t max )
        {
            i_ = boost::make_shared< detail::between >( min, max );
            return *this;
        }

#ifndef MOCK_NUM_ARGS_0
        template<
            BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, typename Constraint_)
        >
        expectation& with(
            BOOST_PP_ENUM_BINARY_PARAMS(MOCK_NUM_ARGS, Constraint_, c) )
        {
            BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_WITH, _)
            return *this;
        }
#endif

        BOOST_PP_REPEAT(MOCK_MAX_SEQUENCES,
            MOCK_EXPECTATION_IN, _)

        bool verify() const
        {
            return i_->verify();
        }

        bool is_valid(
            BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_ARGS, _) ) const
        {
            return ! i_->exhausted()
                BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                    MOCK_EXPECTATION_IS_VALID, _);
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

        friend std::ostream& operator<<(
            std::ostream& s, const expectation& m )
        {
            return s << (m.i_->exhausted() ? 'v' : '.')
                << ' ' << *m.i_
#ifndef MOCK_NUM_ARGS_0
                << ".with( "
                << BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                    MOCK_EXPECTATION_SERIALIZE, _)
                << " )"
#endif
                ;
        }

    private:
        typedef std::vector<
            boost::shared_ptr< sequence_impl >
        > sequences_type;
        typedef sequences_type::const_iterator sequences_cit;

        BOOST_PP_REPEAT(
            MOCK_NUM_ARGS, MOCK_EXPECTATION_MEMBER, _)
        boost::shared_ptr< invocation > i_;
        sequences_type sequences_;
        const char* file_;
        int line_;
    };
}
} // mock

#undef MOCK_EXPECTATION_TYPEDEF
#undef MOCK_EXPECTATION_INITIALIZE
#undef MOCK_EXPECTATION_WITH
#undef MOCK_EXPECTATION_MEMBER
#undef MOCK_EXPECTATION_ARGS
#undef MOCK_EXPECTATION_IS_VALID
#undef MOCK_EXPECTATION_SERIALIZE
#undef MOCK_EXPECTATION_IN
#undef MOCK_EXPECTATION_IN_ADD
