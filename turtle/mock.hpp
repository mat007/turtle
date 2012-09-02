// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_MOCK_HPP_INCLUDED
#define MOCK_MOCK_HPP_INCLUDED

#include "config.hpp"
#include "object.hpp"
#include "reset.hpp"
#include "verify.hpp"
#include "detail/functor.hpp"
#include "detail/function.hpp"
#include "detail/type_name.hpp"
#include "detail/signature.hpp"
#include "detail/parameter.hpp"
#include "detail/cleanup.hpp"
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/mpl/assert.hpp>

#define MOCK_BASE_CLASS(T, I) \
    struct T : I, mock::object, mock::detail::base< I >
#define MOCK_CLASS(T) \
    struct T : mock::object
#define MOCK_FUNCTOR(f, S) \
    mock::detail::functor< S > f, f##_mock

#define MOCK_HELPER(t) \
    t##_mock( mock::detail::root, BOOST_PP_STRINGIZE(t) )
#define MOCK_ANONYMOUS_HELPER(t) \
    t##_mock( mock::detail::root, "?." )

#define MOCK_METHOD_HELPER(S, t) \
    mutable mock::detail::function< S > t##_mock_; \
    mock::detail::function< S >& t##_mock( \
        const mock::detail::context&, \
        boost::unit_test::const_string instance ) const \
    { \
        mock::detail::configure( *this, t##_mock_, \
            instance.substr( 0, instance.rfind( BOOST_PP_STRINGIZE(t) ) ), \
            mock::detail::type_name( BOOST_SP_TYPEID( *this ) ), \
            BOOST_PP_STRINGIZE(t) ); \
        return t##_mock_; \
    }

#define MOCK_PARAM(z, n, d) \
    BOOST_PP_COMMA_IF(n) d, n >::type p##n
#define MOCK_PARAMS(n, S, tpn) \
    BOOST_PP_REPEAT(n, MOCK_PARAM, tpn mock::detail::parameter< S)
#define MOCK_DECL(M, n, S, c, tpn) \
    tpn boost::function_types::result_type< S >::type M( \
        MOCK_PARAMS(n, S, tpn) ) c

#define MOCK_METHOD_AUX(M, n, S, t, c, tpn) \
    MOCK_DECL(M, n, S, c, tpn) \
    { \
        BOOST_MPL_ASSERT_RELATION( n, ==, \
            boost::function_types::function_arity< S >::value ); \
        return MOCK_ANONYMOUS_HELPER(t)( \
            BOOST_PP_ENUM_PARAMS(n, p) ); \
    }

#define MOCK_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_AUX(M, n, S, t,,) \
    MOCK_METHOD_AUX(M, n, S, t, const,) \
    MOCK_METHOD_HELPER(S, t)
#define MOCK_CONST_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_AUX(M, n, S, t, const,) \
    MOCK_METHOD_HELPER(S, t)
#define MOCK_NON_CONST_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_AUX(M, n, S, t,,) \
    MOCK_METHOD_HELPER(S, t)
#define MOCK_METHOD(M, n) \
    MOCK_METHOD_EXT(M, n, MOCK_SIGNATURE(M), M)

#define MOCK_METHOD_EXT_TPL(M, n, S, t) \
    MOCK_METHOD_AUX(M, n, S, t,, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_AUX(M, n, S, t, const, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_HELPER(S, t)
#define MOCK_CONST_METHOD_EXT_TPL(M, n, S, t) \
    MOCK_METHOD_AUX(M, n, S, t, const, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_HELPER(S, t)
#define MOCK_NON_CONST_METHOD_EXT_TPL(M, n, S, t) \
    MOCK_METHOD_AUX(M, n, S, t,, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_HELPER(S, t)

#define MOCK_CONST_CONVERSION_OPERATOR(T, t) \
    operator T() const { return MOCK_ANONYMOUS_HELPER(t)(); } \
    MOCK_METHOD_HELPER(T(), t)
#define MOCK_NON_CONST_CONVERSION_OPERATOR(T, t) \
    operator T() { return MOCK_ANONYMOUS_HELPER(t)(); } \
    MOCK_METHOD_HELPER(T(), t)
#define MOCK_CONVERSION_OPERATOR(T, t) \
    operator T() const { return MOCK_ANONYMOUS_HELPER(t)(); } \
    operator T() { return MOCK_ANONYMOUS_HELPER(t)(); } \
    MOCK_METHOD_HELPER(T(), t)

#define MOCK_FUNCTION_HELPER(S, t, s) \
    s mock::detail::function< S >& t##_mock( \
        mock::detail::context& context, \
        boost::unit_test::const_string instance ) \
    { \
        static mock::detail::function< S > f; \
        return f( context, instance ); \
    }

#define MOCK_CONSTRUCTOR_AUX(T, n, A, t, tpn) \
    MOCK_FUNCTION_HELPER(void A, t, static) \
    T( MOCK_PARAMS(n, void A, tpn) ) \
    { \
        MOCK_HELPER(t)( BOOST_PP_ENUM_PARAMS(n, p) ); \
    }

#define MOCK_CONSTRUCTOR(T, n, A, t) \
    MOCK_CONSTRUCTOR_AUX(T, n, A, t,)
#define MOCK_CONSTRUCTOR_TPL(T, n, A, t) \
    MOCK_CONSTRUCTOR_AUX(T, n, A, t, BOOST_DEDUCED_TYPENAME)

#define MOCK_DESTRUCTOR(T, t) \
    MOCK_METHOD_HELPER(void(), t) \
    ~T() { try { MOCK_ANONYMOUS_HELPER(t)(); } catch( ... ) {} }

#define MOCK_FUNCTION_AUX(F, n, S, t, s, tpn) \
    MOCK_FUNCTION_HELPER(S, t, s) \
    s MOCK_DECL(F, n, S,,tpn) \
    { \
        BOOST_MPL_ASSERT_RELATION( n, ==, \
            boost::function_types::function_arity< S >::value ); \
        return MOCK_HELPER(t)( BOOST_PP_ENUM_PARAMS(n, p) ); \
    }
#define MOCK_FUNCTION(F, n, S, t) \
    MOCK_FUNCTION_AUX(F, n, S, t,,)
#define MOCK_STATIC_METHOD(F, n, S, t) \
    MOCK_FUNCTION_AUX(F, n, S, t, static,)
#define MOCK_STATIC_METHOD_TPL(F, n, S, t) \
    MOCK_FUNCTION_AUX(F, n, S, t, static, BOOST_DEDUCED_TYPENAME)

#define MOCK_EXPECT(t) MOCK_HELPER(t).expect( __FILE__, __LINE__ )
#define MOCK_RESET(t) MOCK_HELPER(t).reset( __FILE__, __LINE__ )
#define MOCK_VERIFY(t) MOCK_HELPER(t).verify( __FILE__, __LINE__ )

#endif // MOCK_MOCK_HPP_INCLUDED
