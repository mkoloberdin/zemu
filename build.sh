#!/bin/bash

cd "$(dirname "$0")"
CMAKE_PARAMS=""

for ARG in "$@" ; do
    case "$ARG" in
        --clean)
            [ -e build ] && rm -r build
            ;;

        --sdl1)
            CMAKE_PARAMS="$CMAKE_PARAMS -D USE_SDL1=On"
            ;;

        --analyze)
            CMAKE_PARAMS="$CMAKE_PARAMS -D USE_ANALYZE=On"
            ;;
    esac
done

cmake $CMAKE_PARAMS -B build . && cmake --build build
exit "$?"
