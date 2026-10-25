#pragma once

// Forward declarations
struct Vertex;
struct Face;
struct HalfEdge;

struct Vertex
{
    double x, y, z;
    HalfEdge *he; // One outgoing half-edge
};

struct Face
{
    HalfEdge *he; // One half-edge belonging to this face
};

struct HalfEdge
{
    Vertex *target; // Target vertex of the directed half-edge
    HalfEdge *twin; // Opposite half-edge
    HalfEdge *next; // Next half-edge around the face, CCW
    Face *face;     // Face to which this half-edge belongs
};