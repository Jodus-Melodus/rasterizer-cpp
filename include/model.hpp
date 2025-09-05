#pragma once
#include <vector>
#include <string>

#include "vector.hpp"

class Model
{
private:
    std::vector<Vector3> vertices;
    std::vector<size_t> edges;

public:
    Model(const std::vector<Vector3> &vertices, const std::vector<size_t> &edges) : vertices(std::move(vertices)), edges(std::move(edges)) {};
    Model(const std::string &path);
};
