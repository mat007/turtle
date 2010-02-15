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
#include <sstream>
#include <stdexcept>

namespace mock
{
    template< typename Result >
    struct silent_error
    {
        static std::string to_string( int i )
        {
            std::stringstream s;
            s << i;
            return s.str();
        }

        static Result abort()
        {
            throw std::runtime_error( "abort" );
        }
        static void no_match( const std::string& /*context*/ )
        {}
        static void missing_action( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
        static void sequence_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
        static void verification_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
        static void untriggered_expectation( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {}
    };
}

#endif // #ifndef MOCK_TEST_SILENT_ERROR_HPP_INCLUDED
