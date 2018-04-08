// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "mock_error.hpp"
#include "undefined.hpp"
#include <turtle/mock.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/ref.hpp>
#include <cmath>

namespace
{
    struct my_custom_mock
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_tag )
    };
}

BOOST_FIXTURE_TEST_CASE( custom_mock_object_without_macros_and_without_inheriting_from_object, mock_error_fixture )
{
    my_custom_mock m;
    MOCK_EXPECT( m.my_tag ).once();
    m.my_method();
    CHECK_CALLS( 1 );
}

namespace
{
    struct my_custom_mock_object
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_tag )
    };
}

BOOST_FIXTURE_TEST_CASE( custom_mock_object_without_macros, mock_error_fixture )
{
    my_custom_mock_object m;
    MOCK_EXPECT( m.my_tag ).once();
    m.my_method();
    CHECK_CALLS( 1 );
}

namespace
{
    MOCK_CLASS( my_mock )
    {
        MOCK_METHOD_EXT( my_method, 1, int( int ), my_tag )
    };
}

BOOST_FIXTURE_TEST_CASE( basic_mock_object_usage, mock_error_fixture )
{
    my_mock m;
    {
        MOCK_EXPECT( m.my_tag ).once().returns( 0 );
        BOOST_CHECK_EQUAL( 0, m.my_method( 13 ) );
        CHECK_CALLS( 1 );
    }
    mock::verify();
    mock::reset();
    {
        MOCK_EXPECT( m.my_tag ).once().with( 42 ).returns( 7 );
        BOOST_CHECK_EQUAL( 7, m.my_method( 42 ) );
        CHECK_CALLS( 1 );
    }
    mock::verify();
    mock::reset();
    {
        MOCK_EXPECT( m.my_tag ).once().returns( 51 );
        BOOST_CHECK_EQUAL( 51, m.my_method( 27 ) );
        CHECK_CALLS( 1 );
    }
}

namespace
{
    class my_ambiguited_interface : boost::noncopyable
    {
    public:
        virtual ~my_ambiguited_interface()
        {}
        virtual void my_method() = 0;
        virtual void my_method( int ) = 0;
    };

    MOCK_BASE_CLASS( my_ambiguited_mock, my_ambiguited_interface )
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_tag1 )
        MOCK_METHOD_EXT( my_method, 1, void( int ), my_tag_2 )
    };
}

BOOST_FIXTURE_TEST_CASE( mock_object_method_disambiguation, mock_error_fixture )
{
    my_ambiguited_mock m;
    MOCK_EXPECT( m.my_tag1 );
    m.my_method();
    CHECK_ERROR( m.my_method( 12 ), "unexpected call", 1, "?.my_ambiguited_mock::my_tag_2( 12 )" );
}

namespace
{
    class my_const_ambiguited_interface : boost::noncopyable
    {
    public:
        virtual ~my_const_ambiguited_interface()
        {}
        virtual void my_method() = 0;
        virtual void my_method() const = 0;
    };

    MOCK_BASE_CLASS( my_const_ambiguited_mock, my_const_ambiguited_interface )
    {
        MOCK_NON_CONST_METHOD_EXT( my_method, 0, void(), tag1 )
        MOCK_CONST_METHOD_EXT( my_method, 0, void(), tag_2 )
    };
}

BOOST_FIXTURE_TEST_CASE( mock_object_method_const_disambiguation, mock_error_fixture )
{
    my_const_ambiguited_mock mock;
    MOCK_EXPECT( mock.tag1 );
    BOOST_CHECK_NO_THROW( mock.my_method() );
    const my_const_ambiguited_mock const_mock;
    CHECK_ERROR( const_mock.my_method(), "unexpected call", 1, "?.my_const_ambiguited_mock::tag_2()" );
}

namespace
{
    MOCK_CLASS( my_undefined_mock )
    {
        MOCK_METHOD_EXT( m, 1, void( undefined& ), t )
    };
}

BOOST_FIXTURE_TEST_CASE( mock_object_method_with_declared_but_not_defined_parameter_is_valid, mock_error_fixture )
{
    my_undefined_mock mock;
    MOCK_EXPECT( mock.t );
}

namespace
{
    MOCK_FUNCTOR( gf, int( float, const std::string& ) );
}

BOOST_FIXTURE_TEST_CASE( mock_functor_in_namespace_is_supported, mock_error_fixture )
{
    boost::function< int( float, const std::string& ) > func;
    MOCK_EXPECT( gf ).once().with( 3, "op" ).returns( 42 );
    func = gf;
    BOOST_CHECK_EQUAL( 42, func( 3, "op" ) );
    CHECK_CALLS( 1 );
}

BOOST_FIXTURE_TEST_CASE( mock_functor_in_function_is_supported, mock_error_fixture )
{
    boost::function< int( float, const std::string& ) > func;
    {
        MOCK_FUNCTOR( f, int( float, const std::string& ) );
        MOCK_EXPECT( f ).once().with( 3, "op" ).returns( 42 );
        func = f;
    }
    BOOST_CHECK_EQUAL( 42, func( 3, "op" ) );
    CHECK_CALLS( 1 );
}

namespace
{
    struct functor_fixture : mock_error_fixture
    {
        MOCK_FUNCTOR( f, int( float, const std::string& ) );
    };
}

BOOST_FIXTURE_TEST_CASE( mock_functor_in_fixture_is_supported, functor_fixture )
{
    MOCK_EXPECT( f ).once().with( 3, "op" ).returns( 42 );
    BOOST_CHECK_EQUAL( 42, f( 3.f, "op" ) );
    CHECK_CALLS( 1 );
}

namespace
{
    template< typename T >
    struct my_template_mock
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_tag )
        MOCK_METHOD_EXT_TPL( my_method, 2, void( T, std::string ), my_tpl_tag )
        MOCK_METHOD_EXT_TPL( my_other_method, 0, void(), my_other_tag )
    };
}

BOOST_FIXTURE_TEST_CASE( mocking_a_template_class_method_is_supported, mock_error_fixture )
{
    my_template_mock< int > m;
    MOCK_EXPECT( m.my_tpl_tag ).with( 3, "" );
    m.my_method( 3, "" );
    BOOST_CHECK( MOCK_VERIFY( m.my_tpl_tag ) );
    CHECK_CALLS( 1 );
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

BOOST_FIXTURE_TEST_CASE( mocking_a_template_base_class_method_is_supported, mock_error_fixture )
{
    my_template_base_class_mock< int > m;
    MOCK_EXPECT( m.my_method ).once().with( 3 );
    m.my_method( 3 );
    BOOST_CHECK( MOCK_VERIFY( m.my_method ) );
    CHECK_CALLS( 1 );
}

namespace
{
    class my_observer : boost::noncopyable
    {
    public:
        virtual ~my_observer()
        {}
        virtual void notify( int value ) = 0;
    };

    class my_manager : boost::noncopyable
    {
    public:
        virtual ~my_manager()
        {}
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

    struct fixture : mock_error_fixture
    {
        my_mock_manager manager;
        my_mock_observer observer;
    };
}

BOOST_FIXTURE_TEST_CASE( basic_mock_object_collaboration_usage, fixture )
{
    MOCK_EXPECT( manager.get_observer ).returns( boost::ref( observer ) );
    my_subject subject( manager );
    MOCK_EXPECT( observer.notify ).once().with( 1 );
    subject.increment();
    MOCK_EXPECT( observer.notify ).once().with( 2 );
    subject.increment();
    MOCK_EXPECT( observer.notify ).once().with( 3 );
    subject.increment();
    CHECK_CALLS( 4 );
}

namespace
{
    MOCK_CLASS( my_constructed_class )
    {
        MOCK_CONSTRUCTOR( my_constructed_class, 2, ( int, const std::string& ), constructor )
    };
}

BOOST_FIXTURE_TEST_CASE( mocking_a_constructor, mock_error_fixture )
{
    MOCK_EXPECT( my_constructed_class::constructor ).with( 42, "some text" ).once();
    my_constructed_class( 42, "some text" );
    BOOST_CHECK( MOCK_VERIFY( my_constructed_class::constructor ) );
    CHECK_CALLS( 1 );
}

namespace
{
    template< typename T >
    MOCK_CLASS( my_constructed_template_class )
    {
        MOCK_CONSTRUCTOR_TPL( my_constructed_template_class, 2, ( T, const std::string& ), constructor )
    };
}

BOOST_FIXTURE_TEST_CASE( mocking_a_template_class_constructor, mock_error_fixture )
{
    MOCK_EXPECT( my_constructed_template_class< int >::constructor ).with( 42, "some text" ).once();
    my_constructed_template_class< int >( 42, "some text" );
    BOOST_CHECK( MOCK_VERIFY( my_constructed_template_class< int >::constructor ) );
    CHECK_CALLS( 1 );
}

namespace
{
    MOCK_CLASS( my_destroyed_class )
    {
        MOCK_DESTRUCTOR( ~my_destroyed_class, destructor )
    };
}

BOOST_FIXTURE_TEST_CASE( mocking_a_destructor, mock_error_fixture )
{
    {
        my_destroyed_class c;
        MOCK_EXPECT( c.destructor ).once();
    }
    CHECK_CALLS( 1 );
}

BOOST_FIXTURE_TEST_CASE( failed_expectation_in_mocked_destructor_does_not_throw, mock_error_fixture )
{
    CHECK_ERROR(
        try
        {
            my_destroyed_class c;
            throw std::runtime_error( "should not crash" );
        }
        catch( std::runtime_error& )
        {
        },
        "unexpected call", 0, "?.my_destroyed_class::destructor()" );
}

BOOST_FIXTURE_TEST_CASE( failed_sequence_in_mocked_destructor_does_not_throw, mock_error_fixture )
{
    mock::sequence s;
    my_custom_mock m;
    CHECK_ERROR(
        {
            my_destroyed_class c;
            MOCK_EXPECT( c.destructor ).once().in( s );
            MOCK_EXPECT( m.my_tag ).once().in( s );
            m.my_method();
        },
        "sequence failed", 1, "c.my_destroyed_class::destructor()\n. once()" );
}

namespace
{
    MOCK_CLASS( boost_optional )
    {
        MOCK_METHOD_EXT( method, 0, boost::optional< my_observer& >(), tag )
    };
}

BOOST_FIXTURE_TEST_CASE( boost_optional_on_base_class_reference_as_return_type_is_supported, mock_error_fixture )
{
    boost_optional b;
    my_mock_observer o;
    MOCK_EXPECT( b.tag ).once().returns( boost::ref( o ) );
    b.method();
    CHECK_CALLS( 1 );
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

BOOST_FIXTURE_TEST_CASE( constraints_and_arguments_are_serialized_lazily, mock_error_fixture )
{
    MOCK_FUNCTOR( f, void( const custom_argument& ) );
    MOCK_EXPECT( f ).with( custom_constraint() );
    f( custom_argument() );
    BOOST_CHECK( ! serialized );
    CHECK_CALLS( 1 );
}

namespace
{
    struct custom_constraint_with_non_const_operator
    {
        template< typename Actual >
        bool operator()( Actual actual )
        {
            return actual == 42;
        }
    };
}

BOOST_FIXTURE_TEST_CASE( custom_constraint_function_operator_does_not_need_to_be_const, mock_error_fixture )
{
    MOCK_FUNCTOR( f, void( float ) );
    MOCK_EXPECT( f ).with( mock::constraint< custom_constraint_with_non_const_operator >( custom_constraint_with_non_const_operator() ) );
    f( 42 );
    CHECK_CALLS( 1 );
}

BOOST_FIXTURE_TEST_CASE( boost_reference_wrapper_is_supported_in_value_constraint, mock_error_fixture )
{
    MOCK_FUNCTOR( f, void( const std::string& ) );
    std::string s;
    MOCK_EXPECT( f ).once().with( boost::cref( s ) );
    s = "string";
    f( "string" );
    CHECK_CALLS( 1 );
}

namespace
{
    template< typename T >
    void nothing( T )
    {}

    struct member_pointer_mock_class
    {
        MOCK_CONST_METHOD_EXT( my_method, 0, void(), my_method )
    };
}

BOOST_FIXTURE_TEST_CASE( member_pointer_on_mock_method_is_valid, mock_error_fixture )
{
    nothing( &member_pointer_mock_class::my_method );
}

namespace
{
    MOCK_FUNCTION( free_function, 1, void( int ), free_function )
}

BOOST_FIXTURE_TEST_CASE( a_free_function_can_be_mocked, mock_error_fixture )
{
    MOCK_EXPECT( free_function ).once();
    CHECK_ERROR(
        BOOST_CHECK( ! MOCK_VERIFY( free_function ) ),
        "verification failed", 0, "free_function\n. once().with( any )" );
    free_function( 42 );
    CHECK_CALLS( 1 );
    BOOST_CHECK( MOCK_VERIFY( free_function ) );
    MOCK_RESET( free_function );
}

namespace
{
    struct some_class : mock::object
    {
        MOCK_STATIC_METHOD( some_static_method, 1, void( int ), some_static_method )
    };
}

BOOST_FIXTURE_TEST_CASE( a_static_method_can_be_mocked, mock_error_fixture )
{
    MOCK_EXPECT( some_class::some_static_method ).once();
    CHECK_ERROR(
        BOOST_CHECK( ! MOCK_VERIFY( some_class::some_static_method ) ),
        "verification failed", 0, "some_class::some_static_method\n. once().with( any )" );
    some_class::some_static_method( 42 );
    CHECK_CALLS( 1 );
    BOOST_CHECK( MOCK_VERIFY( some_class::some_static_method ) );
    MOCK_RESET( some_class::some_static_method );
}

BOOST_FIXTURE_TEST_CASE( a_static_method_is_not_reset_when_resetting_an_instance_of_the_class, mock_error_fixture )
{
    MOCK_EXPECT( some_class::some_static_method ).once();
    some_class c;
    mock::reset( c );
    CHECK_ERROR(
        BOOST_CHECK( ! MOCK_VERIFY( some_class::some_static_method ) ),
        "verification failed", 0, "some_class::some_static_method\n. once().with( any )" );
    MOCK_RESET( some_class::some_static_method );
}

namespace
{
    template< typename T >
    struct some_template_class
    {
        MOCK_STATIC_METHOD_TPL( some_static_method, 1, void( T ), some_static_method )
    };
}

BOOST_FIXTURE_TEST_CASE( a_static_method_in_a_template_class_can_be_mocked, mock_error_fixture )
{
    MOCK_EXPECT( some_template_class< int >::some_static_method ).once();
    CHECK_ERROR(
        BOOST_CHECK( ! MOCK_VERIFY( some_template_class< int >::some_static_method ) ),
        "verification failed", 0, "some_template_class< int >::some_static_method\n. once().with( any )" );
    some_template_class< int >::some_static_method( 42 );
    BOOST_CHECK( mock::verify() );
    BOOST_CHECK( MOCK_VERIFY( some_template_class< int >::some_static_method ) );
    MOCK_RESET( some_template_class< int >::some_static_method );
    CHECK_CALLS( 1 );
}

namespace
{
    MOCK_CLASS( mock_class )
    {
        MOCK_METHOD_EXT( m, 0, void(), t );
    };
}

BOOST_FIXTURE_TEST_CASE( resetting_referenced_mock_class_does_not_crash, mock_error_fixture )
{
    MOCK_FUNCTOR( f, mock_class() );
    {
        mock_class c;
        MOCK_EXPECT( f ).returns( c );
        MOCK_EXPECT( c.t );
    }
    mock::reset();
}

namespace
{
    MOCK_CLASS( mock_class2 )
    {
        MOCK_METHOD_EXT( m, 0, mock_class2(), t );
    };
}

BOOST_FIXTURE_TEST_CASE( resetting_self_referenced_mock_class_does_not_crash, mock_error_fixture )
{
    {
        mock_class2 c;
        MOCK_EXPECT( c.t ).returns( c );
    }
    mock::reset();
}

namespace
{
    template< typename T1, typename T2 >
    struct my_base
    {};
    MOCK_BASE_CLASS( my_comma_mock, my_base< int BOOST_PP_COMMA() int > )
    {};
}

#ifdef MOCK_THREAD_SAFE

#include <boost/thread.hpp>

namespace
{
    void create_class()
    {
        my_mock m;
        MOCK_EXPECT( m.my_tag ).once().with( 3 ).returns( 42 );
        try
        {
            m.my_method( 3 );
        }
        catch( ... )
        {}
    }
}

BOOST_FIXTURE_TEST_CASE( mock_class_creation_is_thread_safe, mock_error_fixture )
{
    boost::thread_group group;
    for( int i = 0; i < 100; ++i )
        group.create_thread( &create_class );
    group.join_all();
    CHECK_CALLS( 100 );
}

namespace
{
    void create_functor( int i )
    {
        mock::detail::functor< void( int ) > f;
        boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
        mock::detail::functor< void( int ) > f_mock;
        MOCK_EXPECT( f ).once().with( i );
        try
        {
            f( i );
        }
        catch( ... )
        {}
    }
}

BOOST_FIXTURE_TEST_CASE( mock_functor_creation_is_thread_safe, mock_error_fixture )
{
    boost::thread_group group;
    for( int i = 0; i < 100; ++i )
        group.create_thread( boost::bind( &create_functor, i ) );
    group.join_all();
    CHECK_CALLS( 100 );
}

namespace
{
    void iterate( my_mock& m )
    {
        MOCK_EXPECT( m.my_tag ).once().with( 3 ).returns( 42 );
        BOOST_CHECK_EQUAL( 42, m.my_method( 3 ) );
    }
}

BOOST_FIXTURE_TEST_CASE( mock_class_is_thread_safe, mock_error_fixture )
{
    my_mock m;
    boost::thread_group group;
    for( int i = 0; i < 100; ++i )
        group.create_thread( boost::bind( &iterate, boost::ref( m ) ) );
    group.join_all();
    CHECK_CALLS( 100 );
}

#endif // MOCK_THREAD_SAFE

namespace
{
    MOCK_CLASS( my_multi_mock )
    {
        MOCK_METHOD_EXT( m1, 1, void( int ), m1 );
        MOCK_METHOD_EXT( m2, 2, void( int, int ), m2 );
    };
}

BOOST_FIXTURE_TEST_CASE( mock_method_accepts_multi_constraint, mock_error_fixture )
{
    my_multi_mock m;
    MOCK_FUNCTOR( f, bool( int, int ) );
    MOCK_EXPECT( m.m2 ).once().with( f );
    MOCK_EXPECT( f ).once().with( 1, 2 ).returns( true );
    m.m2( 1, 2 );
    CHECK_CALLS( 2 );
}

namespace
{
    struct my_polymorphic_constraint
    {
        template< typename T1, typename T2 >
        bool operator()( T1, T2 ) const
        {
            return true;
        }
    };
}

BOOST_FIXTURE_TEST_CASE( mock_method_accepts_polymorphic_multi_constraint, mock_error_fixture )
{
    my_multi_mock m;
    MOCK_EXPECT( m.m2 ).once().with( my_polymorphic_constraint() );
    m.m2( 1, 2 );
    CHECK_CALLS( 1 );
}

#ifdef MOCK_SMART_PTR

BOOST_FIXTURE_TEST_CASE( std_unique_ptr_argument_is_supported_in_action, mock_error_fixture )
{
    MOCK_FUNCTOR( f, void( std::unique_ptr< int > ) );
    std::unique_ptr< int > p;
    MOCK_EXPECT( f ).once().calls(
        []( std::unique_ptr< int > )
        {
        } );
    f( std::unique_ptr< int >( new int( 7 ) ) );
    CHECK_CALLS( 1 );
}

BOOST_FIXTURE_TEST_CASE( std_unique_ptr_argument_is_supported_in_equal_constraint, mock_error_fixture )
{
    {
        MOCK_FUNCTOR( f, void( std::unique_ptr< int > ) );
        MOCK_EXPECT( f ).once().with( mock::equal( 7 ) );
        f( std::unique_ptr< int >( new int( 7 ) ) );
        CHECK_CALLS( 1 );
    }
    {
        MOCK_FUNCTOR( f, void( std::unique_ptr< int > ) );
        MOCK_EXPECT( f ).once().with( 7 );
        f( std::unique_ptr< int >( new int( 7 ) ) );
        CHECK_CALLS( 1 );
    }
}

BOOST_FIXTURE_TEST_CASE( std_unique_ptr_argument_is_supported_in_retrieve_constraint, mock_error_fixture )
{
    {
        MOCK_FUNCTOR( f, void( std::unique_ptr< int > ) );
        MOCK_EXPECT( f ).once().with( nullptr );
        f( 0 );
        CHECK_CALLS( 1 );
    }
    {
        std::unique_ptr< int > i;
        MOCK_FUNCTOR( f, void( std::unique_ptr< int > ) );
        MOCK_EXPECT( f ).once().with( mock::retrieve( i ) );
        std::unique_ptr< int > j( new int( 7 ) );
        f( std::move( j ) );
        BOOST_CHECK( !j );
        BOOST_REQUIRE( i );
        BOOST_CHECK_EQUAL( 7, *i );
        CHECK_CALLS( 1 );
    }
    {
        std::unique_ptr< int > i;
        MOCK_FUNCTOR( f, void( std::unique_ptr< int > ) );
        MOCK_EXPECT( f ).once().with( nullptr );
        MOCK_EXPECT( f ).once().with( mock::retrieve( i ) );
        f( 0 );
        std::unique_ptr< int > j( new int( 7 ) );
        f( std::move( j ) );
        BOOST_CHECK( !j );
        BOOST_REQUIRE( i );
        BOOST_CHECK_EQUAL( 7, *i );
        CHECK_CALLS( 2 );
    }
}

struct my_unique_ptr_class
{
    MOCK_CONSTRUCTOR( my_unique_ptr_class, 1, ( std::unique_ptr< int > ), constructor )
    MOCK_METHOD_EXT( m, 1, void( std::unique_ptr< int > ), m )
    MOCK_STATIC_METHOD( ms, 1, void( std::unique_ptr< int > ), ms )
};

#endif
