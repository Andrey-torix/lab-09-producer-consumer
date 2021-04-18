cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_COVERAGE "Build code coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
  SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
)

# TODO: rename project and delete this comment
project(lab9 VERSION 0.1.0)

string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
hunter_add_package(Boost COMPONENTS  system filesystem log program_options )
find_package(Boost 1.72.0 CONFIG REQUIRED thread log log_setup program_options date_time filesystem system regex)

#SET(Boost_USE_STATIC_LIBS ON)  
#SET(Boost_USE_MULTITHREADED ON)  
#FIND_PACKAGE(Boost 1.72.0 COMPONENTS **system thread filesystem  date_time regex log log_setup** REQUIRED)  


add_library(menu STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/menu.cpp
)
add_executable(lab9
 ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
)

target_include_directories(lab9
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(menu
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_link_libraries(lab9 Boost::thread)

if(BUILD_COVERAGE)
  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/coverage/cmake")
  find_package(codecov)
  string(APPEND CMAKE_CXX_FLAGS " --coverage")
  add_coverage(${PROJECT_NAME})
  #add_coverage(tests)
  #coverage_evaluate()
endif()

if(BUILD_DOCS)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/_docs)
  set(DOXYGEN_EXAMPLE_PATH examples)
  set(DOXYGEN_GENERATE_HTML NO)
  set(DOXYGEN_GENERATE_MAN YES)
  find_package(Doxygen)
  doxygen_add_docs(docs include)
  install(DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/man DESTINATION share)
  # add_dependencies(install docs) need to call target docs before install or package
endif()

include(CPackConfig.cmake)

#install(TARGETS ${PROJECT_NAME} demo
 # EXPORT "${PROJECT_NAME}-targets"
  #RUNTIME DESTINATION bin
  #ARCHIVE DESTINATION lib
  #LIBRARY DESTINATION lib
  #INCLUDES DESTINATION include
#)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

#install(EXPORT "${PROJECT_NAME}-targets"
  #NAMESPACE "${PROJECT_NAME}::"
  #DESTINATION "lib/cmake"
#)
