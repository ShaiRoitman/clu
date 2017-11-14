cmake_minimum_required (VERSION 3.4.0)
project (CommandLineUtility CXX)

if (NOT UNIX)
	if (IS_DIRECTORY "c:/git/boost_1_65_1")
		set (BOOST_ROOT "c:/git/boost_1_65_1")
	elseif (IS_DIRECTORY "t:/GitRepos/boost_1_65_1")
		set (BOOST_ROOT "t:/GitRepos/boost_1_65_1")
	else()
		message("Build boost")
		message(".\b2 variant=debug address-model=64 link=static threading=multi runtime-link=shared --with-program_options --with-regex --with-system --with-thread --with-date_time")
		message(".\b2 variant=release address-model=64 link=static threading=multi runtime-link=shared --with-program_options --with-regex --with-system --with-thread --with-date_time")
		message(FATAL_ERROR "Boost not found")
	endif()

	set (BOOST_ROOT "c:/git/boost_1_65_1")
	set (Boost_DETAILED_FAILURE_MSG ON)
	set (Boost_USE_STATIC_RUNTIME OFF)
	set (Boost_USE_STATIC_LIBS ON)
	set (Boost_USE_MULTITHREADED ON)
	set (BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
endif()

find_package(Boost COMPONENTS program_options regex system)

file(GLOB_RECURSE clh_sources			"CommandLineHandlers/*.cpp")
file(GLOB_RECURSE is_sources			"InputSource/*.cpp")
file(GLOB_RECURSE op_sources			"Operators/*.cpp")
file(GLOB_RECURSE output_sources		"OutputHandlers/*.cpp")
file(GLOB_RECURSE utils_sources			"Utils/*.cpp")
file(GLOB_RECURSE headers	"*.h")
file(GLOB_RECURSE cmakefile "CMakeLists.txt")

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
