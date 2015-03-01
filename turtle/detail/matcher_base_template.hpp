// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace mock
{
namespace detail
{
    template< typename Signature > class matcher_base;

    template<
        BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, typename Actual_) >
    class matcher_base< void( BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, Actual_) ) >
        : boost::noncopyable
    {
    public:
        virtual ~matcher_base() {}

        virtual bool operator()(
            BOOST_PP_ENUM_BINARY_PARAMS(MOCK_NUM_ARGS, Actual_, actual_) ) = 0;

        friend std::ostream& operator<<(
            std::ostream& s, const matcher_base& m )
        {
            m.serialize( s );
            return s;
        }

    private:
        virtual void serialize( std::ostream& ) const = 0;
    };
}
} // mock
