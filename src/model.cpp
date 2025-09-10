#include <fstream>
#include <sstream>
#include <array>
#include <iostream>
#include "model.hpp"
#include "types.hpp"
#include "stb_image.h"

Model::Model(const std::string &modelPath, const std::string &texturePath)
{
    std::ifstream file(modelPath);
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
            tokens.push_back(token);

        if (tokens[0] == "v")
        {
            if (tokens.size() != 4)
                throw std::runtime_error("Invalid object file: expected 3 vertex coordinates");
            float x = std::stof(tokens[1]);
            float y = std::stof(tokens[2]);
            float z = std::stof(tokens[3]);
            Vector3 vec(x, y, z);
            vertices.push_back(vec);
        }
        if (tokens[0] == "vt")
        {
            if (tokens.size() != 3)
                throw std::runtime_error("Invalid object file: expected 2 vertex coordinates");
            float u = std::stof(tokens[1]);
            float v = std::stof(tokens[2]);
            Vector2 coordinate(u, v);
            textureCoordinates.push_back(coordinate);
        }
        else if (tokens[0] == "f")
        {
            std::vector<size_t> faceIndices, textureIndices;
            for (size_t i = 1; i < tokens.size(); i++)
            {
                std::vector<size_t> indices;
                std::istringstream ss(tokens[i]);
                std::string token;

                while (std::getline(ss, token, '/'))
                    indices.push_back(std::stoi(token));

                if (indices.size() != 3)
                    throw std::runtime_error("Invalid object file: expected 3 face indices");

                size_t vertexIndex = indices[0] - 1;
                size_t textureIndex = indices[1] - 1;
                size_t normalIndex = indices[2] - 1;

                faceIndices.push_back(vertexIndex);
                textureIndices.push_back(textureIndex);
            }

            for (size_t i = 1; i < faceIndices.size() - 1; i++)
                faces.push_back(std::make_tuple(
                    std::make_tuple(faceIndices[0], faceIndices[i], faceIndices[i + 1]),
                    std::make_tuple(textureIndices[0], textureIndices[i], textureIndices[i + 1])));
        }
    }

    int width, height, channels;
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        std::cerr << "Failed to load image" << std::endl;
        return;
    }

    std::vector<std::vector<Color>> textureTemp(height, std::vector<Color>(width));

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            int index = (y * width + x) * channels;

            unsigned char r = data[index + 0];
            unsigned char g = (channels > 1) ? data[index + 1] : 0;
            unsigned char b = (channels > 2) ? data[index + 2] : 0;
            unsigned char a = (channels > 3) ? data[index + 3] : 255;
            Color color = {r, g, b};

            textureTemp[y][x] = color;
        }
    }

    texture = textureTemp;

    stbi_image_free(data);
}

std::vector<Vector3> &Model::getVertices()
{
    return vertices;
}

std::vector<std::tuple<std::tuple<size_t, size_t, size_t>, std::tuple<size_t, size_t, size_t>>> &Model::getFaces()
{
    return faces;
}

const std::vector<Vector3> &Model::getVertices() const
{
    return vertices;
}

const std::vector<std::tuple<std::tuple<size_t, size_t, size_t>, std::tuple<size_t, size_t, size_t>>> &Model::getFaces() const
{
    return faces;
}

const std::vector<Vector2> &Model::getTextureCoordinates() const
{
    return textureCoordinates;
}

const std::vector<std::vector<Color>> &Model::getTexture() const
{
    return texture;
}
