//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_ERROR_HPP_INCLUDED
#define MOCK_ERROR_HPP_INCLUDED

#include <boost/test/test_tools.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/test/utils/trivial_singleton.hpp>
#include <boost/exception/enable_current_exception.hpp>

namespace mock
{
namespace detail
{
    class errors_t : public boost::unit_test::singleton< errors_t >
    {
    private:
        friend class boost::unit_test::singleton< errors_t >;
    };
    BOOST_TEST_SINGLETON_INST( errors )
}

    class exception : public boost::execution_exception
    {
    public:
        explicit exception( const std::string& s )
            : boost::execution_exception( boost::execution_exception::user_error, s )
        {}
    };

    template< typename Result >
    struct boost_test_error_policy
    {
        static void missing_result_specification()
        {
            static std::string m;
            m = "mock error : missing result specification";
            throw boost::enable_current_exception( mock::exception( m ) );
        }

        static Result no_match( const std::string& context )
        {
            static std::string m;
            m = "mock error : unexpected call : " + context;
            throw boost::enable_current_exception( mock::exception( m ) );
        }

        static void sequence_failed( const std::string& context,
            const std::string& /*file*/, int /*line*/ )
        {
            static std::string m;
            m = "mock error : sequence failed : " + context;
            throw boost::enable_current_exception( mock::exception( m ) );
        }

        static void verification_failed( const std::string& context,
            const std::string& file, int line )
        {
            notify( "verification failed : " + context, file, line );
        }

        static void untriggered_expectation( const std::string& context,
            const std::string& file, int line )
        {
            notify( "untriggered expectation : " + context, file, line );
        }

        static void notify( const std::string& message,
            const std::string& file, int line )
        {
            boost::test_tools::tt_detail::check_impl(
                false,
                boost::unit_test::lazy_ostream::instance() << message,
                file, (std::size_t)line,
                boost::test_tools::tt_detail::CHECK,
                boost::test_tools::tt_detail::CHECK_MSG,
                0 );
        }
    };
}

#endif // #ifndef MOCK_ERROR_HPP_INCLUDED
