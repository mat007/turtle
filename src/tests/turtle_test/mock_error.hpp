//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_TEST_MOCK_ERROR_HPP_INCLUDED
#define MOCK_TEST_MOCK_ERROR_HPP_INCLUDED

#include <boost/type_traits/remove_reference.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <string>

namespace
{
    int missing_action_count = 0;
    int expected_call_count = 0;
    int unexpected_call_count = 0;
    int sequence_failed_count = 0;
    int verification_failed_count = 0;
    int untriggered_expectation_count = 0;

    std::string last_context;
}
namespace mock
{
    template< typename Result >
    struct mock_error
    {
        static Result abort()
        {
            throw std::runtime_error( "aborted" );
        }

        template< typename Context >
        static void missing_action( const Context& context,
            const std::string& /*file*/, int /*line*/ )
        {
            last_context = boost::lexical_cast< std::string >( context );
            ++missing_action_count;
        }
        template< typename Context >
        static void expected_call( const Context& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            last_context.clear();
            ++expected_call_count;
        }
        template< typename Context >
        static void unexpected_call( const Context& context )
        {
            last_context = boost::lexical_cast< std::string >( context );
            ++unexpected_call_count;
        }
        template< typename Context >
        static void sequence_failed( const Context& context,
            const std::string& /*file*/, int /*line*/ )
        {
            last_context = boost::lexical_cast< std::string >( context );
            ++sequence_failed_count;
        }
        template< typename Context >
        static void verification_failed( const Context& context,
            const std::string& /*file*/, int /*line*/ )
        {
            last_context = boost::lexical_cast< std::string >( context );
            ++verification_failed_count;
        }
        template< typename Context >
        static void untriggered_expectation( const Context& context,
            const std::string& /*file*/, int /*line*/ )
        {
            last_context = boost::lexical_cast< std::string >( context );
            ++untriggered_expectation_count;
        }
    };
}

#endif // MOCK_TEST_MOCK_ERROR_HPP_INCLUDED
