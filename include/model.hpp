#pragma once
#include <vector>
#include <string>
#include <tuple>

#include "types.hpp"

class Model
{
private:
    std::vector<Vector3> vertices;
    std::vector<std::tuple<std::tuple<size_t, size_t, size_t>, std::tuple<size_t, size_t, size_t>>> faces;
    std::vector<std::vector<Color>> texture;
    std::vector<Vector2> textureCoordinates;

public:
    Model(const std::string &modelPath, const std::string &texturePath);
    std::vector<Vector3> &getVertices();
    std::vector<std::tuple<std::tuple<size_t, size_t, size_t>, std::tuple<size_t, size_t, size_t>>> &getFaces();
    const std::vector<Vector3> &getVertices() const;
    const std::vector<std::tuple<std::tuple<size_t, size_t, size_t>, std::tuple<size_t, size_t, size_t>>> &getFaces() const;
    const std::vector<Vector2> &getTextureCoordinates() const;
    const std::vector<std::vector<Color>> &getTexture() const;
};
