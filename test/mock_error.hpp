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
    struct mock_error_data
    {
        void reset()
        {
            call_count = 0;
            error_count = 0;
            last_message.clear();
            last_context.clear();
        }
        bool verify()
        {
            return error_count == 0;
        }

        void call()
        {
            last_context.clear();
            last_message.clear();
            ++call_count;
        }
        void fail( const std::string& message,
            const std::string& context,
            const char* file, int line )
        {
            last_context = context;
            last_message = message;
            last_file = file;
            last_line = line;
            ++error_count;
        }

        int error_count;
        int call_count;
        std::string last_message;
        std::string last_context;
        std::string last_file;
        int last_line;
    };

    inline mock_error_data& data()
    {
        static mock_error_data p;
        return p;
    }

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
            data().call();
        }

        template< typename Context >
        static void fail( const std::string& message, const Context& context,
            const char* file = "", int line = 0 )
        {
            data().fail( message,
                boost::lexical_cast< std::string >( context ), file, line );
        }
    };
}

#endif // MOCK_TEST_MOCK_ERROR_HPP_INCLUDED
