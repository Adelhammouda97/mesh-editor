# Mesh Editor

Interactive 3D mesh editor in C++20 based on the Half-Edge data structure.
Project for the C++ & Software Engineering module, Université Bourgogne Europe, 2025-2026.

## Project Structure

- `src/core`: Half-Edge topology (no graphics)
- `src/io`: OBJ/PLY import/export
- `src/render`: rendering
- `src/commands`: undo/redo
- `tests`: unit tests
- `docs`: report and UML

## Build and Test

    cmake -B build
    cmake --build build --config Debug
    ctest --test-dir build -C Debug --output-on-failure

## Development Workflow

We will use feature branches and pull requests to merge changes into main.

1. Switch to main and pull the latest changes before starting a new task.

2. Create a branch for that task, for example feature/mesh-import or feature/mesh-cleanup.

3. Commit and push changes to that branch.

4. Open a pull request targeting main, with a short description of the changes and what was checked.

5. Ask the other person to review it if needed, then merge once the build passes.

6. After merging, update the local main before creating the next branch.

Do not commit generated build files or machine-specific settings. Discuss changes to the public Mesh interface before making them, since both parts of the project depend on it.

## Current Status

- The core creates vertices, faces and three half-edges per triangle, then connects matching twins.

- A half-edge without a matching reverse edge keeps twin == nullptr. This is expected at an open boundary.

- Missing twins and duplicate directed edges are reported to the console.

- Input validation is not implemented yet. Use valid triangle indices.

- Call buildFromTriangles only once per Mesh object for now: clearing and rebuilding are not implemented.

- Memory cleanup is still pending; the current implementation allocates topology objects with raw pointers.

- Access to geometry and topology for rendering and selection needs to be added.

- The project builds successfully, but the existing smoke test does not verify topology correctness.

## Using the Mesh Core

Mesh builds half-edge topology from vertex positions and triangle indices. File reading belongs to the application layer: the importer reads the file and passes the resulting data to Mesh.
1. Include core/Mesh.h.
2. Prepare the input:
    - std::vector<Eigen::Vector3d> positions: vertex coordinates (x, y, z).
    - std::vector<Eigen::Vector3i> triangles: three vertex indices per triangle.
3. Create a Mesh object and call mesh.buildFromTriangles(positions, triangles).
4. Use mesh.vertexCount(), mesh.faceCount() and mesh.halfEdgeCount() to read the resulting counts.

Indices start at zero and refer to entries in positions. Input faces must be triangles. Adjacent triangles must traverse their shared edge in opposite directions.
buildFromTriangles copies the input and calls the private buildTopology() method internally. The application should not call buildTopology() directly.
