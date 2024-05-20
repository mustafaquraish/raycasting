#!/bin/bash

set -xe

ocen raycast.oc -n -c build/raycast.c
docker run --rm -v $(PWD):/mnt/ -w /mnt/ emscripten/emsdk ./meta/build_wasm.sh build/raycast.c
