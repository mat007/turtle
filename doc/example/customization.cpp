// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ mock_use_conversions
#define MOCK_USE_CONVERSIONS
#include <turtle/mock.hpp>
//]
#include "calculator.hpp"
#include "mock_view.hpp"
#include <boost/test/unit_test.hpp>

//[ mock_stream_user_type
namespace user_namespace
{
    struct user_type
    {};

    inline mock::stream& operator<<( mock::stream& s, const user_type& )
    {
        return s << "user_type";
    }
}
//]

namespace custom_constraint_free_function_test
{
//[ custom_constraint_free_function
bool custom_constraint( int actual )
{
    return actual == 42;
}
//]

//[ custom_constraint_free_function_test
BOOST_AUTO_TEST_CASE( forty_one_plus_one_is_forty_two_free_function )
{
    mock_view v;
    calculator c( v );
    MOCK_EXPECT( v.display ).with( &custom_constraint );
    c.add( 41, 1 );
 }
//]
}

namespace custom_constraint_functor_test
{
//[ custom_constraint_functor
struct custom_constraint
{
    friend bool operator==( int actual, const custom_constraint& )
    {
        return actual == 42;
    }

    friend std::ostream& operator<<( std::ostream& s, const custom_constraint& )
    {
        return s << "_ == 42";
    }
};
//]

//[ custom_constraint_functor_test
BOOST_AUTO_TEST_CASE( forty_one_plus_one_is_forty_two_custom_constraint )
{
    mock_view v;
    calculator c( v );
    MOCK_EXPECT( v.display ).with( custom_constraint() );
    c.add( 41, 1 );
}
//]
}

//[ near_constraint
template< typename Expected >
struct near_constraint
{
    near_constraint( Expected expected, Expected threshold )
      : expected_( expected )
      , threshold_( threshold )
    {}

    template< typename Actual >
    bool operator()( Actual actual ) const
    {
        return std::abs( actual - mock::unwrap_ref(expected_) ) <= mock::unwrap_ref(threshold_) ;
    }

    friend std::ostream& operator<<( std::ostream& s, const near_constraint& c )
    {
        return s << "near( " << mock::format( c.expected_ )
            << ", " << mock::format( c.threshold_ ) << " )";
    }

    Expected expected_, threshold_;
};

template< typename Expected >
mock::constraint< near_constraint< Expected > > near( Expected expected, Expected threshold )
{
    return near_constraint< Expected >( expected, threshold );
}
//]

namespace near_constraint_test
{
//[ near_constraint_test
BOOST_AUTO_TEST_CASE( forty_one_plus_one_is_forty_two_plus_or_minus_one_near )
{
   mock_view v;
   calculator c( v );
   MOCK_EXPECT( v.display ).with( near( 42, 1 ) );
   c.add( 41, 1 );
}
//]
}

namespace near_constraint_cref_test
{
//[ near_constraint_cref_test
BOOST_AUTO_TEST_CASE( forty_one_plus_one_is_forty_two_plus_or_minus_one_near_cref )
{
   mock_view v;
   calculator c( v );
   int expected = 0, threshold = 0;
   MOCK_EXPECT( v.display ).with( near( std::cref( expected ), std::cref( threshold ) ) );
   expected = 42;
   threshold = 1;
   c.add( 41, 1 );
}
//]

// Example of a "strong type" float
struct float_wrapper{
    float value;
    float_wrapper(float value): value(value){}
    operator float() const { return value; }
    friend std::ostream& operator<<( std::ostream& s, const float_wrapper& f)
    {
        return s << f.value;
    }
};

BOOST_AUTO_TEST_CASE( near_constraint_works_with_with_float_wrapper_and_cref )
{
   mock_view v;
   calculator c( v );
   float_wrapper expected = 0, threshold = 0;
   // This works even without the unwrap_ref
   MOCK_EXPECT( v.display ).once().with( near( expected, threshold ) );
   // This requires 2 implicit conversion: from reference_wrapper to float_wrapper, then to float
   // so unwrap_ref in near is required as C++ allows only 1 implicit conversion
   MOCK_EXPECT( v.display ).once().with( near( std::cref( expected ), std::cref( threshold ) ) );
   expected = 42;
   threshold = 1;
   c.add(0, 0);
   c.add(41, 1);
}
}

#undef MOCK_MAX_ARGS
//[ max_args
#define MOCK_MAX_ARGS 20
#include <turtle/mock.hpp>
//]

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

//[ custom_policy
template< typename Result >
struct custom_policy
{
    static Result abort()
    {
        // Notify the test framework that an error occurs which makes it impossible to continue the test.
        // This should most likely throw an exception of some kind.
    }
    template< typename Context >
    static void fail( const char* message, const Context& context, const char* file = "unknown location", int line = 0 )
    {
        // Notify the test framework that an unexpected call has occurred.
    }
    template< typename Context >
    static void call( const Context& context, const char* file, int line )
    {
        // Notify the test framework that an expectation has been fulfilled.
    }
    static void pass( const char* file, int line )
    {
        // Notify the test framework that the test execution merely passed the given code location.
    }
};
//]

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#undef MOCK_ERROR_POLICY
//[ define_custom_policy
#define MOCK_ERROR_POLICY custom_policy
#include <turtle/mock.hpp>
//]

//[ thread_safe
#define MOCK_THREAD_SAFE
#include <turtle/mock.hpp>
//]
