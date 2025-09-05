#include <iostream>
#include <string>
#include "screen.hpp"
#include "model.hpp"

int main()
{
    ScreenBuffer<500, 500> screenBuffer;
    Vector2 a({-10.0, -10.0});
    Vector2 b({10.0, -10.0});
    Vector2 c({0.0, 10.0});
    Color color = {255, 255, 255};

    screenBuffer.DrawTriangle(a, b, c, color);
    std::cout << screenBuffer.Ascii() << std::endl;

    return 0;
}
