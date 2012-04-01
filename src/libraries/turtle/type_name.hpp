//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

#ifndef MOCK_TYPE_NAME_HPP_INCLUDED
#define MOCK_TYPE_NAME_HPP_INCLUDED

#include <boost/test/utils/basic_cstring/io.hpp>
#include <stdexcept>
#include <typeinfo>
#include <ostream>
#ifdef __GNUC__
#include <cxxabi.h>
#include <cstdlib>
#endif

namespace mock
{
namespace detail
{
    class type_name
    {
    public:
        explicit type_name( const std::type_info& info )
            : info_( &info )
        {}
        friend std::ostream& operator<<( std::ostream& s, const type_name& t )
        {
            t.serialize( s, *t.info_ );
            return s;
        }
    private:
        void serialize( std::ostream& s, const std::type_info& info ) const
        {
            const char* name = info.name();
#ifdef __GNUC__
            size_t size = 0;
            int status = 0;
            char* result = abi::__cxa_demangle( name, NULL, &size, &status );
            struct guard
            {
                explicit guard( char* p )
                    : p_( p )
                {}
                ~guard()
                {
                    free( p_ );
                }
            private:
                char* p_;
            } g( result );
            if( result )
                serialize( s, result );
            else
#endif
                serialize( s, name );
        }
        void serialize( std::ostream& s,
            boost::unit_test::const_string name ) const
        {
            boost::unit_test::const_string::size_type p = name.rfind( "::" );
            if( p != boost::unit_test::const_string::npos )
            {
                s << name.substr( p + 2 );
                return;
            }
            p = name.rfind( " " );
            if( p != boost::unit_test::const_string::npos )
            {
                s << name.substr( p + 1 );
                return;
            }
            s << name;
        }

        const std::type_info* info_;
    };
}
}

#endif // MOCK_TYPE_NAME_HPP_INCLUDED
