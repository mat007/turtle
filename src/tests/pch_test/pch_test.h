//
// Copyright Mathieu Champlon 2011
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

//#include <boost/spirit/home/phoenix/statement/throw.hpp>
//#include <boost/spirit/home/phoenix/operator/self.hpp>
//#include <boost/spirit/home/phoenix/core/nothing.hpp>
//#include <boost/spirit/home/phoenix/bind.hpp>
//#include <boost/function.hpp>
//#include <boost/bind.hpp>
//#include <boost/test/test_tools.hpp>
//#include <boost/test/unit_test_suite.hpp>
//#include <boost/test/execution_monitor.hpp>
//#include <boost/type_traits/remove_reference.hpp>
//#include <boost/type_traits/remove_const.hpp>
//#include <boost/type_traits/integral_constant.hpp>
//#include <boost/type_traits/is_convertible.hpp>
//#include <boost/type_traits/detail/yes_no_type.hpp>
//#include <boost/type_traits/is_base_of.hpp>
//#include <boost/function_types/function_type.hpp>
//#include <boost/lambda/lambda.hpp>
//#include <boost/lambda/bind.hpp>

#include <turtle/mock.hpp>
//#include <boost/test/auto_unit_test.hpp>

// release vc80
//
//   xeumeuleu/xml.h 24

//   boost/shared_ptr.hpp                                            5
//   boost/bind.hpp                                                  6
//   boost/type_traits.hpp                                          19
//   boost/function_types/function_type.hpp                         19
//   boost/function.hpp                                             23
//   boost/function.hpp + boost/bind.hpp                            25
//   boost/lambda/lambda.hpp                                        25
//   boost/lambda/bind.hpp                                          22
//   boost/function_types/function_type.hpp + boost/type_traits.hpp 36
//   boost/spirit/home/phoenix/core.hpp                             46
//
//   boost/test/auto_unit_test.hpp                   27
//   turtle/mock.hpp                                 89
//   boost/test/auto_unit_test.hpp + turtle/mock.hpp 93
//
//   all boost includes from turtle  89

// getting rid of :
//   boost/bind                            3
//   function_type                        13
//   boost/phoenix                        31
