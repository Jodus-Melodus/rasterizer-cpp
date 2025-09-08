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
        std::cout << "\033[H\033[J"; // Clear screen and move cursor to Home
        std::cout << screen.Display() << std::endl;
        std::this_thread::sleep_for(100ms);
    }
    return 0;
}
