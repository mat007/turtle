// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_TYPE_NAME_HPP_INCLUDED
#define MOCK_TYPE_NAME_HPP_INCLUDED

#include <boost/test/utils/basic_cstring/io.hpp>
#include <boost/detail/sp_typeinfo.hpp>
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
        explicit type_name( const boost::detail::sp_typeinfo& info )
            : info_( &info )
        {}
        friend std::ostream& operator<<( std::ostream& s, const type_name& t )
        {
            t.serialize( s, *t.info_ );
            return s;
        }
    private:
        typedef boost::unit_test::const_string const_string;
        typedef const_string::size_type size_type;

        void serialize( std::ostream& s,
            const boost::detail::sp_typeinfo& info ) const
        {
            const char* name = info.name();
#ifdef __GNUC__
            int status = 0;
            char* result = abi::__cxa_demangle( name, 0, 0, &status );
            struct guard
            {
                explicit guard( char* p )
                    : p_( p )
                {}
                ~guard()
                {
                    std::free( p_ );
                }
            private:
                char* p_;
            } g( result );
            if( ! status && result )
                serialize( s, result );
            else
#endif
                serialize( s, name );
        }
        void serialize( std::ostream& s, const_string name ) const
        {
            const size_type tpl_end = name.rfind( ">" );
            const size_type nm = name.rfind( "::" );
            if( tpl_end == const_string::npos || tpl_end <= nm )
            {
                s << remove_prefix( remove_namespace( name ) );
                return;
            }
            const size_type tpl_start = find_template_start( name );
            s << remove_namespace( name.substr( 0, tpl_start ) ) << '<';
            serialize( s, name.substr( tpl_start + 1, tpl_end ) );
            s << '>';
        }
        const_string remove_prefix( const_string name ) const
        {
            name = remove_prefix( name, "class " );
            name = remove_prefix( name, "struct " );
            return name;
        }
        const_string remove_prefix( const_string name,
            const_string prefix ) const
        {
            if( name.substr( 0, prefix.size() ) == prefix )
                return name.substr( prefix.size() );
            return name;
        }
        size_type find_template_start( const_string name ) const
        {
            size_type count = 0;
            for( size_type i = name.size(); i > 0; --i )
            {
                if( name[ i ] == '>' )
                    ++count;
                if( name[ i ] == '<' && --count == 0 )
                    return i;
            }
            return static_cast< size_type >( const_string::npos );
        }
        const_string remove_namespace( const_string name ) const
        {
            size_type p = name.rfind( "::" );
            if( p == const_string::npos )
                return name;
            return name.substr( p + 2 );
        }

        const boost::detail::sp_typeinfo* info_;
    };
}
} // mock

#endif // MOCK_TYPE_NAME_HPP_INCLUDED
