#include "model.hpp"
#include "vector.hpp"

#include <fstream>
#include <sstream>
#include <array>

Model::Model(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open obj file");

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (ss >> token)
        {
            tokens.push_back(token);
        }

        if (tokens[0] == "v")
        {
            if (tokens.size() != 4)
                throw std::runtime_error("Invalid object file: expected 3 vertex coordinates");

            std::array<float, 3> values = {
                std::stof(tokens[1]),
                std::stof(tokens[2]),
                std::stof(tokens[3])};
            Vector3 vec(values);
            vertices.push_back(vec);
        }
        else if (tokens[0] == "f")
        {
            std::vector<size_t> faceIndices;
            for (size_t i = 1; i < tokens.size(); i++)
            {
                std::vector<size_t> indices;
                std::istringstream ss(tokens[i]);
                std::string token;

                while (std::getline(ss, token, '/'))
                {
                    indices.push_back(std::stoi(token));
                }

                if (indices.size() != 3)
                    throw std::runtime_error("Invalid object file: expected 3 face indices");

                size_t vertexIndex = indices[0] - 1;
                size_t textureIndex = indices[1] - 1;
                size_t normalIndex = indices[2] - 1;

                faceIndices.push_back(vertexIndex);
            }

            for (size_t i = 1; i < faceIndices.size() - 1; i++)
            {
                faces.push_back(std::make_tuple(faceIndices[0], faceIndices[i], faceIndices[i + 1]));
            }
        }
    }
}

const std::vector<Vector3> &Model::getVertices() const
{
    return vertices;
}

const std::vector<std::tuple<size_t, size_t, size_t>> &Model::getFaces() const
{
    return faces;
}
