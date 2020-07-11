// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ retrieve_cref_problem
namespace mock_test
{
    class base_class
    {
    public:
        virtual void method( int value ) = 0;
    };

    class my_class
    {
        base_class& b;
    public:
        explicit my_class( base_class& );

        void process(); // the processing will call 'method' two times with the same value, but we don't know what value beforehand
    };
}
//]

namespace mock_test
{
    my_class::my_class( base_class& b): b(b){}
    void my_class::process()
    {
        int secret_value = 42;
        b.method(secret_value);
        b.method(secret_value);
    }
}

//[ retrieve_cref_solution
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

namespace mock_test
{
    MOCK_BASE_CLASS( mock_base_class, base_class )
    {
        MOCK_METHOD( method, 1 )
    };
}

BOOST_AUTO_TEST_CASE( method_is_called_two_times_with_the_same_value )
{
    using namespace mock_test;
    mock_base_class mock;
    my_class c( mock );
    int value;
    MOCK_EXPECT( mock.method ).once().with( mock::retrieve( value ) ); // on first call retrieve the value, this expectation takes precedence because it can never fail
    MOCK_EXPECT( mock.method ).once().with( std::cref( value ) );    // on second call compare the previously retrieved value with the newly received one
    c.process();
}
//]
