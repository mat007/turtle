// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "mock_error.hpp"
#include <turtle/reset.hpp>
#include <turtle/verify.hpp>
#include <turtle/detail/function.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/scoped_ptr.hpp>

namespace
{
    struct object : mock::object
    {};

    object static_o;
}

BOOST_FIXTURE_TEST_CASE( verifying_an_empty_object_succeeds, mock_error_fixture )
{
    object o;
    BOOST_CHECK( mock::verify( o ) );
}

namespace
{
    struct fixture : mock_error_fixture
    {
        fixture()
        {
            mock::detail::configure( o, e, "instance", MOCK_TYPE_NAME(o), "name" );
        }
        object o;
        mock::detail::function< void() > e;
    };
}

BOOST_FIXTURE_TEST_CASE( verifying_an_object_containing_a_failing_expectation_fails, fixture )
{
    e.expect().once();
    CHECK_ERROR(
        BOOST_CHECK( ! mock::verify( o ) ),
        "verification failed", 0, "instanceobject::name\n. once()" );
    mock::reset( o );
    BOOST_CHECK( mock::verify( o ) );
}

BOOST_FIXTURE_TEST_CASE( verifying_all_objects_with_one_of_them_containing_a_failing_expectation_fails, fixture )
{
    e.expect().once();
    CHECK_ERROR(
        BOOST_CHECK( ! mock::verify() ),
        "verification failed", 0, "instanceobject::name\n. once()" );
    mock::reset();
    BOOST_CHECK( mock::verify() );
}

BOOST_FIXTURE_TEST_CASE( resetting_an_object_containing_a_failing_expectation_and_verifying_it_succeeds, fixture )
{
    e.expect().once();
    mock::reset( o );
    BOOST_CHECK( mock::verify( o ) );
}

BOOST_FIXTURE_TEST_CASE( an_object_is_assignable_by_sharing_its_state, mock_error_fixture )
{
    object o1;
    mock::detail::function< void() > e;
    {
        object o2;
        mock::detail::configure( o2, e, "instance", MOCK_TYPE_NAME(o2), "name" );
        e.expect().once();
        o1 = o2;
        CHECK_ERROR(
            BOOST_CHECK( ! mock::verify( o1 ) ),
            "verification failed", 0, "instanceobject::name\n. once()" );
        CHECK_ERROR(
            BOOST_CHECK( ! mock::verify( o2 ) ),
            "verification failed", 0, "instanceobject::name\n. once()" );
    }
    CHECK_ERROR(
        BOOST_CHECK( ! mock::verify( o1 ) ),
        "verification failed", 0, "instanceobject::name\n. once()" );
}

BOOST_FIXTURE_TEST_CASE( an_object_is_copiable_by_sharing_its_state, mock_error_fixture )
{
    boost::scoped_ptr< object > o2( new object );
    const object o1( *o2 );
    mock::detail::function< void() > e;
    mock::detail::configure( *o2, e, "instance", MOCK_TYPE_NAME(*o2), "name" );
    e.expect().once();
    CHECK_ERROR(
        BOOST_CHECK( ! mock::verify( *o2 ) ),
        "verification failed", 0, "instanceobject::name\n. once()" );
    CHECK_ERROR(
        BOOST_CHECK( ! mock::verify( o1 ) ),
        "verification failed", 0, "instanceobject::name\n. once()" );
    o2.reset();
    CHECK_ERROR(
        BOOST_CHECK( ! mock::verify( o1 ) ),
        "verification failed", 0, "instanceobject::name\n. once()" );
}
