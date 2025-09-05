#pragma once
#include <array>
#include <string>
#include <algorithm>
#include <cmath>
#include "types.hpp"
#include "vector.hpp"

const char GRADIENT[] = {' ', '.', '\'', '`', '^', '\"', ',', ':', ';', 'I', 'l', '!', 'i',
                         '>', '<', '~', '+', '_', '-', '?', ']', '[', '}', '{', '1', ')', '(',
                         '|', '\\', '/', '*', 't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z',
                         'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z', 'm', 'w', 'q', 'p',
                         'd', 'b', 'k', 'h', 'a', 'o', '*', '#', 'M', 'W', '&', '8', '%', 'B',
                         '@', '$'};

constexpr unsigned int GRADIENTSIZE = sizeof(GRADIENT) / sizeof(GRADIENT[0]);

bool calculate_barycentric_coordinates(Vector2 p, Vector2 a, Vector2 b, Vector2 c)
{
    float denominator = (b[1] - c[1]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[1] - c[1]);
    float u = ((b[1] - c[1]) * (p[0] - c[0]) + (c[0] - b[0]) * (p[1] - c[1])) / denominator;
    float v = ((c[1] - a[1]) * (p[0] - c[0]) + (a[0] - c[0]) * (p[1] - c[1])) / denominator;
    float w = 1.0 - u - v;
    return (u >= 0.0) && (v >= 0.0) && (w >= 0.0);
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

    std::string Ascii() const
    {
        std::string result;

        for (int y = -yOffset; y < H - yOffset; y++)
        {
            for (int x = -xOffset; x < W - xOffset; x++)
            {
                result.push_back(GetAsciiGradient(x, y));
            }
            result.push_back('\n');
        }

        return result;
    }

    void DrawTriangle(Vector2 a, Vector2 b, Vector2 c, Color color)
    {
        int maxX = std::ceil(std::max(a[0], std::max(b[0], c[0])));
        int minX = std::floor(std::min(a[0], std::min(b[0], c[0])));
        int maxY = std::ceil(std::max(a[1], std::max(b[1], c[1])));
        int minY = std::floor(std::min(a[1], std::min(b[1], c[1])));

        for (int y = minY; y < maxY; y++)
            for (int x = minX; x < maxX; x++)
            {
                Vector2 p({(float)x, (float)y});

                if (calculate_barycentric_coordinates(p, a, b, c))
                {
                    Set(x, y, color);
                }
            }
    }
};