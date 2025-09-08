#pragma once
#include <array>
#include <string>
#include <algorithm>
#include <cmath>
#include <random>
#include "types.hpp"
#include "model.hpp"
#include "vector.hpp"

bool calculateBarycentricCoordinates(Vector2 p, Vector2 a, Vector2 b, Vector2 c)
{
    float denominator = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    float u = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denominator;
    float v = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denominator;
    float w = 1.0 - u - v;
    return (u >= 0.0) && (v >= 0.0) && (w >= 0.0);
}

Vector2 projectCoordinate(Vector3 p, float focalLength)
{
    float denominator = focalLength + p.z;
    if (denominator == 0.0)
        denominator = 0.00001;
    float projectedX = (focalLength * p.x) / denominator;
    float projectedY = (focalLength * p.y) / denominator;
    return Vector2(projectedX, projectedY);
}

template <const int W, const int H>
class ScreenBuffer
{
private:
    std::array<std::array<Color, W>, H> buffer;
    int xOffset;
    int yOffset;

    void DrawTriangle(Vector2 a, Vector2 b, Vector2 c, Color color)
    {
        int maxX = (int)std::ceilf((std::max)(a.x, (std::max)(b.x, c.x)));
        int minX = (int)std::floorf((std::min)(a.x, (std::min)(b.x, c.x)));
        int maxY = (int)std::ceilf((std::max)(a.y, (std::max)(b.y, c.y)));
        int minY = (int)std::floorf((std::min)(a.y, (std::min)(b.y, c.y)));

        for (int y = minY; y < maxY; y++)
            for (int x = minX; x < maxX; x++)
            {
                Vector2 p({(float)x, (float)y});

                if (calculateBarycentricCoordinates(p, a, b, c))
                {
                    Set(x, y, color);
                }
            }
    }
    Color Get(int x, int y) const
    {
        return buffer[y + yOffset][x + xOffset];
    }

    void Set(int x, int y, Color color)
    {
        buffer[y + yOffset][x + xOffset] = color;
    }

public:
    ScreenBuffer() : buffer{}, xOffset(W / 2), yOffset(H / 2) {}

    void Clear()
    {
        buffer = {};
    }

    std::string Display() const
    {
        std::string result;

        for (int y = -yOffset; y < H - yOffset; y++)
        {
            for (int x = -xOffset; x < W - xOffset; x++)
                result.append(Get(x, y).display());
            result.push_back('\n');
        }

        return result;
    }

    void drawModel(const Model &model, float focalLength)
    {
        std::random_device rng;
        std::mt19937 gen(rng());
        std::uniform_int_distribution<int> dist(0, 255);
        size_t faceIndex1, faceIndex2, faceIndex3;

        for (std::tuple<size_t, size_t, size_t> faceIndices : model.getFaces())
        {
            std::tie(faceIndex1, faceIndex2, faceIndex3) = faceIndices;
            std::vector<Vector3> vertices = model.getVertices();
            Vector2 vertex1 = projectCoordinate(vertices[faceIndex1], focalLength);
            Vector2 vertex2 = projectCoordinate(vertices[faceIndex2], focalLength);
            Vector2 vertex3 = projectCoordinate(vertices[faceIndex3], focalLength);
            Color color = {
                .r = static_cast<unsigned char>(dist(gen)),
                .g = static_cast<unsigned char>(dist(gen)),
                .b = static_cast<unsigned char>(dist(gen))};
            DrawTriangle(vertex1, vertex2, vertex3, color);
        }
    }
};