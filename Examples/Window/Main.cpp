#include "NayukiYq/Application.h"

int main()
{

    Application app = Application();

    app.Initialize();
    app.Update();
    app.CleanUp();

    return 0;
}