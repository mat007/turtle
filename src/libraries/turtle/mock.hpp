//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_MOCK_HPP_INCLUDED
#define MOCK_MOCK_HPP_INCLUDED

#include "config.hpp"
#include "error.hpp"
#include "object.hpp"
#include "function.hpp"
#include "type_name.hpp"
#include "args.hpp"
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/copy.hpp>
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
    void set_parent( E& e, const std::string& prefix,
        const std::string& name, const object& o )
    {
        o.set_child( e );
        o.tag( prefix );
        e.tag( name );
    }
    template< typename E, typename T >
    void set_parent( E& e, const std::string& prefix,
        const std::string& name, const T&,
        BOOST_DEDUCED_TYPENAME boost::disable_if<
            BOOST_DEDUCED_TYPENAME boost::is_base_of< object, T >
        >::type* = 0 )
    {
        e.tag( prefix + name );
    }

    template< typename E >
    E& configure( BOOST_DEDUCED_TYPENAME E::function_tag,
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
        if( parent != "?" || e.tag() == "?" )
            mock::detail::set_parent( e,
                parent + op + mock::detail::type_name( typeid( T ) ) + "::",
                name, t );
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

    template< typename T >
    struct base
    {
        typedef T base_type;
    };

    template< typename E, int N >
    struct has_arity : boost::mpl::equal_to< BOOST_DEDUCED_TYPENAME E::arity, boost::mpl::size_t< N > >
    {};

#define MOCK_CALL(z, n, d) \
    template< typename E > \
    BOOST_DEDUCED_TYPENAME boost::enable_if< \
    BOOST_DEDUCED_TYPENAME has_arity< E, n >::type, \
        BOOST_DEDUCED_TYPENAME E::result_type \
    >::type \
        call( E e BOOST_PP_COMMA_IF(n) MOCK_ARGS(n, BOOST_DEDUCED_TYPENAME E::signature_type, BOOST_DEDUCED_TYPENAME ) ) \
    { \
        return e( BOOST_PP_ENUM_PARAMS(n, p) ); \
    }
    BOOST_PP_REPEAT_FROM_TO(0, MOCK_NUM_ARGS, MOCK_CALL, BOOST_PP_EMPTY)
#undef MOCK_CALL
}
}

#define MOCK_BASE_CLASS(T, I) \
    struct T : I, mock::object, mock::detail::base< I >
#define MOCK_CLASS(T) \
    struct T : mock::object
#define MOCK_FUNCTOR(S) \
    mock::function< S >

#define MOCK_MOCKER(o, t) \
    mock::detail::configure( mock::detail::ref( o ).exp##t, \
        BOOST_PP_STRINGIZE(o), mock::detail::op( o ), \
        BOOST_PP_STRINGIZE(t), mock::detail::ref( o ) )
#define MOCK_ANONYMOUS_MOCKER_EXT(o, M, t) \
    mock::detail::configure( mock::detail::ref( o ).exp##t, \
        "?", ".", BOOST_PP_STRINGIZE(M), mock::detail::ref( o ) )
#define MOCK_ANONYMOUS_MOCKER(o, t) \
    MOCK_ANONYMOUS_MOCKER_EXT( o, t, t )

#define MOCK_METHOD_EXPECTATION(S, t) \
    mutable mock::function< S > exp##t;
#define MOCK_SIGNATURE(M) \
    mock::detail::signature< BOOST_TYPEOF(&base_type::M) >::type
#define MOCK_SIGNATURE_TPL(M) \
    BOOST_DEDUCED_TYPENAME mock::detail::signature< BOOST_TYPEOF_TPL(&base_type::M) >::type

#define MOCK_METHOD_STUB(M, n, S, t, c, tpn) \
    MOCK_DECL(M, n, S, c, tpn) \
    { \
        return mock::detail::call( MOCK_ANONYMOUS_MOCKER(this, t) \
            BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, p) ); \
    }

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
    ~T() { MOCK_ANONYMOUS_MOCKER_EXT(this, ~T, t).test(); } \
    MOCK_METHOD_EXPECTATION(void(), t)

#define MOCK_CONST_CONVERSION_OPERATOR(T, t) \
    operator T() const { return MOCK_ANONYMOUS_MOCKER_EXT(this, operator T, t)(); } \
    MOCK_METHOD_EXPECTATION(T(), t)
#define MOCK_NON_CONST_CONVERSION_OPERATOR(T, t) \
    operator T() { return MOCK_ANONYMOUS_MOCKER_EXT(this, operator T, t)(); } \
    MOCK_METHOD_EXPECTATION(T(), t)
#define MOCK_CONVERSION_OPERATOR(T, t) \
    operator T() const { return MOCK_ANONYMOUS_MOCKER_EXT(this, operator T, t)(); } \
    operator T() { return MOCK_ANONYMOUS_MOCKER_EXT(this, operator T, t)(); } \
    MOCK_METHOD_EXPECTATION(T(), t)

#define MOCK_EXPECT(o,t) MOCK_MOCKER(o,t).expect( __FILE__, __LINE__ )
#define MOCK_RESET(o,t) MOCK_MOCKER(o,t).reset()
#define MOCK_VERIFY(o,t) MOCK_MOCKER(o,t).verify()

// alternate experimental macros below, way too slow to compile to be really usable

namespace mock
{
namespace detail
{
#define MOCK_CALL_INVALID_TYPE(z, n, d) invalid_type
#define MOCK_CALL(z, n, d) \
    template< typename E > \
    BOOST_DEDUCED_TYPENAME boost::disable_if< \
    BOOST_DEDUCED_TYPENAME has_arity< E, n >::type, \
        BOOST_DEDUCED_TYPENAME E::result_type \
    >::type \
        call( E BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, MOCK_CALL_INVALID_TYPE, BOOST_PP_EMPTY) ) \
    {}
    BOOST_PP_REPEAT_FROM_TO(0, MOCK_NUM_ARGS, MOCK_CALL, BOOST_PP_EMPTY)
#undef MOCK_CALL
#undef MOCK_CALL_INVALID_TYPE
}
}

#define MOCK_METHOD_STUB_PROXY(z, n, d) \
    MOCK_METHOD_STUB( \
        BOOST_PP_ARRAY_ELEM(0, d), \
        n, \
        BOOST_PP_ARRAY_ELEM(1, d), \
        BOOST_PP_ARRAY_ELEM(2, d), \
        BOOST_PP_ARRAY_ELEM(3, d), \
        BOOST_PP_ARRAY_ELEM(4, d))

#define MOCK_METHOD_STUB_ALT(M, S, t, c, tpn) \
    BOOST_PP_REPEAT_FROM_TO(0, MOCK_NUM_ARGS, MOCK_METHOD_STUB_PROXY, (5,(M, S, t, c, tpn)))

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
