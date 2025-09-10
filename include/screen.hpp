#pragma once
#include <array>
#include <string>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <random>
#include "types.hpp"
#include "model.hpp"

std::tuple<float, float, float> calculateBarycentricCoordinates(Vector2 p, Vector2 a, Vector2 b, Vector2 c)
{
    float denominator = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    float u = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denominator;
    float v = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denominator;
    float w = 1.0 - u - v;
    return std::make_tuple(u, v, w);
}

Vector3 projectCoordinate(Vector3 p, float focalLength)
{
    float denominator = focalLength + p.z;
    if (denominator == 0.0)
        denominator = 0.00001;
    return Vector3((focalLength * p.x) / denominator, -(focalLength * p.y) / denominator, p.z);
}

float normalizeDepth(const float z, const float nearPlane, const float farPlane)
{
    return (z - nearPlane) / (farPlane - nearPlane);
}

void rotateModel(Model &model, const Axis rotation, const float theta)
{
    const float sinTheta = sinf(theta);
    const float cosTheta = cosf(theta);

    for (Vector3 &vertex : model.getVertices())
    {
        const float x = vertex.x;
        const float y = vertex.y;
        const float z = vertex.z;

        switch (rotation)
        {
        case X:
            vertex.y = y * cosTheta - z * sinTheta;
            vertex.z = y * sinTheta + z * cosTheta;
            break;
        case Y:
            vertex.x = x * cosTheta + z * sinTheta;
            vertex.z = -x * sinTheta + z * cosTheta;
            break;
        case Z:
            vertex.x = x * cosTheta - y * sinTheta;
            vertex.y = x * sinTheta + y * cosTheta;
            break;
        default:
            throw std::runtime_error("Unexpected rotation axis!");
        }
    }
}

template <const int W, const int H>
class ScreenBuffer
{
private:
    std::array<std::array<Color, W>, H> buffer;
    std::array<std::array<float, W>, H> depthBuffer;
    int xOffset;
    int yOffset;

    void DrawTriangle(Vector3 a, Vector3 b, Vector3 c, Color color)
    {
        int maxX = (int)std::ceilf((std::max)(a.x, (std::max)(b.x, c.x)));
        int minX = (int)std::floorf((std::min)(a.x, (std::min)(b.x, c.x)));
        int maxY = (int)std::ceilf((std::max)(a.y, (std::max)(b.y, c.y)));
        int minY = (int)std::floorf((std::min)(a.y, (std::min)(b.y, c.y)));
        float nearPlane = 0.1;
        float farPlane = 100.0;

        Vector2 a2 = {a.x, a.y};
        Vector2 b2 = {b.x, b.y};
        Vector2 c2 = {c.x, c.y};

        for (int y = minY; y < maxY; y++)
            for (int x = minX; x < maxX; x++)
            {
                Vector2 p({(float)x, (float)y});
                float u, v, w;
                std::tie(u, v, w) = calculateBarycentricCoordinates(p, a2, b2, c2);

                if ((u >= 0.0) && (v >= 0.0) && (w >= 0.0))
                {
                    float depth = normalizeDepth(u * a.z + v * b.z + w * c.z, nearPlane, farPlane);

                    if (depth < GetDepthBuffer(x, y))
                    {
                        Set(x, y, color);
                        SetDepthBuffer(x, y, depth);
                    }
                }
            }
    }
    Color Get(int x, int y) const
    {
        int indexX = x + xOffset;
        int indexY = y + yOffset;

        if (indexX < 0 || indexX >= W || indexY < 0 || indexY >= H)
            throw std::runtime_error("Index out of bounds");

        return buffer[y + yOffset][x + xOffset];
    }

    void Set(int x, int y, Color color)
    {
        int indexX = x + xOffset;
        int indexY = y + yOffset;

        if (indexX < 0 || indexX >= W || indexY < 0 || indexY >= H)
            return;

        buffer[y + yOffset][x + xOffset] = color;
    }

    float GetDepthBuffer(const int x, const int y) const
    {
        int indexX = x + xOffset;
        int indexY = y + yOffset;

        if (indexX < 0 || indexX >= W || indexY < 0 || indexY >= H)
            return INFINITY;

        return depthBuffer[y + yOffset][x + xOffset];
    }

    void SetDepthBuffer(int x, int y, float depth)
    {
        int indexX = x + xOffset;
        int indexY = y + yOffset;

        if (indexX < 0 || indexX >= W || indexY < 0 || indexY >= H)
            return;

        depthBuffer[y + yOffset][x + xOffset] = depth;
    }

public:
    ScreenBuffer() : buffer{}, depthBuffer{}, xOffset(W / 2), yOffset(H / 2) {}

    void Clear()
    {
        buffer = {};
        for (auto &row : depthBuffer)
            std::fill(row.begin(), row.end(), INFINITY);
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
        static thread_local std::mt19937 gen(rng());
        std::uniform_int_distribution<int> dist(0, 255);
        size_t faceIndex1, faceIndex2, faceIndex3;

        for (std::tuple<size_t, size_t, size_t> faceIndices : model.getFaces())
        {
            std::tie(faceIndex1, faceIndex2, faceIndex3) = faceIndices;
            const std::vector<Vector3> &vertices = model.getVertices();
            Vector3 vertex1 = projectCoordinate(vertices[faceIndex1], focalLength);
            Vector3 vertex2 = projectCoordinate(vertices[faceIndex2], focalLength);
            Vector3 vertex3 = projectCoordinate(vertices[faceIndex3], focalLength);

            Color color = {
                .r = static_cast<unsigned char>(dist(gen)),
                .g = static_cast<unsigned char>(dist(gen)),
                .b = static_cast<unsigned char>(dist(gen))};
            DrawTriangle(vertex1, vertex2, vertex3, color);
        }
    }
};