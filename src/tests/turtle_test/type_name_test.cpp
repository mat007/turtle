//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

#include <turtle/type_name.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/lexical_cast.hpp>

struct my_type_from_default_namespace {};

namespace
{
    template< typename T >
    std::string to_string( const T& )
    {
        return boost::lexical_cast< std::string >( mock::detail::type_name( typeid( T ) ) );
    }
}

BOOST_AUTO_TEST_CASE( name_of_type_from_default_namespace_is_extracted )
{
    BOOST_CHECK_EQUAL( "my_type_from_default_namespace", to_string( my_type_from_default_namespace() ) );
}

namespace
{
    struct my_type_from_anonymous_namespace {};
}

BOOST_AUTO_TEST_CASE( name_of_type_from_anonymous_namespace_is_extracted )
{
    BOOST_CHECK_EQUAL( "my_type_from_anonymous_namespace", to_string( my_type_from_anonymous_namespace() ) );
}

namespace nm
{
    struct my_type_from_named_namespace {};
}

BOOST_AUTO_TEST_CASE( name_of_type_from_named_namespace_is_extracted )
{
    BOOST_CHECK_EQUAL( "my_type_from_named_namespace", to_string( nm::my_type_from_named_namespace() ) );
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
    BOOST_CHECK_EQUAL( "my_type_from_named_inner_namespace", to_string( nm::inner::my_type_from_named_inner_namespace() ) );
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
    BOOST_CHECK_EQUAL( "my_type_from_unnamed_inner_namespace", to_string( inner::my_type_from_unnamed_inner_namespace() ) );
}

BOOST_AUTO_TEST_CASE( name_of_local_type_is_extracted )
{
    struct my_local_type {};
    BOOST_CHECK_EQUAL( "my_local_type", boost::lexical_cast< std::string >( mock::detail::type_name( typeid( my_local_type ) ) ) );
}
