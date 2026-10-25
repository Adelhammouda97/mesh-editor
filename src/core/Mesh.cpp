#include "core/Mesh.h"
#include <utility>
#include <unordered_map>
#include <cstddef>
#include <functional>
#include <iostream>

namespace
{
    struct PairHash
    { // Computes hashes for edge lookup, helping us find twin half-edges efficiently.
        std::size_t operator()(const std::pair<int, int> &p) const
        {
            const std::size_t h1 = std::hash<int>{}(p.first);
            const std::size_t h2 = std::hash<int>{}(p.second);
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
}

void Mesh::buildFromTriangles(
    const std::vector<Eigen::Vector3d> &positions,
    const std::vector<Eigen::Vector3i> &triangles)
{
    V = positions;
    F = triangles;
    buildTopology();
}

void Mesh::buildTopology()
{
    // Create all vertices.
    for (std::size_t i = 0; i < V.size(); ++i)
    {
        Eigen::Vector3d tmp = V[i];
        Vertex *p = new Vertex;

        p->x = tmp[0];
        p->y = tmp[1];
        p->z = tmp[2];
        p->he = nullptr;

        TabVertex.push_back(p);
    }

    std::unordered_map<std::pair<int, int>, HalfEdge *, PairHash> hashTab;
    // Triangle processing
    for (std::size_t i = 0; i < F.size(); ++i)
    {
        Eigen::Vector3i Fi = F[i];
        Face *tmpF = new Face;

        HalfEdge *HE0 = new HalfEdge;
        HalfEdge *HE1 = new HalfEdge;
        HalfEdge *HE2 = new HalfEdge;

        HE0->twin = nullptr;
        HE1->twin = nullptr;
        HE2->twin = nullptr;

        HE0->target = TabVertex[Fi[1]];
        HE1->target = TabVertex[Fi[2]];
        HE2->target = TabVertex[Fi[0]];

        HE0->next = HE1;
        HE1->next = HE2;
        HE2->next = HE0;

        HE0->face = tmpF;
        HE1->face = tmpF;
        HE2->face = tmpF;

        tmpF->he = HE0;

        // Store pointers to the three half-edges in an array.
        HalfEdge *localHE[3] = {HE0, HE1, HE2};

        // For each vertex of the current face, set its outgoing half-edge if Vertex::he has not been assigned yet.
        for (int j = 0; j < Fi.size(); ++j)
        {
            if (TabVertex[Fi[j]]->he == nullptr)
            {
                TabVertex[Fi[j]]->he = localHE[j];
            }
        }

        TabFace.push_back(tmpF);
        TabHalfEdge.push_back(HE0);
        TabHalfEdge.push_back(HE1);
        TabHalfEdge.push_back(HE2);

        auto result0 = hashTab.emplace(std::pair<int, int>{Fi[0], Fi[1]}, HE0);
        if (!result0.second)
        {
            std::cerr << "Duplicate directed edge: "
                      << Fi[0] << " -> " << Fi[1] << '\n';
        }
        auto result1 = hashTab.emplace(std::pair<int, int>{Fi[1], Fi[2]}, HE1);
        if (!result1.second)
        {
            std::cerr << "Duplicate directed edge: "
                      << Fi[1] << " -> " << Fi[2] << '\n';
        }
        auto result2 = hashTab.emplace(std::pair<int, int>{Fi[2], Fi[0]}, HE2); // Add directed edges to the lookup table.
        if (!result2.second)
        {
            std::cerr << "Duplicate directed edge: "
                      << Fi[2] << " -> " << Fi[0] << '\n'; // Check whether insertion failed because the key already exists.
        }
    }

    for (const auto &entry : hashTab)
    { // Find and connect twin half-edges.
        const auto &key = entry.first;
        HalfEdge *current = entry.second;
        if (current->twin != nullptr) // Skip half-edges whose twins are already assigned.
        {
            continue;
        }
        int start = key.first;
        int end = key.second;
        std::pair<int, int> reverseKey{end, start}; // Create the reverse key.
        auto found = hashTab.find(reverseKey);
        if (found != hashTab.end())
        {
            HalfEdge *opposite = found->second;
            current->twin = opposite;
            opposite->twin = current;
        }
        else
        { // Report an edge with no matching twin.
            std::cerr << "No twin found for edge "
                      << start << " -> " << end << '\n';
        }
    }
}

std::size_t Mesh::vertexCount() const
{
    return TabVertex.size();
}

std::size_t Mesh::halfEdgeCount() const
{
    return TabHalfEdge.size();
}

std::size_t Mesh::faceCount() const
{
    return TabFace.size();
}