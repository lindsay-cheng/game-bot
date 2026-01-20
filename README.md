# reversi (othello)

This repo contains an reversi bot + a small terminal UI to play against it

## run

Build and run with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/reversi
```

Notes:
- `build/` is generated build output (safe to delete).
- if you don't have `cmake` installed, you can install it locally via a python venv:

```bash
python3 -m venv .venv
. .venv/bin/activate
pip install cmake
cmake -S . -B build
cmake --build build
./build/reversi
```
