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
#include "cleanup.hpp"
#include "object.hpp"
#include "function.hpp"
#include "type_name.hpp"
#include "args.hpp"
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/single_view.hpp>
#include <boost/mpl/pop_front.hpp>
#define BOOST_TYPEOF_SILENT
#include <boost/typeof/typeof.hpp>
#include <stdexcept>

namespace mock
{
namespace detail
{
    template< typename T >
    T& deref( T& t )
    {
        return t;
    }
    template< typename T >
    T& deref( T* t )
    {
        if( ! t )
            throw std::invalid_argument( "derefencing null pointer" );
        return *t;
    }
    template< typename T >
    T& deref( std::auto_ptr< T >& t )
    {
        if( ! t.get() )
            throw std::invalid_argument( "derefencing null pointer" );
        return *t;
    }
    template< typename T >
    T& deref( const std::auto_ptr< T >& t )
    {
        if( ! t.get() )
            throw std::invalid_argument( "derefencing null pointer" );
        return *t;
    }
    template< typename T >
    T& deref( boost::shared_ptr< T > t )
    {
        if( ! t.get() )
            throw std::invalid_argument( "derefencing null pointer" );
        return *t;
    }

    template< typename M >
    struct signature :
        boost::function_types::function_type<
            boost::mpl::joint_view<
                boost::mpl::single_view<
                    BOOST_DEDUCED_TYPENAME
                        boost::function_types::result_type< M >::type
                >,
                BOOST_DEDUCED_TYPENAME boost::mpl::pop_front<
                    BOOST_DEDUCED_TYPENAME
                        boost::function_types::parameter_types< M >
                >::type
            >
        >
    {};

    template< typename T >
    struct base
    {
        typedef T base_type;
    };
}
}

#define MOCK_BASE_CLASS(T, I) \
    struct T : I, mock::object, mock::detail::base< I >
#define MOCK_CLASS(T) \
    struct T : mock::object
#define MOCK_FUNCTOR(S) \
    mock::function< S >

#define MOCK_MOCKER(o, t) \
    mock::detail::deref( o ).t##configure( mock::detail::root, \
        BOOST_PP_STRINGIZE(o) )
#define MOCK_ANONYMOUS_MOCKER(o, t) \
    mock::detail::deref( o ).t##configure( mock::detail::root, "?" )

#define MOCK_METHOD_EXPECTATION(S, t) \
    mutable mock::function< S > t##expectation; \
    mock::function< S >& t##configure( const mock::detail::context&, \
        const std::string& instance ) const \
    { \
        mock::detail::configure( *this, t##expectation, instance, \
            mock::detail::type_name( typeid( *this ) ), \
            BOOST_PP_STRINGIZE(t) ); \
        return t##expectation; \
    }

#define MOCK_SIGNATURE(M) \
    mock::detail::signature< BOOST_TYPEOF(&base_type::M) >::type // $$$$ MAT inline mock::detail::signature

#define MOCK_METHOD_STUB(M, n, S, t, c, tpn) \
    MOCK_DECL(M, n, S, c, tpn) \
    { \
        return MOCK_ANONYMOUS_MOCKER(this, t)( \
            BOOST_PP_ENUM_PARAMS(n, p) ); \
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

#define MOCK_DESTRUCTOR(T, t) \
    ~T() { MOCK_ANONYMOUS_MOCKER(this, t).test(); } \
    MOCK_METHOD_EXPECTATION(void(), t)

#define MOCK_CONST_CONVERSION_OPERATOR(T, t) \
    operator T() const { return MOCK_ANONYMOUS_MOCKER(this, t)(); } \
    MOCK_METHOD_EXPECTATION(T(), t)
#define MOCK_NON_CONST_CONVERSION_OPERATOR(T, t) \
    operator T() { return MOCK_ANONYMOUS_MOCKER(this, t)(); } \
    MOCK_METHOD_EXPECTATION(T(), t)
#define MOCK_CONVERSION_OPERATOR(T, t) \
    operator T() const { return MOCK_ANONYMOUS_MOCKER(this, t)(); } \
    operator T() { return MOCK_ANONYMOUS_MOCKER(this, t)(); } \
    MOCK_METHOD_EXPECTATION(T(), t)

#define MOCK_EXPECT(o,t) MOCK_MOCKER(o,t).expect( __FILE__, __LINE__ )
#define MOCK_RESET(o,t) MOCK_MOCKER(o,t).reset()
#define MOCK_VERIFY(o,t) MOCK_MOCKER(o,t).verify()

#endif // MOCK_MOCK_HPP_INCLUDED
