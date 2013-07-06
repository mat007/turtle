#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

namespace
{
//[ limitations_throw_specifier_problem
    struct base_class
    {
        virtual ~base_class()
        {}

        virtual void method() throw ();
    };
//]

//[ limitations_throw_specifier_solution
    MOCK_BASE_CLASS( mock_class, base_class )
    {
        void method() throw ()
        {
            method_proxy();
        }
        MOCK_METHOD( method_proxy, 0, void(), method )
    };
//]
}
