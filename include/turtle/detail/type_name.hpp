// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_TYPE_NAME_HPP_INCLUDED
#define MOCK_TYPE_NAME_HPP_INCLUDED

#include "../config.hpp"
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/test/utils/basic_cstring/io.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION >= 107000
#    include <boost/core/typeinfo.hpp>
#    define MOCK_TYPEID(t) BOOST_CORE_TYPEID(t)
#    define MOCK_TYPEINFO boost::core::typeinfo
#else
#    include <boost/detail/sp_typeinfo.hpp>
#    define MOCK_TYPEID(t) BOOST_SP_TYPEID(t)
#    define MOCK_TYPEINFO boost::detail::sp_typeinfo
#endif
#include <boost/shared_ptr.hpp>
#include <ostream>
#include <stdexcept>
#include <typeinfo>
#ifdef __GNUC__
#    include <cstdlib>
#    include <cxxabi.h>
#endif

#define MOCK_TYPE_NAME(t) mock::detail::type_name(MOCK_TYPEID(t))

namespace mock { namespace detail {
    class type_name
    {
    public:
        explicit type_name(const MOCK_TYPEINFO& info) : info_(&info) {}
        friend std::ostream& operator<<(std::ostream& s, const type_name& t)
        {
            t.serialize(s, *t.info_);
            return s;
        }

    private:
        void serialize(std::ostream& s, const MOCK_TYPEINFO& info) const
        {
            const char* name = info.name();
#ifdef __GNUC__
            int status = 0;
            boost::shared_ptr<char> demangled(abi::__cxa_demangle(name, 0, 0, &status), &std::free);
            if(!status && demangled)
                serialize(s, demangled.get());
            else
#endif
                serialize(s, name);
        }

        typedef std::string::size_type size_type;

        void serialize(std::ostream& s, std::string name) const
        {
            const size_type nm = rfind(name, ':') + 1;
            const size_type tpl = name.find('<', nm);
            s << clean(name.substr(nm, tpl - nm));
            if(tpl == std::string::npos)
                return;
            s << '<';
            list(s, name.substr(tpl + 1, name.rfind('>') - tpl - 1));
            s << '>';
        }
        void list(std::ostream& s, const std::string& name) const
        {
            const size_type comma = rfind(name, ',');
            if(comma != std::string::npos)
            {
                list(s, name.substr(0, comma));
                s << ", ";
            }
            serialize(s, name.substr(comma + 1));
        }
        std::string clean(std::string name) const
        {
            boost::algorithm::trim(name);
            boost::algorithm::erase_all(name, "class ");
            boost::algorithm::erase_all(name, "struct ");
            boost::algorithm::erase_all(name, "__ptr64");
            boost::algorithm::replace_all(name, " &", "&");
            boost::algorithm::replace_all(name, "& ", "&");
            boost::algorithm::replace_all(name, " *", "*");
            boost::algorithm::replace_all(name, "* ", "*");
            return name;
        }
        size_type rfind(const std::string& name, char c) const
        {
            size_type count = 0;
            for(size_type i = name.size() - 1; i > 0; --i)
            {
                if(name[i] == '>')
                    ++count;
                else if(name[i] == '<')
                    --count;
                if(name[i] == c && count == 0)
                    return i;
            }
            return std::string::npos;
        }

        const MOCK_TYPEINFO* info_;
    };
}} // namespace mock::detail

#endif // MOCK_TYPE_NAME_HPP_INCLUDED
