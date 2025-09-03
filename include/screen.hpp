#pragma once
#include <array>

template <const int W, const int H>
class ScreenBuffer
{
private:
    std::array<std::array<int, W>, H> buffer;
    unsigned int xOffset;
    unsigned int yOffset;

public:
    ScreenBuffer() : buffer{}, xOffset(W / 2), yOffset(H / 2) {}

    int get(int x, int y)
    {
        return buffer[y + yOffset][x + xOffset];
    }

    void set(int x, int y, int value)
    {
        buffer[y + yOffset][x + xOffset] = value;
    }
};
