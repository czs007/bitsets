# Licensed to the LF AI & Data foundation under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required( VERSION 3.18 )

if ( APPLE )
    set( CMAKE_CROSSCOMPILING TRUE )
    set( RUN_HAVE_GNU_POSIX_REGEX 0 )
    set( CMAKE_C_COMPILER "/usr/local/opt/llvm/bin/clang" )
    set( CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++" )
endif ()

add_definitions(-DELPP_THREAD_SAFE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
message( STATUS "Building using CMake version: ${CMAKE_VERSION}" )

project(core)
include(CheckCXXCompilerFlag)

if ( APPLE )
    message(STATUS "==============Darwin Environment==============")
    check_cxx_compiler_flag(-std=c++11 HAS_STD_CPP11_FLAG)
    if(HAS_STD_CPP11_FLAG)
        add_compile_options(-std=c++11)
    endif()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message(STATUS "==============Linux Environment===============")
    set(LINUX TRUE)
elseif ( MSYS )
    message( STATUS "==============MSYS Environment===============" )
else ()
    message(FATAL_ERROR "Unsupported platform!" )
endif ()

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
include( Utils )

# **************************** Build time, type and code version ****************************
get_current_time( BUILD_TIME )
message( STATUS "Build time = ${BUILD_TIME}" )

get_build_type( TARGET BUILD_TYPE DEFAULT "Release" )
message( STATUS "Build type = ${BUILD_TYPE}" )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# **************************** Project ****************************
project( bitsets VERSION "1.0" )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED on )

#set( MILVUS_SOURCE_DIR      ${PROJECT_SOURCE_DIR} )
#set( MILVUS_BINARY_DIR      ${PROJECT_BINARY_DIR} )
#set( MILVUS_ENGINE_SRC      ${PROJECT_SOURCE_DIR}/src )
#set( MILVUS_THIRDPARTY_SRC  ${PROJECT_SOURCE_DIR}/thirdparty )

# This will set RPATH to all excutable TARGET
# self-installed dynamic libraries will be correctly linked by excutable
set( CMAKE_INSTALL_RPATH "/usr/lib" "${CMAKE_INSTALL_PREFIX}/lib" "${CMAKE_INSTALL_PREFIX}/lib64" )
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )


# **************************** Dependencies ****************************

include( ExternalProject )
include( GNUInstallDirs )
include_directories(${PROJECT_SOURCE_DIR})

# **************************** Compiler arguments ****************************
message( STATUS "Building bitsets" )

if (LINUX OR MSYS)
append_flags( CMAKE_CXX_FLAGS
              FLAGS
              "-fPIC"
              "-DELPP_THREAD_SAFE"
              "-fopenmp"
              "-Werror"
              )
endif ()

if ( APPLE )
    append_flags( CMAKE_CXX_FLAGS
            FLAGS
            "-fPIC"
            "-DELPP_THREAD_SAFE"
            "-fopenmp"
            "-Wno-error"
            "-Wsign-compare"
            "-Wall"
            "-pedantic"
            "-Wno-unused-command-line-argument"
            "-Wextra"
            "-Wno-unused-parameter"
            "-Wno-deprecated"
            "-DBOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED=1"
            )
endif ()

# Warning: add_subdirectory(src) must be after append_flags("-ftest-coverage"),
# otherwise cpp code coverage tool will miss src folder

add_subdirectory( bitset )
add_subdirectory( boost_ext )

add_custom_target( Clean-All COMMAND ${CMAKE_BUILD_TOOL} clean )

add_executable(bench_bitset bench_bitset.cpp)
target_link_libraries(bench_bitset
	bitset
        boost_bitset_ext
        )
install(TARGETS bench_bitset 
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin/
)

add_executable(check check.cpp)
target_link_libraries(check
	bitset
        boost_bitset_ext
        )
install(TARGETS check 
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin/
)
