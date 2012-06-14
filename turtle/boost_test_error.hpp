//
// Copyright Mathieu Champlon 2011
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

#ifndef MOCK_BOOST_TEST_ERROR_POLICY_HPP_INCLUDED
#define MOCK_BOOST_TEST_ERROR_POLICY_HPP_INCLUDED

#include <boost/test/framework.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/exception/enable_current_exception.hpp>

namespace mock
{
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

        static void checkpoint( const char* file, int line )
        {
            boost::unit_test::unit_test_log.set_checkpoint( file,
                static_cast< std::size_t >( line ) );
        }

        template< typename Context >
        static void fail(
            const char* message, const Context& context,
            const char* file = "unknown location", int line = 0 )
        {
            boost::unit_test::framework::assertion_result( false );
            boost::unit_test::unit_test_log
                << boost::unit_test::log::begin( file,
                    static_cast< std::size_t >( line ) )
                << boost::unit_test::log_all_errors
                << message << ": " << context
                << boost::unit_test::log::end();
        }

        template< typename Context >
        static void expected_call( const Context& context,
            const char* file, int line )
        {
            boost::unit_test::framework::assertion_result( true );
            boost::unit_test::unit_test_log
                << boost::unit_test::log::begin( file,
                    static_cast< std::size_t >( line ) )
                << boost::unit_test::log_successful_tests
                << "mock expectation fulfilled: " << context
                << boost::unit_test::log::end();
        }

        template< typename Context >
        static void missing_action( const Context& context,
            const char* file, int line )
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
            const char* file, int line )
        {
            fail( "sequence failed", context, file, line );
        }
        template< typename Context >
        static void verification_failed( const Context& context,
            const char* file, int line )
        {
            fail( "verification failed", context, file, line );
        }
        template< typename Context >
        static void untriggered_expectation( const Context& context,
            const char* file, int line )
        {
            fail( "untriggered expectation", context, file, line );
        }
    };
}

#endif // MOCK_BOOST_TEST_ERROR_POLICY_HPP_INCLUDED
