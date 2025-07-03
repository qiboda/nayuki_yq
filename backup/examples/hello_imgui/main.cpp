#include "nayuki_yq/application.h"

#include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_vulkan.h"

int main()
{
    Application app = Application();

    app.Initialize();
    app.Update();
    app.CleanUp();

    return 0;
}
