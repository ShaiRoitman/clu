cmake_minimum_required (VERSION 2.8.11)
project (CommandLineUtility)

set (BOOST_BASE_DIR "C:/git/boost/boost_1_65_0")
set (BOOST_INCLUDE ${BOOST_BASE_DIR})
set (BOOST_LIBS "${BOOST_BASE_DIR}/stage/lib")

file(GLOB_RECURSE sources	"sources/*.cpp")
file(GLOB_RECURSE headers	"headers/*.h")
file(GLOB_RECURSE external	"include/*.h")
file(GLOB_RECURSE cmakefile "CMakeLists.txt")

include_directories(${BOOST_INCLUDE})
link_directories(${BOOST_LIBS})

add_executable (clu ${sources} ${headers} ${external} ${cmakefile})

source_group("Sources" ${sources})
source_group("Headers" ${headers})
source_group("External" ${external})
