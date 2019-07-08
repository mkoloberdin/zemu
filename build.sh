#!/bin/bash

cd "$(dirname "$0")"

if [ "$1" = "--clean" ] ; then
	[ -e build ] && rm -r build
fi

cmake -D USE_SDL1=1 -D CMAKE_FIND_FRAMEWORK=LAST -B build . && cmake --build build

if [ "$(uname -s)" = "Darwin" ] && [ -e build/zemu ] ; then
	ruby osx-mojave-fix.rb build/zemu
fi
