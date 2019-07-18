cmake_minimum_required (VERSION 2.8)
project (zame_z80)

find_program (RUBY_COMMAND "ruby")

if (RUBY_COMMAND)
    message ("Ruby found")
else ()
    message (FATAL_ERROR "Ruby not found (required to build zame_z80 core)")
endif ()

set (CMAKE_C_STANDARD 99)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

if (CMAKE_COMPILER_IS_GNUCC)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fomit-frame-pointer -pipe -pedantic")
endif ()

file (GLOB_RECURSE NS_SOURCES
    src/*.ns.*
)

if (DEFINED ZAME_SRC_OUTPUT_DIR)
    set (OUTPUT_DIR ${ZAME_SRC_OUTPUT_DIR})
else ()
    set (OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/processed")
endif ()

include_directories (${OUTPUT_DIR})
set (Z80EX_WRAPPER_SOURCES)

foreach (NS_FILE ${NS_SOURCES})
    string (REPLACE ".ns" "" DEST_FILE ${NS_FILE})
    string (REGEX REPLACE "^.*src/(.+)$" "${OUTPUT_DIR}/\\1" DEST_FILE ${DEST_FILE})

    add_custom_command (
        OUTPUT ${DEST_FILE}
        COMMAND ${RUBY_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR}/nspp ${NS_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/src ${OUTPUT_DIR}
        DEPENDS ${NS_FILE}
    )

    list (APPEND Z80EX_WRAPPER_SOURCES ${DEST_FILE})
endforeach()

add_library (z80ex_wrapper STATIC ${Z80EX_WRAPPER_SOURCES})
