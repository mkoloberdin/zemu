#!/bin/bash

cd "$(dirname "$0")"

if [ "$1" = "--clean" ] || [ "$2" = "--clean" ] || [ "$3" = "--clean" ] ; then
    [ -e build ] && rm -r build
fi

CMAKE_PARAMS=""
USE_SDL1="no"
USE_ANALYZE="no"

if [ "$1" = "--sdl1" ] || [ "$2" = "--sdl1" ] || [ "$3" = "--sdl1" ] ; then
    CMAKE_PARAMS="$CMAKE_PARAMS -D USE_SDL1=On"
    USE_SDL1="yes"
fi

if [ "$1" = "--analyze" ] || [ "$2" = "--analyze" ] || [ "$3" = "--analyze" ] ; then
    CMAKE_PARAMS="$CMAKE_PARAMS -D CMAKE_BUILD_TYPE=Release"
    USE_ANALYZE="yes"
fi

cmake $CMAKE_PARAMS -D CMAKE_FIND_FRAMEWORK=LAST -D CMAKE_EXPORT_COMPILE_COMMANDS=On -B build . && cmake --build build
STATUS="$?" ; [ "$STATUS" != "0" ] && exit "$STATUS"

if [ "$USE_SDL1" = "yes" ] && [ "$(uname -s)" = "Darwin" ] && [ -e build/zemu ] ; then
    ruby osx-mojave-fix.rb build/zemu
fi

if [ "$USE_ANALYZE" = "yes" ] ; then
    mkdir -p ./build-analyze
    cd ./build-analyze

    [ -e result ] && rm -r result

    pvs-studio-analyzer analyze -o analyze.log -i -j 2 -f ../build/compile_commands.json \
    && plog-converter -a "GA;64;OP;CS;MISRA" -t fullhtml -o result analyze.log

    STATUS="$?" ; [ "$STATUS" != "0" ] && exit "$STATUS"
fi

exit 0
