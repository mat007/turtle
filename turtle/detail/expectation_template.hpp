// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define MOCK_EXPECTATION_INITIALIZE(z, n, d) \
    BOOST_PP_COMMA_IF(n) c##n##_( \
        boost::make_shared< \
            matcher< T##n, constraint< any > > >( mock::any ) )

#define MOCK_EXPECTATION_WITH(z, n, d) \
    c##n##_ = boost::make_shared< \
        matcher< T##n, Constraint_##n > >( c##n );

#define MOCK_EXPECTATION_MEMBER(z, n, d) \
    boost::shared_ptr< matcher_base< T##n > > c##n##_;

#define MOCK_EXPECTATION_ARGS(z, n, d) \
    BOOST_PP_COMMA_IF(n) T##n a##n

#define MOCK_EXPECTATION_IS_VALID(z, n, d) \
    && (*c##n##_)( a##n )

#define MOCK_EXPECTATION_SERIALIZE(z, n, d) \
    BOOST_PP_IF(n, << ", " <<,) *e.c##n##_

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
    public:
        expectation()
            : BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_INITIALIZE, _)
            BOOST_PP_COMMA_IF(MOCK_NUM_ARGS)
                invocation_( boost::make_shared< unlimited >() )
            , file_( "unknown location" )
            , line_( 0 )
        {}
        expectation( const char* file, int line )
            : BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_INITIALIZE, _)
            BOOST_PP_COMMA_IF(MOCK_NUM_ARGS)
                invocation_( boost::make_shared< unlimited >() )
            , file_( file )
            , line_( line )
        {}

        ~expectation()
        {
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                (*it)->remove( this );
        }

        void invoke( const boost::shared_ptr< invocation >& i )
        {
            invocation_ = i;
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

        void add( sequence& s )
        {
            s.impl_->add( this );
            sequences_.push_back( s.impl_ );
        }

        bool verify() const
        {
            return invocation_->verify();
        }

        bool is_valid(
            BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_ARGS, _) ) const
        {
            return ! invocation_->exhausted()
                BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                    MOCK_EXPECTATION_IS_VALID, _);
        }

        bool invoke() const
        {
            for( sequences_cit it = sequences_.begin();
                it != sequences_.end(); ++it )
                if( ! (*it)->is_valid( this ) )
                    return false;
            bool result = invocation_->invoke();
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
            std::ostream& s, const expectation& e )
        {
            return s << (e.invocation_->exhausted() ? 'v' : '.')
                << ' ' << *e.invocation_
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
        boost::shared_ptr< invocation > invocation_;
        sequences_type sequences_;
        const char* file_;
        int line_;
    };
}
} // mock

#undef MOCK_EXPECTATION_INITIALIZE
#undef MOCK_EXPECTATION_WITH
#undef MOCK_EXPECTATION_MEMBER
#undef MOCK_EXPECTATION_ARGS
#undef MOCK_EXPECTATION_IS_VALID
#undef MOCK_EXPECTATION_SERIALIZE
