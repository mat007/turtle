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
#include "type_name.hpp"
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
#include <boost/mpl/copy.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/back_inserter.hpp>
#define BOOST_TYPEOF_SILENT
#include <boost/typeof/typeof.hpp>
#include <boost/shared_ptr.hpp>
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

    template< typename E >
    E& set_parent( E& e, const object& o )
    {
        o.set_parent( e );
        return e;
    }
    template< typename E, typename T >
    E& set_parent( E& e, const T&,
        BOOST_DEDUCED_TYPENAME boost::disable_if<
            BOOST_DEDUCED_TYPENAME boost::is_base_of< object, T >::type
        >::type* = 0 )
    {
        return e;
    }

    template< typename T >
    std::string op( T& )
    {
        return ".";
    }
    template< typename T >
    std::string op( T* )
    {
        return "->";
    }
    template< typename T >
    std::string op( std::auto_ptr< T >& )
    {
        return "->";
    }
    template< typename T >
    std::string op( boost::shared_ptr< T >& )
    {
        return "->";
    }

    template< typename E >
    E& configure( BOOST_DEDUCED_TYPENAME E::expectation_tag,
        const std::string& parent, const std::string& /*op*/,
        const std::string& /*name*/, E& e )
    {
        if( parent != "?" || e.tag() == "?" )
            e.tag( parent );
        return e;
    }
    template< typename E, typename T >
    E& configure( E& e, const std::string& parent, const std::string& op,
        const std::string& name, const T& t )
    {
        set_parent( e, t );
        if( parent != "?" || e.tag() == "?" )
            e.tag( parent + op + type_name( typeid( T ) ) + "::" + name );
        return e;
    }

    template< typename T >
    struct base
    {
        typedef T base_type;
    };

    struct no_type
    {
    private:
        no_type();
    };

    template< typename S, int n, bool B >
    struct arg_imp
    {
        typedef no_type type;
    };
    template< typename S, int n >
    struct arg_imp< S, n, true >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::mpl::at_c<
                BOOST_DEDUCED_TYPENAME
                    boost::function_types::parameter_types< S >::type,
                n - 1
            >::type type;
    };
    template< typename S, int n, int N >
    struct arg
    {
        BOOST_MPL_ASSERT_RELATION( n, >, 0 );
        BOOST_MPL_ASSERT_RELATION( n, <=, N );
        typedef BOOST_DEDUCED_TYPENAME
            arg_imp< S, n,
                boost::function_types::function_arity< S >::value == N
            >::type type;
    };

    template< typename E, int N >
    struct has_arity
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::mpl::equal_to<
                BOOST_DEDUCED_TYPENAME E::arity,
                boost::mpl::size_t< N >
            >::type type;
    };

#define MOCK_CALL_PARAM(z, n, d) \
    BOOST_DEDUCED_TYPENAME \
        boost::mpl::at_c< \
            BOOST_DEDUCED_TYPENAME E::parameter_types, \
            n \
        >::type t##n
#define MOCK_CALL_NO_TYPE(z, n, d) no_type
#define MOCK_CALL(z, n, d) \
    template< typename E > \
    BOOST_DEDUCED_TYPENAME boost::enable_if< \
        BOOST_DEDUCED_TYPENAME has_arity< E, n >::type, \
        BOOST_DEDUCED_TYPENAME E::result_type \
    >::type \
        call( E e BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, MOCK_CALL_PARAM, BOOST_PP_EMPTY) ) \
    { \
        return e( BOOST_PP_ENUM_PARAMS(n, t) ); \
    } \
    template< typename E > \
    BOOST_DEDUCED_TYPENAME boost::disable_if< \
        BOOST_DEDUCED_TYPENAME has_arity< E, n >::type, \
        BOOST_DEDUCED_TYPENAME E::result_type \
    >::type \
        call( E BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, MOCK_CALL_NO_TYPE, BOOST_PP_EMPTY) ) \
    { \
        throw std::logic_error( "should never be called" ); \
    }
    BOOST_PP_REPEAT_FROM_TO(0, MOCK_MAX_ARGS, MOCK_CALL, BOOST_PP_EMPTY)
#undef MOCK_CALL
#undef MOCK_CALL_NO_TYPE
#undef MOCK_CALL_PARAM
}
}

#define MOCK_BASE_CLASS(T, I) \
    struct T : I, mock::object, mock::detail::base< I >
#define MOCK_CLASS(T) \
    struct T : mock::object
#define MOCK_FUNCTOR(S) \
    mock::expectation< S >

#define MOCK_MOCKER(o, t) \
    mock::detail::configure( mock::detail::ref( o ).exp##t, \
        BOOST_PP_STRINGIZE(o), mock::detail::op( o ), \
        BOOST_PP_STRINGIZE(t), mock::detail::ref( o ) )
#define MOCK_ANONYMOUS_MOCKER(o, t) \
    mock::detail::configure( mock::detail::ref( o ).exp##t, \
        "?", ".", BOOST_PP_STRINGIZE(t), mock::detail::ref( o ) )

#define MOCK_METHOD_EXPECTATION(S, t) \
    mutable mock::expectation< S > exp##t;
#define MOCK_SIGNATURE(M) \
    mock::detail::signature< BOOST_TYPEOF(&base_type::M) >::type
#define MOCK_SIGNATURE_TPL(M) \
    BOOST_DEDUCED_TYPENAME mock::detail::signature< BOOST_TYPEOF_TPL(&base_type::M) >::type

#define MOCK_METHOD_ARG(N, n, S, tpn) \
    BOOST_PP_COMMA_IF(n) tpn mock::detail::arg< S, BOOST_PP_INC(n), N >::type BOOST_PP_CAT(p, n)
#define MOCK_METHOD_ARG_PROXY(z, n, d) \
    MOCK_METHOD_ARG( \
        BOOST_PP_ARRAY_ELEM(0, d), \
        n, \
        BOOST_PP_ARRAY_ELEM(1, d), \
        BOOST_PP_ARRAY_ELEM(2, d) )
#define MOCK_METHOD_STUB(M, n, S, t, c, tpn) \
    tpn boost::function_types::result_type< S >::type M( \
        BOOST_PP_REPEAT_FROM_TO(0, n, MOCK_METHOD_ARG_PROXY, (3, (n, S, tpn))) ) c \
    { \
        return mock::detail::call( MOCK_ANONYMOUS_MOCKER(this, t) \
            BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, p) ); \
    }
#define MOCK_METHOD_STUB_PROXY(z, n, d) \
    MOCK_METHOD_STUB( \
        BOOST_PP_ARRAY_ELEM(0, d), \
        n, \
        BOOST_PP_ARRAY_ELEM(1, d), \
        BOOST_PP_ARRAY_ELEM(2, d), \
        BOOST_PP_ARRAY_ELEM(3, d), \
        BOOST_PP_ARRAY_ELEM(4, d))

#define MOCK_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_STUB(M, n, S, t,,) \
    MOCK_METHOD_STUB(M, n, S, t, const,) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_CONST_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_STUB(M, n, S, t, const,) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_NON_CONST_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_STUB(M, n, S, t,,) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_METHOD(M, n) \
    MOCK_METHOD_EXT(M, n, MOCK_SIGNATURE(M), M)

#define MOCK_METHOD_EXT_TPL(M, n, S, t) \
    MOCK_METHOD_STUB(M, n, S, t,, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_STUB(M, n, S, t, const, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_CONST_METHOD_EXT_TPL(M, n, S, t) \
    MOCK_METHOD_STUB(M, n, S, t, const, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_NON_CONST_METHOD_EXT_TPL(M, n, S, t) \
    MOCK_METHOD_STUB(M, n, S, t,, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_METHOD_TPL(M, n) \
    MOCK_METHOD_EXT_TPL(M, n, MOCK_SIGNATURE_TPL(M), M)

#define MOCK_DESTRUCTOR(T, t) \
    ~T() { exp##t(); } \
    MOCK_METHOD_EXPECTATION(void(), t)

#define MOCK_EXPECT(o,t) MOCK_MOCKER(o,t).expect( __FILE__, __LINE__ )
#define MOCK_RESET(o,t) MOCK_MOCKER(o,t).reset()
#define MOCK_VERIFY(o,t) MOCK_MOCKER(o,t).verify()

// alternate experimental macros below, way too slow to compile to be really usable

#define MOCK_METHOD_STUB_ALT(M, S, t, c, tpn) \
    BOOST_PP_REPEAT_FROM_TO(0, MOCK_MAX_ARGS, MOCK_METHOD_STUB_PROXY, (5,(M, S, t, c, tpn)))

#define MOCK_METHOD_EXT_ALT(M, S, t) \
    MOCK_METHOD_STUB_ALT(M, S, t,,) \
    MOCK_METHOD_STUB_ALT(M, S, t, const,) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_METHOD_ALT(M) \
    MOCK_METHOD_EXT_ALT(M, MOCK_SIGNATURE(M), M)

#define MOCK_METHOD_EXT_TPL_ALT(M, S, t) \
    MOCK_METHOD_STUB_ALT(M, S, t,, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_STUB_ALT(M, S, t, const, BOOST_DEDUCED_TYPENAME) \
    MOCK_METHOD_EXPECTATION(S, t)
#define MOCK_METHOD_TPL_ALT(M) \
    MOCK_METHOD_EXT_TPL_ALT(M, MOCK_SIGNATURE_TPL(M), M)

#endif // #ifndef MOCK_MOCK_HPP_INCLUDED
