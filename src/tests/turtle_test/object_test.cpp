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
#include <turtle/object.hpp>
#include <turtle/expectation.hpp>

BOOST_AUTO_TEST_CASE( verifying_an_empty_object_succeeds )
{
    mock::object o;
    BOOST_CHECK( o.verify() );
}

BOOST_AUTO_TEST_CASE( verifying_an_object_containing_a_failing_expectation_fails )
{
    mock::object o;
    mock::expectation< void() > e;
    o.set_parent( e );
    e.expect().once();
    BOOST_CHECK( ! o.verify() );
    o.reset();
    BOOST_CHECK( o.verify() );
}

BOOST_AUTO_TEST_CASE( verifying_all_objects_with_one_of_them_containing_a_failing_expectation_fails )
{
    mock::object o;
    mock::expectation< void() > e;
    o.set_parent( e );
    e.expect().once();
    BOOST_CHECK( ! mock::verify() );
    mock::reset();
    BOOST_CHECK( mock::verify() );
}

BOOST_AUTO_TEST_CASE( resetting_an_object_containing_a_failing_expectation_and_verifying_it_succeeds )
{
    mock::object o;
    mock::expectation< void() > e;
    o.set_parent( e );
    e.expect().once();
    o.reset();
    BOOST_CHECK( o.verify() );
}

BOOST_AUTO_TEST_CASE( an_object_is_assignable_by_sharing_its_state )
{
    mock::object o1;
    mock::expectation< void() > e;
    {
        mock::object o2;
        o2.set_parent( e );
        e.expect().once();
        o1 = o2;
        BOOST_CHECK( ! o2.verify() );
        BOOST_CHECK( ! o1.verify() );
    }
    BOOST_CHECK( ! o1.verify() );
}

BOOST_AUTO_TEST_CASE( an_object_is_copiable_by_sharing_its_state )
{
    std::auto_ptr< mock::object > o2( new mock::object );
    const mock::object o1( *o2 );
    mock::expectation< void() > e;
    o2->set_parent( e );
    e.expect().once();
    BOOST_CHECK( ! o2->verify() );
    BOOST_CHECK( ! o1.verify() );
    o2.reset();
    BOOST_CHECK( ! o1.verify() );
}
