//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "silent_error.hpp"

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define MOCK_ERROR_POLICY silent_error
#include <turtle/mock.hpp>

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/ref.hpp>
#include <cmath>

namespace
{
    struct my_custom_mock
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_method )
    };
}

BOOST_AUTO_TEST_CASE( custom_mock_object_without_macros_and_without_inheriting_from_object )
{
    my_custom_mock m;
    MOCK_EXPECT( m, my_method ).once();
    m.my_method();
}

namespace
{
    struct my_custom_mock_object
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_method )
    };
}

BOOST_AUTO_TEST_CASE( custom_mock_object_without_macros )
{
    my_custom_mock_object m;
    MOCK_EXPECT( m, my_method ).once();
    m.my_method();
}

namespace
{
    MOCK_CLASS( my_mock )
    {
        MOCK_METHOD_EXT( my_method, 1, int( int ), my_method )
    };
}

BOOST_AUTO_TEST_CASE( basic_mock_object_usage )
{
    my_mock m;
    {
        MOCK_EXPECT( m, my_method ).once().returns( 0 );
        BOOST_CHECK_EQUAL( 0, m.my_method( 13 ) );
    }
    mock::verify();
    mock::reset();
    {
        MOCK_EXPECT( m, my_method ).once().with( 42 ).returns( 7 );
        BOOST_CHECK_EQUAL( 7, m.my_method( 42 ) );
    }
    mock::verify();
    mock::reset();
    {
        MOCK_EXPECT( m, my_method ).once().returns( 51 );
        BOOST_CHECK_EQUAL( 51, m.my_method( 27 ) );
    }
}

namespace
{
    class my_ambiguited_interface : boost::noncopyable
    {
    public:
        virtual ~my_ambiguited_interface() {}

        virtual void my_method() = 0;
        virtual void my_method( int ) = 0;
    };

    MOCK_BASE_CLASS( my_ambiguited_mock, my_ambiguited_interface )
    {
        MOCK_METHOD_EXT( my_method, 0, void(), tag1 )
        MOCK_METHOD_EXT( my_method, 1, void( int ), tag2 )
    };
}

BOOST_AUTO_TEST_CASE( mock_object_method_disambiguation )
{
    my_ambiguited_mock mock;
    MOCK_EXPECT( mock, tag1 );
    BOOST_CHECK_NO_THROW( mock.my_method() );
    BOOST_CHECK_THROW( mock.my_method( 12 ), std::exception );
}

namespace
{
    class my_const_ambiguited_interface : boost::noncopyable
    {
    public:
        virtual ~my_const_ambiguited_interface() {}

        virtual void my_method() = 0;
        virtual void my_method() const = 0;
    };

    MOCK_BASE_CLASS( my_const_ambiguited_mock, my_const_ambiguited_interface )
    {
        MOCK_NON_CONST_METHOD_EXT( my_method, 0, void(), tag1 )
        MOCK_CONST_METHOD_EXT( my_method, 0, void(), tag2 )
    };
}

BOOST_AUTO_TEST_CASE( mock_object_method_const_disambiguation )
{
    my_const_ambiguited_mock mock;
    MOCK_EXPECT( mock, tag1 );
    BOOST_CHECK_NO_THROW( mock.my_method() );
    const my_const_ambiguited_mock const_mock;
    BOOST_CHECK_THROW( const_mock.my_method(), std::exception );
}

namespace
{
    struct my_declared_but_undefined_type;

    MOCK_CLASS( my_declared_but_undefined_mock )
    {
        MOCK_METHOD_EXT( m, 1, void( my_declared_but_undefined_type& ), m )
    };
}

BOOST_AUTO_TEST_CASE( mock_object_method_with_declared_but_not_defined_parameter_is_valid )
{
    my_declared_but_undefined_mock mock;
    MOCK_EXPECT( mock, m );
}

BOOST_AUTO_TEST_CASE( mock_functor_in_function_is_supported )
{
    boost::function< int( float, const std::string& ) > func;
    {
        MOCK_FUNCTOR( int( float, const std::string& ) ) f;
        MOCK_EXPECT( f, _ ).once().with( 3, "op" ).returns( 42 );
        func = f;
    }
    BOOST_CHECK_EQUAL( 42, func( 3, "op" ) );
}

namespace
{
    struct functor_fixture
    {
        MOCK_FUNCTOR( int( float, const std::string& ) ) f;
    };
}

BOOST_FIXTURE_TEST_CASE( mock_functor_in_fixture_is_supported, functor_fixture )
{
    MOCK_EXPECT( f, _ ).once().with( 3, "op" ).returns( 42 );
    BOOST_CHECK_EQUAL( 42, f( 3.f, "op" ) );
}

namespace
{
    template< typename T >
    struct my_template_mock
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_method )
        MOCK_METHOD_EXT_TPL( my_method, 2, void( T, std::string ), my_method_t )
        MOCK_METHOD_EXT_TPL( my_other_method, 0, void(), my_other_method )
    };
}

BOOST_AUTO_TEST_CASE( mocking_a_template_class_method_is_supported )
{
    my_template_mock< int > m;
    MOCK_EXPECT( m, my_method_t ).with( 3, "" );
    m.my_method( 3, "" );
}

namespace
{
    template< typename T >
    struct my_template_base_class
    {
        virtual ~my_template_base_class()
        {}
        virtual void my_method( T ) = 0;
        virtual void my_other_method() = 0;
    };
    template< typename T >
    MOCK_BASE_CLASS( my_template_base_class_mock, my_template_base_class< T > )
    {
        MOCK_METHOD_EXT_TPL( my_method, 1, void( T ), my_method )
        MOCK_METHOD_EXT_TPL( my_other_method, 0, void(), my_other_method )
    };
}

BOOST_AUTO_TEST_CASE( mocking_a_template_base_class_method_is_supported )
{
    my_template_base_class_mock< int > m;
    MOCK_EXPECT( m, my_method ).once().with( 3 );
    m.my_method( 3 );
}

namespace
{
    class my_observer : boost::noncopyable
    {
    public:
        virtual ~my_observer() {}

        virtual void notify( int value ) = 0;
    };

    class my_manager : boost::noncopyable
    {
    public:
        virtual ~my_manager() {}

        virtual my_observer& get_observer() const = 0;
    };

    class my_subject : boost::noncopyable
    {
    public:
        explicit my_subject( my_manager& f )
            : o_( f.get_observer() )
            , value_( 0 )
        {}
        void increment()
        {
            o_.notify( ++value_ );
        }
    private:
        my_observer& o_;
        int value_;
    };

    MOCK_BASE_CLASS( my_mock_observer, my_observer )
    {
        MOCK_METHOD( notify, 1 )
    };

    MOCK_BASE_CLASS( my_mock_manager, my_manager )
    {
        MOCK_METHOD( get_observer, 0 )
    };

    struct fixture
    {
        my_mock_manager manager;
        my_mock_observer observer;
    };
}

BOOST_FIXTURE_TEST_CASE( basic_mock_object_collaboration_usage, fixture )
{
    MOCK_EXPECT( manager, get_observer ).returns( boost::ref( observer ) );
    my_subject subject( manager );
    MOCK_EXPECT( observer, notify ).once().with( 1 );
    subject.increment();
    MOCK_EXPECT( observer, notify ).once().with( 2 );
    subject.increment();
    MOCK_EXPECT( observer, notify ).once().with( 3 );
    subject.increment();
}

namespace
{
    MOCK_CLASS( my_destroyed_class )
    {
        MOCK_DESTRUCTOR( my_destroyed_class, destructor )
    };
}

BOOST_AUTO_TEST_CASE( mocking_a_destructor )
{
    my_destroyed_class c;
    MOCK_EXPECT( c, destructor ).once();
}

BOOST_AUTO_TEST_CASE( failed_expectation_in_mocked_destructor_does_not_throw )
{
    try
    {
        my_destroyed_class c;
        throw std::runtime_error( "should not crash" );
    }
    catch( std::runtime_error& )
    {
    }
}

BOOST_AUTO_TEST_CASE( failed_sequence_in_mocked_destructor_does_not_throw )
{
    mock::sequence s;
    my_custom_mock m;
    {
        my_destroyed_class c;
        MOCK_EXPECT( c, destructor ).once().in( s );
        MOCK_EXPECT( m, my_method ).once().in( s );
        m.my_method();
    }
}

namespace
{
    MOCK_CLASS( boost_optional )
    {
        MOCK_METHOD_EXT( method, 0, boost::optional< my_observer& >(), method )
    };
}

BOOST_AUTO_TEST_CASE( boost_optional_on_base_class_reference_as_return_type_is_supported )
{
    boost_optional b;
    my_mock_observer o;
    MOCK_EXPECT( b, method ).once().returns( boost::ref( o ) );
    b.method();
}

namespace
{
    bool serialized = false;

    struct custom_argument
    {
        friend std::ostream& operator<<( std::ostream& s, custom_argument )
        {
            serialized = true;
            return s;
        }
    };
    struct custom_constraint
    {
        template< typename Actual >
        friend bool operator==( Actual, custom_constraint )
        {
            return true;
        }
        friend std::ostream& operator<<( std::ostream& s, custom_constraint )
        {
            serialized = true;
            return s;
        }
    };
}

BOOST_AUTO_TEST_CASE( constraints_and_arguments_are_serialized_lazily )
{
    MOCK_FUNCTOR( void( custom_argument ) ) f;
    MOCK_EXPECT( f, _ ).with( custom_constraint() );
    f( custom_argument() );
    BOOST_CHECK( ! serialized );
}

namespace
{
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
            return std::abs( actual - boost::unwrap_ref( expected_ ) )
                < boost::unwrap_ref( threshold_ );
        }

        friend std::ostream& operator<<( std::ostream& s, const near_constraint& c )
        {
            return s << "near( " << mock::format( c.expected_ )
                << ", " << mock::format( c.threshold_ ) << " )";
        }

        //template< typename Actual >
        //void explain( std::ostream& s, Actual actual ) const
        //{
        //    s << std::abs( actual - expected_ ) << " >= " << threshold_;
        //}

        Expected expected_;
        Expected threshold_;
    };
    template< typename Expected >
    mock::constraint< near_constraint< Expected > > near( Expected expected, Expected threshold )
    {
        return near_constraint< Expected >( expected, threshold );
    }
}

BOOST_AUTO_TEST_CASE( using_custom_constraint )
{
    MOCK_FUNCTOR( void( float ) ) f;
    MOCK_EXPECT( f, _ ).with( near( 3.f, 0.01f ) );
    f( 3.f );
    const std::string expected = "f\n"
                                 ". unlimited().with( near( 3, 0.01 ) )";
    std::stringstream s;
    s << f;
    BOOST_CHECK_EQUAL( expected, s.str() );
}

BOOST_AUTO_TEST_CASE( boost_reference_wrapper_is_supported_in_value_constraint )
{
    MOCK_FUNCTOR( void( const std::string& ) ) f;
    std::string s;
    MOCK_EXPECT( f, _ ).once().with( boost::cref( s ) );
    s = "string";
    f( "string" );
}
