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
    using clock = std::chrono::high_resolution_clock;
    EnableANSI();
    ScreenBuffer<208, 50> screen;
    auto lastFrame = clock::now();
    Model model("../../objects/nissan.obj");
    float focalLength = 100.0;
    bool running = true;
    float rotationSpeed = 1.0;

    while (running)
    {
        auto now = clock::now();
        std::chrono::duration<float> delta = now - lastFrame;
        lastFrame = now;
        float deltaTime = delta.count();

        // Keyboard input
        if (_kbhit())
        {
            int ch = _getch();
            if (ch == 27)
            {
                running = false;
            }
        }

        rotateModel(model, Axis::Y, deltaTime * rotationSpeed);
        screen.Clear();
        screen.drawModel(model, focalLength);
        std::cout << "\x1b[2J\x1b[H";
        std::cout << screen.Display() << std::flush;
        std::this_thread::sleep_for(100ms);
    }
    return 0;
}
