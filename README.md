# reversi (othello)

This repo contains an reversi bot + a small terminal UI to play against it

## run

Build and run with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/reversi
```

## sample

```
Enter the board dimension: 8
Computer plays (B/W) : B
  abcdefgh
a UUUUUUUU
b UUUUUUUU
c UUUUUUUU
d UUUWBUUU
e UUUBWUUU
f UUUUUUUU
g UUUUUUUU
h UUUUUUUU
Computer places B at cd.
  abcdefgh
a UUUUUUUU
b UUUUUUUU
c UUUBUUUU
d UUUBBUUU
e UUUBWUUU
f UUUUUUUU
g UUUUUUUU
h UUUUUUUU
Enter move for W (row col): cc
Player places W at cc.
  abcdefgh
a UUUUUUUU
b UUUUUUUU
c UUWBUUUU
d UUUWBUUU
e UUUBWUUU
f UUUUUUUU
g UUUUUUUU
h UUUUUUUU
Computer places B at dc.
...
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
