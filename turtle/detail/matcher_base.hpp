// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_MATCHER_BASE_HPP_INCLUDED
#define MOCK_MATCHER_BASE_HPP_INCLUDED

#include <boost/noncopyable.hpp>
#include <iosfwd>

namespace mock
{
namespace detail
{
    template< typename Actual >
    class matcher_base : boost::noncopyable
    {
    public:
        virtual ~matcher_base() {}

        virtual bool operator()( Actual ) = 0;

        friend std::ostream& operator<<(
            std::ostream& s, const matcher_base& c )
        {
            c.serialize( s );
            return s;
        }
    private:
        virtual void serialize( std::ostream& ) const = 0;
    };
}
} // mock

#endif // MOCK_MATCHER_BASE_HPP_INCLUDED
