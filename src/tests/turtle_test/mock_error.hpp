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
    int missing_result_specification_count = 0;
    int no_match_count = 0;
    int sequence_failed_count = 0;
    int verification_failed_count = 0;
    int untriggered_expectation_count = 0;
}
namespace mock
{
    template< typename Result >
    struct mock_error
    {
        static Result missing_result_specification( const std::string& /*context*/, const std::string& /*file*/, int /*line*/ )
        {
            ++missing_result_specification_count;
            static BOOST_DEDUCED_TYPENAME boost::remove_reference< Result >::type r;
            return r;
        }
        static Result no_match( const std::string& /*context*/ )
        {
            ++no_match_count;
            static BOOST_DEDUCED_TYPENAME boost::remove_reference< Result >::type r;
            return r;
        }
        static void sequence_failed( const std::string& /*context*/, const std::string& /*file*/, int /*line*/ )
        {
            ++sequence_failed_count;
        }
        static void verification_failed( const std::string& /*context*/, const std::string& /*file*/, int /*line*/ )
        {
            ++verification_failed_count;
        }
        static void untriggered_expectation( const std::string& /*context*/, const std::string& /*file*/, int /*line*/ )
        {
            ++untriggered_expectation_count;
        }
    };
    template<>
    struct mock_error< void >
    {
        static void missing_result_specification( const std::string& /*context*/, const std::string& /*file*/, int /*line*/ )
        {
            ++missing_result_specification_count;
        }
        static void no_match( const std::string& /*context*/ )
        {
            ++no_match_count;
        }
        static void sequence_failed( const std::string& /*context*/, const std::string& /*file*/, int /*line*/ )
        {
            ++sequence_failed_count;
        }
        static void verification_failed( const std::string& /*context*/, const std::string& /*file*/, int /*line*/ )
        {
            ++verification_failed_count;
        }
        static void untriggered_expectation( const std::string& /*context*/, const std::string& /*file*/, int /*line*/ )
        {
            ++untriggered_expectation_count;
        }
    };
}

#endif // #ifndef MOCK_TEST_MOCK_ERROR_HPP_INCLUDED
