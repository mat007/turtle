// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_TEST_MOCK_ERROR_HPP_INCLUDED
#define MOCK_TEST_MOCK_ERROR_HPP_INCLUDED

#define MOCK_ERROR_POLICY mock_error
#include <boost/test/test_tools.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>

namespace
{
    int error_count = 0;
    int call_count = 0;

    std::string last_message;
    std::string last_context;

    template< typename Result >
    struct mock_error
    {
        static Result abort()
        {
            throw std::runtime_error( "aborted" );
        }

        static void pass( const char* /*file*/, int /*line*/ )
        {}

        template< typename Context >
        static void call( const Context& /*context*/,
            const char* /*file*/, int /*line*/ )
        {
            last_context.clear();
            last_message.clear();
            ++call_count;
        }

        template< typename Context >
        static void fail( const std::string& message, const Context& context,
            const char* /*file*/ = "", int /*line*/ = 0 )
        {
            last_context = boost::lexical_cast< std::string >( context );
            last_message = message;
            ++error_count;
        }
    };

    struct error_fixture
    {
        error_fixture()
        {
            reset();
        }
        ~error_fixture()
        {
            BOOST_CHECK( verify() );
            BOOST_CHECK_EQUAL( 0, call_count );
        }
        void reset()
        {
            call_count = 0;
            error_count = 0;
            last_message.clear();
            last_context.clear();
        }
        bool verify() const
        {
            return error_count == 0;
        }
    };
}

#define CHECK_CALLS( calls ) \
    BOOST_CHECK_EQUAL( calls, call_count ); \
    call_count = 0;
#define CHECK_ERROR( expr, error, calls, context ) \
    BOOST_CHECK( verify() ); \
    try { expr; } catch( ... ) {} \
    BOOST_CHECK_EQUAL( 1, error_count ); \
    BOOST_CHECK_EQUAL( error, last_message ); \
    BOOST_CHECK_EQUAL( context, last_context ); \
    CHECK_CALLS( calls ); \
    reset();

#endif // MOCK_TEST_MOCK_ERROR_HPP_INCLUDED
