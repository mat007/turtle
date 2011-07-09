//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_GROUP_HPP_INCLUDED
#define MOCK_GROUP_HPP_INCLUDED

#include "verifiable.hpp"
#include <functional>
#include <algorithm>
#include <vector>

namespace mock
{
namespace detail
{
    class group
    {
    public:
        void add( verifiable& v )
        {
            verifiables_.push_back( &v );
        }
        void remove( verifiable& v )
        {
            verifiables_.erase(
                std::remove( verifiables_.begin(), verifiables_.end(), &v ),
                verifiables_.end() );
        }

        bool verify() const
        {
            bool valid = true;
            for( verifiables_cit it = verifiables_.begin();
                    it != verifiables_.end(); ++it )
                if( ! (*it)->verify() )
                    valid = false;
            return valid;
        }
        void reset() const
        {
            std::for_each( verifiables_.begin(), verifiables_.end(),
                std::mem_fun( &verifiable::reset ) );
        }

    private:
        typedef std::vector< verifiable* > verifiables_t;
        typedef verifiables_t::iterator verifiables_it;
        typedef verifiables_t::const_iterator verifiables_cit;

        verifiables_t verifiables_;
    };
}
}

#endif // MOCK_GROUP_HPP_INCLUDED
