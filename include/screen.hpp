#pragma once
#include <array>
#include <string>
#include <algorithm>
#include <cmath>
#include "types.hpp"
#include "vector.hpp"

const char GRADIENT[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

constexpr unsigned int GRADIENTSIZE = sizeof(GRADIENT) / sizeof(GRADIENT[0]);

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

public:
    ScreenBuffer() : buffer{}, xOffset(W / 2), yOffset(H / 2) {}

    Color Get(int x, int y) const
    {
        return buffer[y + yOffset][x + xOffset];
    }

    void Set(int x, int y, Color color)
    {
        buffer[y + yOffset][x + xOffset] = color;
    }

    char GetAsciiGradient(int x, int y) const
    {
        Color color = Get(x, y);
        int index = std::clamp(int(color.toGray() / 255.0 * (GRADIENTSIZE - 1)), 0, int(GRADIENTSIZE - 1));
        return GRADIENT[index];
    }

    void Clear()
    {
        buffer = {};
    }

    std::string Ascii() const
    {
        std::string result;

        for (int y = -yOffset; y < H - yOffset; y++)
        {
            for (int x = -xOffset; x < W - xOffset; x++)
                result.push_back(GetAsciiGradient(x, y));
            result.push_back('\n');
        }

        return result;
    }

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
};