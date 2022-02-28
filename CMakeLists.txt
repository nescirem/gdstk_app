cmake_minimum_required(VERSION 3.13.0)

project(gdstk_app)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ZLIB 1.2.9)
if(NOT ZLIB_FOUND)
    # Check first for header files
    if(DEFINED ENV{ZLIB_INCLUDE_DIRS})
        set(ZLIB_INCLUDE_DIRS ENV{ZLIB_INCLUDE_DIRS})
    else()
        message(FATAL_ERROR "ZLIB headers were not found, either via find_package or env variable")
    endif()

    # Use normal library-finding methods first
    find_library(ZLIB_LIBRARIES zlib)
    # But give precedence to user-set library path
    if(DEFINED ENV{ZLIB_LIBRARIES})
        set(ZLIB_LIBRARIES ENV{ZLIB_LIBRARIES})
    # This would be set if find_library succedes, e.g. the library is on PATH, but no env-var was set.
    elseif(NOT ZLIB_LIBRARIES)
        message(FATAL_ERROR "ZLIB could not be found, either via find_package, find_library, or env variable.")
    endif()
endif(NOT ZLIB_FOUND)
foreach(IT ${ZLIB_LIBRARIES})
    set(PRIVATE_LIBS "${PRIVATE_LIBS} ${IT}")
endforeach()

find_package(PkgConfig REQUIRED QUIET)
pkg_search_module(gdstk REQUIRED gdstk IMPORTED_TARGET)
if(TARGET PkgConfig::gdstk)
	message(STATUS "Found gdstk")
endif()

#include_directories("/usr/local/include/gdstk/")
#include_directories(${gdstk_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --std=c++17 -lstdc++fs")

add_executable(gdstk_app main.cpp)

target_link_libraries(gdstk_app ${ZLIB_LIBRARIES} PkgConfig::gdstk)
#target_link_libraries(gdstk_app ${gdstk_LIBS})
