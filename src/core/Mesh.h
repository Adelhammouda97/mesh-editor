#pragma once

#include <vector>
#include <cstddef>
#include <Eigen/Dense>
#include "HalfEdge.h"

class Mesh
{
private:
    std::vector<Eigen::Vector3d> V;
    std::vector<Eigen::Vector3i> F;

    std::vector<Vertex *> TabVertex;
    std::vector<Face *> TabFace;
    std::vector<HalfEdge *> TabHalfEdge;
    void buildTopology();

public:
    void buildFromTriangles( // Interface for builds half-edge connectivity from vertex positions and triangle indices.
        const std::vector<Eigen::Vector3d> &positions,
        const std::vector<Eigen::Vector3i> &triangles);

    std::size_t vertexCount() const;
    std::size_t faceCount() const;
    std::size_t halfEdgeCount() const;
};