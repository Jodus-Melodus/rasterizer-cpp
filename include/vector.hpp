#include <array>

class Vector2
{
private:
    std::array<float, 2> data;

public:
    Vector2(std::array<float, 2>);
    const float operator[](const int index);
    ~Vector2();
};
