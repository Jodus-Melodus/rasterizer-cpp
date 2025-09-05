#include <iostream>
#include <string>
#include "screen.hpp"
#include "model.hpp"

int main()
{

    std::string path = "/workspace/objects/cube.obj";
    try
    {
        Model model(path);
        std::cout << "Loaded model with "
                  << model.getVertices().size() << " vertices and "
                  << model.getFaces().size() << " triangles.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading model: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
