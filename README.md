# Ray Casting Engine

A Simple 2D ray-caster, implemented in [ocen](https://github.com/ocen-lang/ocen).

See a demo [here](https://mustafaquraish.github.io/raycasting).

## Build SDL version

Make sure you have the following dependencies installed:

- SDL2 (should be able to do `-lSDL2` in your compiler)
- zlib (should be able to do `-lz` in your compiler)

```shell
$ ocen raycast.oc -o raycast
$ ./raycast
```

## Build WebAssembly version

### With local toolchain

Ensure you have the emscripten SDK installed and activated.

```shell
$ ./meta/build_wasm.sh
$ python -m http.server -d build
```

### Within emscripten docker container

Make sure you have docker installed, and running.

```shell
$ ./meta/build_wasm_docker.sh
$ python -m http.server -d build
```