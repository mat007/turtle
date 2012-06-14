//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

#include "mock_error.hpp"
#include <turtle/object.hpp>
#include <turtle/function.hpp>
#include <boost/test/auto_unit_test.hpp>

namespace
{
    struct object : mock::object
    {};

    object static_o;
}

BOOST_AUTO_TEST_CASE( verifying_an_empty_object_succeeds )
{
    object o;
    BOOST_CHECK( mock::verify( o ) );
}

namespace
{
    struct fixture
    {
        fixture()
        {
            mock::detail::configure( o, e, "instance", mock::detail::type_name( typeid( "type" ) ), "name" );
        }
        object o;
        mock::function< void() > e;
    };
}

BOOST_FIXTURE_TEST_CASE( verifying_an_object_containing_a_failing_expectation_fails, fixture )
{
    e.expect().once();
    BOOST_CHECK( ! mock::verify( o ) );
    mock::reset( o );
    BOOST_CHECK( mock::verify( o ) );
}

BOOST_FIXTURE_TEST_CASE( verifying_all_objects_with_one_of_them_containing_a_failing_expectation_fails, fixture )
{
    e.expect().once();
    BOOST_CHECK( ! mock::verify() );
    mock::reset();
    BOOST_CHECK( mock::verify() );
}

BOOST_FIXTURE_TEST_CASE( resetting_an_object_containing_a_failing_expectation_and_verifying_it_succeeds, fixture )
{
    e.expect().once();
    mock::reset( o );
    BOOST_CHECK( mock::verify( o ) );
}

BOOST_AUTO_TEST_CASE( an_object_is_assignable_by_sharing_its_state )
{
    object o1;
    mock::function< void() > e;
    {
        object o2;
        mock::detail::configure( o2, e, "instance", mock::detail::type_name( typeid( "type" ) ), "name" );
        e.expect().once();
        o1 = o2;
        BOOST_CHECK( ! mock::verify( o2 ) );
        BOOST_CHECK( ! mock::verify( o1 ) );
    }
    BOOST_CHECK( ! mock::verify( o1 ) );
}

BOOST_AUTO_TEST_CASE( an_object_is_copiable_by_sharing_its_state )
{
    std::auto_ptr< object > o2( new object );
    const object o1( *o2 );
    mock::function< void() > e;
    mock::detail::configure( *o2, e, "instance", mock::detail::type_name( typeid( "type" ) ), "name" );
    e.expect().once();
    BOOST_CHECK( ! mock::verify( *o2 ) );
    BOOST_CHECK( ! mock::verify( o1 ) );
    o2.reset();
    BOOST_CHECK( ! mock::verify( o1 ) );
}
