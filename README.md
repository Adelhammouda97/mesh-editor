# Mesh Editor

Interactive 3D mesh editor in C++20 based on the Half-Edge data structure.
Project for the C++ & Software Engineering module, Université Bourgogne Europe, 2025-2026.

## Build (Windows)

    cmake -B build
    cmake --build build --config Debug
    ctest --test-dir build -C Debug --output-on-failure

## Structure

- `src/core`: Half-Edge topology (no graphics)
- `src/io`: OBJ/PLY import/export
- `src/render`: rendering
- `src/commands`: undo/redo
- `tests`: unit tests
- `docs`: report and UML