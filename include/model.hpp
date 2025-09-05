#pragma once
#include <vector>
#include <string>
#include <tuple>

#include "vector.hpp"

class Model
{
private:
    std::vector<Vector3> vertices;
    std::vector<std::tuple<size_t, size_t, size_t>> faces;

public:
    Model(const std::vector<Vector3> &vertices, const std::vector<std::tuple<size_t, size_t, size_t>> &faces) : vertices(std::move(vertices)), faces(std::move(faces)) {};
    Model(const std::string &path);
    const std::vector<Vector3> &getVertices() const;
    const std::vector<std::tuple<size_t, size_t, size_t>> &getFaces() const;
};
