#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include <Windows.h>
#include "screen.hpp"
#include "model.hpp"

using namespace std::chrono_literals;

void EnableANSI()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

int main()
{
    EnableANSI();
    ScreenBuffer<208, 50> screen;
    Model model("objects\cube.obj");
    float focalLength = 50.0;
    bool running = true;

    while (running)
    {

        // Keyboard input
        if (_kbhit())
        {
            int ch = _getch();
            if (ch == 27)
            {
                running = false;
            }
        }

        screen.Clear();
        screen.drawModel(model, focalLength);
        std::cout << "\x1b[2J\x1b[H";
        std::cout << screen.Display() << std::endl;
        std::this_thread::sleep_for(100ms);
    }
    return 0;
}
