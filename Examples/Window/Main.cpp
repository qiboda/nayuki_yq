#include <NayukiYq/GlfwGeneral.h>

#include <flecs.h>

int main() {

    std::cout << "main" << std::endl;

    flecs::world ecs;
    ecs.import <WindowModule>();

    ecs.progress();

    std::cout << "ecs process over" << std::endl;

    auto window = ecs.get<Window>();

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window->pWindow)) {

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        /*渲染过程，待填充*/

        glfwPollEvents();
        ecs.progress(deltaTime);

        std::cout << "deltaTime: " << deltaTime << std::endl;
    }

    std::cout << "terminate window" << std::endl;

    TerminateWindow();
    return 0;
}