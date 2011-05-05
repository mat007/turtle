//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_ERROR_HPP_INCLUDED
#define MOCK_ERROR_HPP_INCLUDED

#include "config.hpp"
#ifdef MOCK_USE_BOOST_TEST
#include <boost/test/framework.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/exception/enable_current_exception.hpp>
#else
#include <iostream>
#endif // MOCK_USE_BOOST_TEST
#include <string>

namespace mock
{
#ifdef MOCK_USE_BOOST_TEST

    struct exception : public boost::execution_aborted
    {};

    template< typename Result >
    struct boost_test_error_policy
    {
        static Result abort()
        {
            boost::unit_test::framework::test_unit_aborted(
                boost::unit_test::framework::current_test_case() );
            throw boost::enable_current_exception( exception() );
        }

        template< typename Context >
        static void fail(
            const std::string& message, const Context& context,
            const std::string& file = "unknown location", int line = 0 )
        {
            boost::unit_test::framework::assertion_result( false );
            boost::unit_test::unit_test_log
                << boost::unit_test::log::begin( file, (std::size_t)line )
                << boost::unit_test::log_all_errors
                << message << ": " << context
                << boost::unit_test::log::end();
        }

        template< typename Context >
        static void expected_call( const Context& context,
            const std::string& file, int line )
        {
            boost::unit_test::framework::assertion_result( true );
            boost::unit_test::unit_test_log
                << boost::unit_test::log::begin( file, (std::size_t)line )
                << boost::unit_test::log_successful_tests
                << "mock expectation fulfilled: " << context
                << boost::unit_test::log::end();
        }

#else // MOCK_USE_BOOST_TEST

    struct exception
    {};

    template< typename Result >
    struct basic_error_policy
    {
        static Result abort()
        {
            throw exception();
        }

        template< typename Context >
        static void fail(
            const std::string& message, const Context& context,
            const std::string& file = "unknown location", int line = 0 )
        {
            std::cerr << file << '(' << line << "): "
                << message << ": " << context << std::endl;
        }

        template< typename Context >
        static void expected_call( const Context& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}

#endif // MOCK_USE_BOOST_TEST

        template< typename Context >
        static void missing_action( const Context& context,
            const std::string& file, int line )
        {
            fail( "missing action", context, file, line );
        }
        template< typename Context >
        static void unexpected_call( const Context& context )
        {
            fail( "unexpected call", context );
        }
        template< typename Context >
        static void sequence_failed( const Context& context,
            const std::string& /*file*/, int /*line*/ )
        {
            fail( "sequence failed", context );
        }
        template< typename Context >
        static void verification_failed( const Context& context,
            const std::string& file, int line )
        {
            fail( "verification failed", context, file, line );
        }
        template< typename Context >
        static void untriggered_expectation( const Context& context,
            const std::string& file, int line )
        {
            fail( "untriggered expectation", context, file, line );
        }
    };
}

#endif // MOCK_ERROR_HPP_INCLUDED
