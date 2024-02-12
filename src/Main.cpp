#define STB_IMAGE_IMPLEMENTATION
#include "Application.hpp"

const uint32_t SCREEN_WIDTH = 1000;
const uint32_t SCREEN_HEIGHT = 1000;

int main()
{
    Application* app = new Application{
        {SCREEN_WIDTH, SCREEN_HEIGHT},
        "Platformer"
    };
    app->start();

    delete app;

    return 0;
}
