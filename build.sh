#!/bin/bash

cd "$(dirname "$0")"

if [ "$1" = "--clean" ] || [ "$2" = "--clean" ] ; then
    [ -e build ] && rm -r build
fi

CMAKE_PARAMS=""
USE_SDL1="no"

if [ "$1" = "--sdl1" ] || [ "$2" = "--sdl1" ] ; then
    CMAKE_PARAMS="-D USE_SDL1=1"
    USE_SDL1="yes"
fi

cmake $CMAKE_PARAMS -D CMAKE_FIND_FRAMEWORK=LAST -B build . && cmake --build build
[ "$?" != "0" ] && exit "$?"

if [ "$USE_SDL1" = "yes" ] && [ "$(uname -s)" = "Darwin" ] && [ -e build/zemu ] ; then
    ruby osx-mojave-fix.rb build/zemu
fi
