cmake_minimum_required (VERSION 2.8)
project (zame_z80)

find_program (RUBY_EXECUTABLE "ruby")

if (RUBY_EXECUTABLE)
	message ("Ruby found")
else ()
	message (FATAL_ERROR "Ruby not found (required to build zame_z80 core)")
endif ()

if (CMAKE_COMPILER_IS_GNUCC)
	set (CMAKE_C_FLAGS "-O2 -fomit-frame-pointer -Wall -pipe -std=gnu99 -pedantic")
endif ()

file (GLOB_RECURSE ns_sources
	src/*.ns.*
)

if (DEFINED ZAME_SRC_OUTPUT_DIR)
	set (OUTPUT_DIR ${ZAME_SRC_OUTPUT_DIR})
else ()
	set (OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/processed")
endif ()

include_directories(${OUTPUT_DIR})
set (z80ex_wrapper_sources)

foreach (ns_file ${ns_sources})
	string (REPLACE ".ns" "" dest_file ${ns_file})
	string (REGEX REPLACE "^.*src/(.+)$" "${OUTPUT_DIR}/\\1" dest_file ${dest_file})

	add_custom_command(
		OUTPUT ${dest_file}
		COMMAND ${RUBY_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/nspp ${ns_file} ${CMAKE_CURRENT_SOURCE_DIR}/src ${OUTPUT_DIR}
		DEPENDS ${ns_file}
	)

	list (APPEND z80ex_wrapper_sources ${dest_file})
endforeach()

# message ("z80ex_wrapper_sources = ${z80ex_wrapper_sources}")
add_library (z80ex_wrapper STATIC ${z80ex_wrapper_sources})
# set_target_properties (z80ex_wrapper PROPERTIES OUTPUT_NAME z80ex_dasm)
