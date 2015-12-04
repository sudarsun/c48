cmake_minimum_required(VERSION 2.6)
project(c48)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(c48)
add_subdirectory(evaluation)
add_executable(test main.cpp)

target_link_libraries(test LINK_PUBLIC core c48 evaluation)
install(TARGETS test RUNTIME DESTINATION bin)

