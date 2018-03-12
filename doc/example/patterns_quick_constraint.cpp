// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ quick_constraint_problem
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>
#include <iostream>

namespace
{
    class my_class
    {
    public:
        explicit my_class( int data )
            : data_( data )
        {}
        int data_;
    };

    std::ostream& operator<<( std::ostream& os, const my_class& c ) // my_class is serializable to an std::ostream
    {
        return os << "my_class( " << c.data_ << " )";
    }

    MOCK_CLASS( my_mock )
    {
        MOCK_METHOD( method, 1, void( const my_class& ) ) // how to simply write a custom constraint ?
    };
}
//]

//[ quick_constraint_solution
#include <boost/lexical_cast.hpp>

namespace // in the same namespace as 'my_class'
{
    bool operator==( const my_class& actual, const std::string& expected ) // the first part of the trick is to compare to a string
    {
        return boost::lexical_cast< std::string >( actual ) == expected;
    }
} // mock

BOOST_AUTO_TEST_CASE( method_is_called )
{
    my_mock mock;
    MOCK_EXPECT( mock.method ).once().with( "my_class( 42 )" ); // the second part of the trick is to express the constraint as a string
    mock.method( my_class( 42 ) );
}
//]
