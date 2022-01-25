# Copyright 2019 Alexander Grund
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.8)
project(turtle VERSION 1.3.2 LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(IS_ROOT_PROJECT ON)
  include(CTest)
else()
  set(IS_ROOT_PROJECT OFF)
endif()

option(TURTLE_INSTALL "Enable to add install target" ${IS_ROOT_PROJECT})

# Default boost libs are static on windows and dynamic on linux
if(WIN32 AND NOT DEFINED Boost_USE_STATIC_LIBS)
  set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost 1.58 REQUIRED)

set(MOCK_VERSION "\"${PROJECT_VERSION}\"")
set(_turtleVersionFile ${CMAKE_CURRENT_BINARY_DIR}/include/turtle/version.hpp)
configure_file(version.hpp.cmake ${_turtleVersionFile} @ONLY)

add_library(turtle INTERFACE)
add_library(turtle::turtle ALIAS turtle)
target_include_directories(turtle INTERFACE $<BUILD_INTERFACE:include;${CMAKE_CURRENT_BINARY_DIR}/include>)
target_compile_features(turtle INTERFACE cxx_std_14)

target_link_libraries(turtle INTERFACE Boost::boost Boost::disable_autolinking)

if(NOT CMAKE_VERSION VERSION_LESS 3.19)
  file(GLOB _turtleHeaders include/turtle/*.hpp)
  file(GLOB _turtleHeadersDetail include/turtle/detail/*.hpp)
  source_group(turtle FILES ${_turtleHeaders} ${_turtleVersionFile})
  source_group(turtle/detail FILES ${_turtleHeadersDetail})
  target_sources(turtle PRIVATE ${_turtleHeaders} ${_turtleVersionFile} ${_turtleHeadersDetail})
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

if(TURTLE_INSTALL)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  target_include_directories(turtle INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  install(TARGETS turtle
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
  install(DIRECTORY include/ ${CMAKE_CURRENT_BINARY_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  set(configFile ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake)
  set(versionFile ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake)
  set(configInstallDestination lib/cmake/${PROJECT_NAME})

  configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/Config.cmake.in
    ${configFile} 
    INSTALL_DESTINATION ${configInstallDestination}
  )

  if(NOT CMAKE_VERSION VERSION_LESS 3.14)
    write_basic_package_version_file(${versionFile} COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)
  else()
    set(OLD_CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
    set(CMAKE_SIZEOF_VOID_P "")
    write_basic_package_version_file(${versionFile} COMPATIBILITY SameMajorVersion)
    set(CMAKE_SIZEOF_VOID_P ${OLD_CMAKE_SIZEOF_VOID_P})
  endif()

  install(FILES ${configFile} ${versionFile} DESTINATION ${configInstallDestination})

  install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE "turtle::"
    DESTINATION ${configInstallDestination}
  )
endif()
