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
#include <string>

namespace
{
    int missing_action_count = 0;
    int unexpected_call_count = 0;
    int sequence_failed_count = 0;
    int verification_failed_count = 0;
    int untriggered_expectation_count = 0;
}
namespace mock
{
    template< typename Result >
    struct mock_error
    {
        static Result abort()
        {
            static BOOST_DEDUCED_TYPENAME boost::remove_reference< Result >::type r;
            return r;
        }

        static void missing_action( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            ++missing_action_count;
        }
        static void unexpected_call( const std::string& /*context*/ )
        {
            ++unexpected_call_count;
        }
        static void sequence_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            ++sequence_failed_count;
        }
        static void verification_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            ++verification_failed_count;
        }
        static void untriggered_expectation( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            ++untriggered_expectation_count;
        }
    };
    template<>
    struct mock_error< void >
    {
        static void abort()
        {}
        static void missing_action( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            ++missing_action_count;
        }
        static void unexpected_call( const std::string& /*context*/ )
        {
            ++unexpected_call_count;
        }
        static void sequence_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            ++sequence_failed_count;
        }
        static void verification_failed( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            ++verification_failed_count;
        }
        static void untriggered_expectation( const std::string& /*context*/,
            const std::string& /*file*/, int /*line*/ )
        {
            ++untriggered_expectation_count;
        }
    };
}

#endif // #ifndef MOCK_TEST_MOCK_ERROR_HPP_INCLUDED
