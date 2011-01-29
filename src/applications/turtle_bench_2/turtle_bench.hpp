//
//  Copyright Mathieu Champlon 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#pragma message( BOOST_PP_STRINGIZE( BOOST_PP_CAT( mock_class, BOOST_PP_ITERATION() ) ) )

#define MOCK_F(z, n, d) MOCK_METHOD( f##n, 0 )
//#define MOCK_F(z, n, d) MOCK_METHOD_ALT( f##n )

MOCK_BASE_CLASS( BOOST_PP_CAT( mock_class, BOOST_PP_ITERATION() ), base_class )
{
    BOOST_PP_REPEAT_FROM_TO(0, NUMBER_OF_METHODS, MOCK_F, )
};
