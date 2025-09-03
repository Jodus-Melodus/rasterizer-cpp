#pragma once
#include <array>
#include <string>
#include <algorithm>

#include "types.hpp"

const char GRADIENT[] = {' ', '.', '\'', '`', '^', '\"', ',', ':', ';', 'I', 'l', '!', 'i',
                         '>', '<', '~', '+', '_', '-', '?', ']', '[', '}', '{', '1', ')', '(',
                         '|', '\\', '/', '*', 't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z',
                         'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z', 'm', 'w', 'q', 'p',
                         'd', 'b', 'k', 'h', 'a', 'o', '*', '#', 'M', 'W', '&', '8', '%', 'B',
                         '@', '$'};

constexpr unsigned int GRADIENTSIZE = sizeof(GRADIENT) / sizeof(GRADIENT[0]);

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

    void Set(int x, int y, Color value)
    {
        buffer[y + yOffset][x + xOffset] = value;
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
};
