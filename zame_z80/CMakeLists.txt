cmake_minimum_required(VERSION 2.8)
project(zame_z80)

if(CMAKE_COMPILER_IS_GNUCC)
	set (CMAKE_C_FLAGS "-O2 -fomit-frame-pointer -Wall -pipe -std=gnu99 -pedantic")
endif()

include_directories("${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE ns_sources
	src/*.ns.*
)

set(z80ex_wrapper_sources)
foreach(ns_file ${ns_sources})
	string(REPLACE ".ns" "" dest_file ${ns_file})
	string(REPLACE "src/" "processed/" dest_file ${dest_file})
	add_custom_command(
		OUTPUT ${dest_file}
		COMMAND ruby ${CMAKE_CURRENT_SOURCE_DIR}/nspp ${ns_file} ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/processed
		DEPENDS ${ns_file}
	)
	list(APPEND z80ex_wrapper_sources ${dest_file})
endforeach()

# message ("z80ex_wrapper_sources = ${z80ex_wrapper_sources}")

add_library(z80ex_wrapper STATIC ${z80ex_wrapper_sources})
# set_target_properties(z80ex_wrapper PROPERTIES OUTPUT_NAME z80ex_dasm)

add_dependencies(z80ex_wrapper z80ex_wrapper_sources)
