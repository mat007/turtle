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
#endif // MOCK_USE_BOOST_TEST
#include <iostream>

namespace mock
{
#ifdef MOCK_USE_BOOST_TEST
    template< typename Result >
    struct boost_test_error_policy
    {
        static Result missing_result_specification( const std::string& context,
            const std::string& file, int line )
        {
            fail( "mock error : missing result specification : " + context,
                file, line );
            throw boost::enable_current_exception(
                boost::execution_aborted() );
        }

        static Result no_match( const std::string& context )
        {
            fail( "mock error : unexpected call : " + context,
                "unknown location", 0 );
            throw boost::enable_current_exception(
                boost::execution_aborted() );
        }

        static void sequence_failed( const std::string& context,
            const std::string& /*file*/, int /*line*/ )
        {
            fail( "mock error : sequence failed : " + context,
                "unknown location", 0 );
            throw boost::enable_current_exception(
                boost::execution_aborted() );
        }

        static void verification_failed( const std::string& context,
            const std::string& file, int line )
        {
            fail( "verification failed : " + context, file, line );
        }

        static void untriggered_expectation( const std::string& context,
            const std::string& file, int line )
        {
            fail( "untriggered expectation : " + context, file, line );
        }

        static void fail( const std::string& message,
            const std::string& file, int line )
        {
            boost::unit_test::framework::assertion_result( false );
            boost::unit_test::unit_test_log
                << boost::unit_test::log::begin( file, (std::size_t)line )
                << boost::unit_test::log_all_errors
                << boost::unit_test::lazy_ostream::instance() << message
                << boost::unit_test::log::end();
        }
    };
#endif // MOCK_USE_BOOST_TEST

    struct exception
    {};

    template< typename Result >
    struct basic_error_policy
    {
        static void log( const std::string& message,
            const std::string& context,
            const std::string& file = "unknown location", int line = 0 )
        {
            std::cerr << file << '(' << line << "): "
                << "mock error: " << message << ": " << context << std::endl;
        }

        static Result missing_result_specification( const std::string& context,
            const std::string& file, int line )
        {
            log( "missing result specification", context, file, line );
            throw exception();
        }

        static Result no_match( const std::string& context )
        {
            log( "unexpected call", context );
            throw exception();
        }

        static void sequence_failed( const std::string& context,
            const std::string& /*file*/, int /*line*/ )
        {
            log( "sequence failed", context );
            throw exception();
        }

        static void verification_failed( const std::string& context,
            const std::string& file, int line )
        {
            log( "verification failed", context, file, line );
        }

        static void untriggered_expectation( const std::string& context,
            const std::string& file, int line )
        {
            log( "untriggered expectation", context, file, line );
        }
    };
}

#endif // #ifndef MOCK_ERROR_HPP_INCLUDED
