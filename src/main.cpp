#include <iostream>

#include "screen.hpp"

int main()
{
    ScreenBuffer<10, 10> screen;
    std::cout << screen.Ascii();

    return 0;
}