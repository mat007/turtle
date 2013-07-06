#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

namespace
{
//[ limitations_const_parameter_warning_problem
    class base
    {
    public:
        virtual void method( const int ) = 0;
    };
//]
}

namespace limitations_const_parameter_warning_explanation
{
//[ limitations_const_parameter_warning_explanation
    class derived : public base
    {
    public:
        virtual void method( const int );
    };

    void derived::method( int )
    {}
//]
}

namespace limitations_const_parameter_warning_solution
{
//[ limitations_const_parameter_warning_solution
    MOCK_BASE_CLASS( mock_base, base )
    {
        void method( const int i )
        {
            method_stub( i );
        }
        MOCK_METHOD( method_stub, 1, void( int ), method )
    };
//]
}
