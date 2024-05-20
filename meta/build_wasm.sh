#!/bin/bash

set -xe

mkdir -p build

if [[ -z "$1" ]]; then
    ocen raycast.oc -n -c build/raycast.c
    FILE=build/raycast.c
else
    FILE=$1
fi


cp assets/index.html build/
emcc $FILE \
    -DOC_NO_BACKTRACE \
    -s USE_SDL=2 \
    -s USE_ZLIB=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    --preload-file assets \
    --use-preload-plugins \
    -o build/index.js