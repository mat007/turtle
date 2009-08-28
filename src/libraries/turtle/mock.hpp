//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_MOCK_HPP_INCLUDED
#define MOCK_MOCK_HPP_INCLUDED

#include "error.hpp"
#include "object.hpp"
#include "expectation.hpp"
#include <boost/function.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/back_inserter.hpp>
#define BOOST_TYPEOF_SILENT
#include <boost/typeof/typeof.hpp>
#include <stdexcept>

namespace mock
{
namespace detail
{
    template< typename T >
    T& ref( T& t )
    {
        return t;
    }
    template< typename T >
    T& ref( T* t )
    {
        if( ! t )
            throw std::invalid_argument( "derefencing null pointer" );
        return *t;
    }
    template< typename T >
    T& ref( std::auto_ptr< T >& t )
    {
        if( ! t.get() )
            throw std::invalid_argument( "derefencing null pointer" );
        return *t;
    }
    template< typename T >
    T& ref( boost::shared_ptr< T >& t )
    {
        if( ! t.get() )
            throw std::invalid_argument( "derefencing null pointer" );
        return *t;
    }
}
}

#define MOCK_MIXIN(T) \
    template< typename U > struct T##_mock_mixin : U \
    { \
        explicit T##_mock_mixin( const std::string& tag = "" ) \
        { \
            mock::object::set_name( BOOST_PP_STRINGIZE(T) + tag ); \
        } \
        explicit T##_mock_mixin( mock::node& parent, const std::string& tag = "" ) \
        { \
            mock::object::set_name( BOOST_PP_STRINGIZE(T) + tag ); \
            mock::node::set_parent( parent ); \
        } \
    }; \
    struct T##_super_class; \
    typedef T##_mock_mixin< T##_super_class > T;
#define MOCK_INTERFACE(T, I) \
    MOCK_MIXIN(T) \
    struct T##_typedef { typedef I interface_type; }; \
    struct T##_super_class : I, mock::object, private T##_typedef
#define MOCK_CLASS(T) \
    MOCK_MIXIN(T) \
    struct T##_super_class : mock::object

namespace mock
{
namespace detail
{
    template< typename M >
    struct signature
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function_types::result_type< M >::type result;
        typedef BOOST_DEDUCED_TYPENAME
            boost::function_types::parameter_types< M >::type parameters;
        typedef BOOST_DEDUCED_TYPENAME
            boost::function_types::function_type<
                BOOST_DEDUCED_TYPENAME boost::mpl::push_front<
                    BOOST_DEDUCED_TYPENAME boost::mpl::pop_front<
                        BOOST_DEDUCED_TYPENAME boost::mpl::copy<
                            parameters,
                            boost::mpl::back_inserter<
                                boost::mpl::vector<>
                            >
                        >::type
                    >::type,
                    result
                >::type
            >::type type;
    };
}
}

#define MOCK_METHOD_ARG(z, n, S) BOOST_PP_COMMA_IF(n) \
    BOOST_PP_CAT(BOOST_PP_CAT(boost::function< S >::arg, BOOST_PP_INC(n)),_type) \
    BOOST_PP_CAT(a, BOOST_PP_INC(n))
#define MOCK_METHOD_ARGS(n, S) \
    BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_METHOD_ARG, S)
#define MOCK_MOCKER_ARG(z, n, d) \
    BOOST_PP_COMMA_IF(n) BOOST_PP_CAT(a, BOOST_PP_INC(n))
#define MOCK_MOCKER_ARGS(n, M) \
    BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_MOCKER_ARG, BOOST_PP_EMPTY)
#define MOCK_METHOD_STUB(M, n, S, t, c) \
    boost::function< S >::result_type M( MOCK_METHOD_ARGS(n, S) ) c \
    { \
        return MOCK_MOCKER(*this, t)( MOCK_MOCKER_ARGS(n, M) ); \
    }
#define MOCK_METHOD_EXPECTATION(S, t) \
    mutable mock::expectation< S > t##_exp;
#define MOCK_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_STUB(M, n, S, t,) \
    MOCK_METHOD_STUB(M, n, S, t, const) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_CONST_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_STUB(M, n, S, t, const) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_NON_CONST_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_STUB(M, n, S, t,) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_METHOD(M, n) \
    MOCK_METHOD_EXT(M, n, MOCK_SIGNATURE(&interface_type::M), M)
#define MOCK_SIGNATURE(pM) \
    mock::detail::signature< BOOST_TYPEOF(pM) >::type
#define MOCK_MOCKER(m, t) \
    mock::detail::ref( m ).t##_exp.set_name( BOOST_PP_STRINGIZE(t) ).set_parent( \
        const_cast< mock::object& >( dynamic_cast< const mock::object& >( mock::detail::ref( m ) ) ) )

#define MOCK_EXPECT(m,t) MOCK_MOCKER(m,t).expect( __FILE__, __LINE__ )
#define MOCK_RESET(m,t) MOCK_MOCKER(m,t).reset()
#define MOCK_VERIFY(m,t) MOCK_MOCKER(m,t).verify()

#endif // #ifndef MOCK_MOCK_HPP_INCLUDED
