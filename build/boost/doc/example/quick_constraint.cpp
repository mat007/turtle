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

namespace mock // it could also be in the namespace of 'my_class'
{
    bool operator==( const my_class& actual, const std::string& expected ) // the trick is to compare to a string
    {
        return boost::lexical_cast< std::string >( actual ) == expected;
    }
}

BOOST_AUTO_TEST_CASE( method_is_called )
{
    my_mock mock;
    MOCK_EXPECT( mock.method ).once().with( "my_class( 42 )" ); // the constraint is given as a string
    mock.method( my_class( 42 ) );
}
//]
