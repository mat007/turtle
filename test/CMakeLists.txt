# Copyright 2019 Alexander Grund
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

find_package(Boost 1.58 REQUIRED COMPONENTS unit_test_framework thread)

add_library(TurtleTestMain INTERFACE)
target_link_libraries(TurtleTestMain INTERFACE Boost::unit_test_framework Boost::disable_autolinking)
target_compile_definitions(TurtleTestMain INTERFACE BOOST_AUTO_TEST_MAIN)
# Heuristically guess if we are compiling against dynamic boost
if(NOT Boost_USE_STATIC_LIBS AND Boost_UNIT_TEST_FRAMEWORK_LIBRARY AND NOT Boost_UNIT_TEST_FRAMEWORK_LIBRARY MATCHES "\\${CMAKE_STATIC_LIBRARY_SUFFIX}\$")
    target_compile_definitions(TurtleTestMain INTERFACE BOOST_TEST_DYN_LINK)
endif()
# Enable warnings
option(TURTLE_WERROR "Treat warnings as errors" ON)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(TurtleTestMain INTERFACE -Wall -Wextra -pedantic -Wno-long-long)
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-Wunused-function TURTLE_CXX_UNUSED_FUNCTION)
  if(TURTLE_CXX_UNUSED_FUNCTION)
    target_compile_options(TurtleTestMain INTERFACE -Wno-unused-function)
  endif()
  if(TURTLE_WERROR)
    target_compile_options(TurtleTestMain INTERFACE -Werror)
  endif()
elseif(MSVC)
  target_compile_options(TurtleTestMain INTERFACE /W4)
  if(TURTLE_WERROR)
    target_compile_options(TurtleTestMain INTERFACE /WX)
  endif()
endif()

file(GLOB_RECURSE testFiles test_*.cpp)
set(testsUsingUndefinedCPP test_function test_integration)
foreach(testFile IN LISTS testFiles)
  get_filename_component(name ${testFile} NAME_WE)

  foreach(testVariant IN ITEMS "" _max_args _use_conversions _no_decltype _no_variadic_macros _thread_safe)
    set(curName ${name}${testVariant})
    add_executable(${curName} ${testFile})
    if(name IN_LIST testsUsingUndefinedCPP)
      target_sources(${curName} PRIVATE undefined.cpp)
    endif()
    target_link_libraries(${curName} PRIVATE turtle::turtle TurtleTestMain)
    add_test(NAME ${curName} COMMAND ${curName})
  endforeach()

  target_compile_definitions(${name}_max_args PRIVATE MOCK_MAX_ARGS=21)
  target_compile_definitions(${name}_use_conversions PRIVATE MOCK_USE_CONVERSIONS)
  target_compile_definitions(${name}_no_decltype PRIVATE MOCK_NO_DECLTYPE)
  target_compile_definitions(${name}_no_variadic_macros PRIVATE MOCK_NO_VARIADIC_MACROS)

  target_link_libraries(${name}_thread_safe PRIVATE Boost::thread)
  target_compile_definitions(${name}_thread_safe PRIVATE MOCK_THREAD_SAFE BOOST_THREAD_USES_MOVE)
endforeach()

add_executable(link-test_defined EXCLUDE_FROM_ALL test_exception.cpp defined_1.cpp defined_2.cpp)
target_link_libraries(link-test_defined PRIVATE turtle::turtle TurtleTestMain)
add_test(NAME link-test_defined COMMAND "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target link-test_defined --config $<CONFIG>)

file(GLOB_RECURSE compileFailureTestFiles fail_*.cpp)
foreach(testFile IN LISTS compileFailureTestFiles)
  get_filename_component(name ${testFile} NAME_WE)

  add_library(${name} STATIC EXCLUDE_FROM_ALL ${testFile})
  target_link_libraries(${name} turtle::turtle)
  add_test(NAME compile-${name} COMMAND "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${name} --config $<CONFIG>)
  set_tests_properties(compile-${name} PROPERTIES WILL_FAIL TRUE)
endforeach()
