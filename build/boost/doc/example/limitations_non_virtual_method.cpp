#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

//[ limitations_non_virtual_method_problem
class base
{
public:
    void method() // the method is not virtual
    {}
};
//]

//[ limitations_non_virtual_method_problem_2
MOCK_BASE_CLASS( mock_base, base )
{
    MOCK_METHOD( method, 0 )
};
//]
