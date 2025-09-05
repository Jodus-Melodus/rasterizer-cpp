#include "vector.hpp"

Vector2::Vector2(std::array<float, 2> data)
{
    this->data = data;
}

const float Vector2::operator[](const int index)
{
    return this->data.at(index);
}

Vector3::Vector3(std::array<float, 3> data)
{
    this->data = data;
}

const float Vector3::operator[](const int index)
{
    return this->data.at(index);
}
