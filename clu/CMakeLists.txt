cmake_minimum_required (VERSION 2.8.11)
project (CommandLineUtility CXX)

set (BOOST_BASE_DIR "/home/master/boost/boost_1_65_1")
set (BOOST_INCLUDE ${BOOST_BASE_DIR})
set (BOOST_LIBS "${BOOST_BASE_DIR}/stage/lib")

file(GLOB_RECURSE clh_sources			"CommandLineHandlers/*.cpp")
file(GLOB_RECURSE is_sources			"InputSource/*.cpp")
file(GLOB_RECURSE op_sources			"Operators/*.cpp")
file(GLOB_RECURSE output_sources		"OutputHandlers/*.cpp")
file(GLOB_RECURSE utils_sources			"Utils/*.cpp")

set (sources ${clh_sources} ${is_sources} ${op_sources} ${output_sources} ${utils_sources} "CommandLineUtility.cpp")

include_directories(".")
include_directories("Utils")
include_directories("InputSource")
include_directories("OutputHandlers")

file(GLOB_RECURSE headers	"*.h")
file(GLOB_RECURSE cmakefile "CMakeLists.txt")

include_directories(${BOOST_INCLUDE})
link_directories(${BOOST_LIBS})


add_executable (clu ${sources} ${headers} ${external} ${cmakefile})
set_target_properties(clu PROPERTIES LINKER_LANGUAGE CXX)

if (UNIX)
   target_link_libraries(clu libboost_program_options-gcc54-mt-1_65_1.so pthread libboost_regex-gcc54-mt-1_65_1.so libboost_system-gcc54-mt-1_65_1.so)
endif(UNIX)

source_group(TREE "." PREFIX "Sources" FILES ${sources})
source_group(TREE "." PREFIX "Headers" FILES ${headers})
source_group("BuildSystem" FILES ${cmakefile})
#source_group("Headers" ${headers})
#source_group("External" ${external})

set_property(TARGET clu PROPERTY CXX_STANDARD 11)
