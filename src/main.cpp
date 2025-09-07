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
    ScreenBuffer<500, 200> screenBuffer;

    bool running = true;
    while (running)
    {
        std::cout << "\033[H\033[J"; // Clear screen and move cursor to Home

        // Keyboard input
        if (_kbhit())
        {
            int ch = _getch();
            if (ch == 27)
            {
                running = false;
            }
        }

        Vector2 a(-10, -10);
        Vector2 b(10, -10);
        Vector2 c(0, 10);
        Color color = {100, 0, 255};
        screenBuffer.DrawTriangle(a, b, c, color);

        std::cout << screenBuffer.Ascii() << std::endl;

        screenBuffer.Clear();
        std::this_thread::sleep_for(500ms);
    }
    return 0;
}
