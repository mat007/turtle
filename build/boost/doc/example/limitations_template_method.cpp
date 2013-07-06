#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

//[ limitations_template_method_problem
class concept
{
public:
    template< typename T >
    void method( T t )
    {}
};

template< typename T >
class client
{
public:
    client( T t )             // T is supposed to model the previous concept
    {
        t.method( 42 );
        t.method( "string" );
    }
};
//]
 
//[ limitations_template_method_solution
MOCK_CLASS( mock_concept )
{
    MOCK_METHOD( method, 1, void( int ), method_int )
    MOCK_METHOD( method, 1, void( const char* ), method_string )
};
//]

