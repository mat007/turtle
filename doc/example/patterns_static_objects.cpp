// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2015
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ static_objects_problem
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>
#include <ostream>

namespace
{
    struct my_class
    {
        my_class( int i )
            : i_( i )
        {}

        int i_;
    };

    std::ostream& operator<<( std::ostream& os, const my_class* c )
    {
        return os << "my_class " << c->i_; // the 'c' pointer must be valid when logging
    }

    MOCK_FUNCTION( f, 1, void( my_class* ) ) // being static 'f' outlive the test case
}

BOOST_AUTO_TEST_CASE( static_objects_problem )
{
    my_class c( 42 );
    MOCK_EXPECT( f ).once().with( &c ); // the set expectation will also outlive the test case and leak into other test cases using 'f'
} // the 'c' instance goes out of scope and the '&c' pointer becomes dangling
//]

//[ static_objects_partial_solution
struct fixture
{
    ~fixture()
    {
        mock::reset(); // the use of a fixture ensures the reset will prevent the expectations from leaking into other test cases
    }
};

BOOST_FIXTURE_TEST_CASE( static_object_partial_solution, fixture )
{
    my_class c( 42 );
    MOCK_EXPECT( f ).once().with( &c );
    f( &c );
    mock::verify(); // verify the expectations before local objects are destroyed and before the fixture resets them
}
//]

//[ static_objects_solution
BOOST_FIXTURE_TEST_CASE( static_objects_solution, mock::cleanup ) // actually the library includes a ready to use fixture just like the one described
{
    my_class c( 42 );
    MOCK_EXPECT( f ).once().with( &c );
    f( &c );
    mock::verify();
}
//]
