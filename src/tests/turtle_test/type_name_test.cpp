//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/type_name.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

struct my_type_from_default_namespace {};

BOOST_AUTO_TEST_CASE( name_of_type_from_default_namespace_is_extracted )
{
    BOOST_CHECK_EQUAL( "my_type_from_default_namespace", mock::detail::type_name< my_type_from_default_namespace >() );
}

namespace
{
    struct my_type_from_anonymous_namespace {};
}

BOOST_AUTO_TEST_CASE( name_of_type_from_anonymous_namespace_is_extracted )
{
    BOOST_CHECK_EQUAL( "my_type_from_anonymous_namespace", mock::detail::type_name< my_type_from_anonymous_namespace >() );
}

namespace nm
{
    struct my_type_from_named_namespace {};
}

BOOST_AUTO_TEST_CASE( name_of_type_from_named_namespace_is_extracted )
{
    BOOST_CHECK_EQUAL( "my_type_from_named_namespace", mock::detail::type_name< nm::my_type_from_named_namespace >() );
}

namespace nm
{
namespace inner
{
    struct my_type_from_named_inner_namespace {};
}
}

BOOST_AUTO_TEST_CASE( name_of_type_from_named_inner_namespace_is_extracted )
{
    BOOST_CHECK_EQUAL( "my_type_from_named_inner_namespace", mock::detail::type_name< nm::inner::my_type_from_named_inner_namespace >() );
}

namespace
{
namespace inner
{
    struct my_type_from_unnamed_inner_namespace {};
}
}

BOOST_AUTO_TEST_CASE( name_of_type_from_unnamed_inner_namespace_is_extracted )
{
    BOOST_CHECK_EQUAL( "my_type_from_unnamed_inner_namespace", mock::detail::type_name< inner::my_type_from_unnamed_inner_namespace >() );
}
