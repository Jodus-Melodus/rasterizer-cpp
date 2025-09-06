#include <iostream>
#include <string>
#include "screen.hpp"
#include "model.hpp"

int main()
{
    // ScreenBuffer<500, 500> screenBuffer;
    // Vector2 a({-10.0, -10.0});
    // Vector2 b({10.0, -10.0});
    // Vector2 c({0.0, 10.0});
    // Color color = {255, 255, 255};

    // screenBuffer.DrawTriangle(a, b, c, color);
    // std::cout << screenBuffer.Ascii() << std::endl;
    Vector3 point(0.0, 0.0, -5.0);
    Vector2 projectedPoint = projectCoordinate(point, 10.0);
    std::cout << projectedPoint.x << ", " << projectedPoint.y << std::endl;
    return 0;
}
