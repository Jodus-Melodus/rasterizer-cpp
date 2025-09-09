#pragma once

const char GRADIENT[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

constexpr unsigned int GRADIENTSIZE = sizeof(GRADIENT) / sizeof(GRADIENT[0]);

struct Color
{
    unsigned char r, g, b;

    char asChar() const
    {
        float gray = (0.299 * (float)r + 0.587 * (float)g + 0.114 * (float)b) / 255.0;
        unsigned int index = roundf(gray * (GRADIENTSIZE - 1));
        return GRADIENT[index];
    }

    std::string display() const
    {
        return std::format("\x1b[38;2;{};{};{}m{}\x1b[0m", r, g, b, asChar());
    }
};

enum Axis
{
    X,
    Y,
    Z
};