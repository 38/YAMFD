cmake_minimum_required(VERSION 2.6)
project(yamfd)

set(CMAKE_USE_RELATIVE_PATHS ON)

file(MAKE_DIRECTORY bin/lib)
file(MAKE_DIRECTORY bin/test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

if("$ENV{O}" STREQUAL "")
	set(OPTLEVEL	0)
else("$ENV{O}" STREQUAL "")
	set(OPTLEVEL	$ENV{O})
endif("$ENV{O}" STREQUAL "")

if(NOT "$ENV{CC}" STREQUAL "")
	set(CMAKE_C_COMPILER "$ENV{CC}")
endif(NOT "$ENV{CC}" STREQUAL "")

set(CFLAGS -O${OPTLEVEL}\ -Wall\ -Werror\ -g)

include_directories("include" ".")

enable_testing()

aux_source_directory(src/ source)
set_source_files_properties(${source} PROPERTIES COMPILE_FLAGS ${CFLAGS})

add_executable(yamfd ${source})
