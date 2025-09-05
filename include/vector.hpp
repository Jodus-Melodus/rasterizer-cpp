#pragma once
#include <array>

class Vector2
{
private:
    std::array<float, 2> data;

public:
    Vector2(std::array<float, 2>);
    const float operator[](const int index);
};

class Vector3
{
private:
    std::array<float, 3> data;

public:
    Vector3(std::array<float, 3> data);
    const float operator[](const int index);
};
