cmake_minimum_required (VERSION 3.4.0)
project (CommandLineUtility CXX)

set (BOOST_ROOT "C:/dev/boost_1_70_0")

if(DEFINED ENV{CMAKE_SYSTEM_ROOTS})
	include ($ENV{CMAKE_SYSTEM_ROOTS} OPTIONAL)
	message ("Using $ENV{CMAKE_SYSTEM_ROOTS}")
endif()

if (NOT UNIX)
	set (Boost_USE_STATIC_RUNTIME OFF)
	set (Boost_USE_STATIC_LIBS ON)
	set (Boost_USE_MULTITHREADED ON)
endif()

set (Boost_DETAILED_FAILURE_MSG ON)
find_package(Boost COMPONENTS program_options regex system)

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()

file(GLOB_RECURSE clh_sources			"CommandLineHandlers/*.cpp")
file(GLOB_RECURSE is_sources			"InputSource/*.cpp")
file(GLOB_RECURSE op_sources			"Operators/*.cpp")
file(GLOB_RECURSE output_sources		"OutputHandlers/*.cpp")
file(GLOB_RECURSE utils_sources			"Utils/*.cpp")
file(GLOB_RECURSE headers				"*.h")
file(GLOB_RECURSE cmakefile				"CMakeLists.txt" "ReleaseNotes.txt")

set (sources ${clh_sources} ${is_sources} ${op_sources} ${output_sources} ${utils_sources} "CommandLineUtility.cpp")

include_directories(".")
include_directories("Utils")
include_directories("InputSource")
include_directories("OutputHandlers")


include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable (clu ${sources} ${headers} ${external} ${cmakefile})
set_target_properties(clu PROPERTIES LINKER_LANGUAGE CXX)

if (UNIX)
   set (AUX_LIBS "pthread")
   target_link_libraries(clu ${Boost_LIBRARIES}   ${AUX_LIBS})
else()
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(UNIX)

if(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
    message("Please consider to switch to CMake 3.8.0")
else()
    source_group(TREE "." PREFIX "Sources" FILES ${sources})
    source_group(TREE "." PREFIX "Headers" FILES ${headers})
endif()

source_group("BuildSystem" FILES ${cmakefile})
source_group("External" FILES ${external})

set_property(TARGET clu PROPERTY CXX_STANDARD 11)
