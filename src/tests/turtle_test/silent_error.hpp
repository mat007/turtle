//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_TEST_SILENT_ERROR_HPP_INCLUDED
#define MOCK_TEST_SILENT_ERROR_HPP_INCLUDED

#include <string>
#include <stdexcept>

namespace mock
{
    template< typename Result >
    struct silent_error
    {
        static Result abort()
        {
            throw std::runtime_error( "abort" );
        }
        template< typename Context >
        static void expected_call( const Context& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
        template< typename Context >
        static void unexpected_call( const Context& /*context*/ )
        {}
        template< typename Context >
        static void missing_action( const Context& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
        template< typename Context >
        static void sequence_failed( const Context& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
        template< typename Context >
        static void verification_failed( const Context& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
        template< typename Context >
        static void untriggered_expectation( const Context& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
    };
}

#endif // #ifndef MOCK_TEST_SILENT_ERROR_HPP_INCLUDED
