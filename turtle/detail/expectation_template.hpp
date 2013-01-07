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
        new matcher< arg##n##_type, constraint< any > >( mock::any ) )

#define MOCK_EXPECTATION_WITH(z, n, d) \
    c##n##_.reset( new matcher< arg##n##_type, Constraint_##n >( c##n ) );

#define MOCK_EXPECTATION_MEMBER(z, n, d) \
    boost::shared_ptr< matcher_base< arg##n##_type > > c##n##_;

#define MOCK_EXPECTATION_ARGS(z, n, d) \
    BOOST_PP_COMMA_IF(n) arg##n##_type a##n

#define MOCK_EXPECTATION_IS_VALID(z, n, d) \
    && (*c##n##_)( a##n )

#define MOCK_EXPECTATION_SERIALIZE(z, n, d) \
    BOOST_PP_IF(n, << ", " <<,) *m.c##n##_

namespace mock
{
namespace detail
{
    template< typename Signature > class expectation;

    template< typename R BOOST_PP_COMMA_IF(MOCK_NUM_ARGS)
        BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, typename T) >
    class expectation< R (BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS,T)) >
        : public expectation_base
        , public action< R, R (BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS,T)) >
    {
        BOOST_PP_REPEAT(MOCK_NUM_ARGS,
            MOCK_EXPECTATION_TYPEDEF, BOOST_PP_EMPTY)
    public:
#ifndef MOCK_NUM_ARGS_0
        expectation()
            : BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_INITIALIZE, BOOST_PP_EMPTY)
        {}
        template< BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, typename Constraint_) >
        expectation& with(
            BOOST_PP_ENUM_BINARY_PARAMS(MOCK_NUM_ARGS, Constraint_, c) )
        {
            BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_WITH, BOOST_PP_EMPTY)
            return *this;
        }
#endif
        bool is_valid(
            BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                MOCK_EXPECTATION_ARGS, BOOST_PP_EMPTY) ) const
        {
            return ! i_->exhausted()
                BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                    MOCK_EXPECTATION_IS_VALID, BOOST_PP_EMPTY);
        }

        expectation& in( sequence& s )
        {
            add( s.impl_ );
            return *this;
        }
        expectation& once()
        {
            expect( new detail::once() );
            return *this;
        }
        expectation& never()
        {
            expect( new detail::never() );
            return *this;
        }
        expectation& exactly( std::size_t count )
        {
            expect( new detail::exactly( count ) );
            return *this;
        }
        expectation& at_least( std::size_t min )
        {
            expect( new detail::at_least( min ) );
            return *this;
        }
        expectation& at_most( std::size_t max )
        {
            expect( new detail::at_most( max ) );
            return *this;
        }
        expectation& between( std::size_t min, std::size_t max )
        {
            expect( new detail::between( min, max ) );
            return *this;
        }

        friend std::ostream& operator<<(
            std::ostream& s, const expectation& m )
        {
            return s << (m.i_->exhausted() ? 'v' : '.')
                << ' ' << *m.i_
#ifndef MOCK_NUM_ARGS_0
                << ".with( "
                << BOOST_PP_REPEAT(MOCK_NUM_ARGS,
                    MOCK_EXPECTATION_SERIALIZE, BOOST_PP_EMPTY)
                << " )"
#endif
                ;
        }
    private:
        BOOST_PP_REPEAT(MOCK_NUM_ARGS, MOCK_EXPECTATION_MEMBER, BOOST_PP_EMPTY)
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
