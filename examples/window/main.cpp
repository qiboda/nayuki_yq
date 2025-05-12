#include "nayuki_yq/application.h"

int main()
{
    Application app = Application();

    app.Initialize();
    app.Update();
    app.CleanUp();

    return 0;
}
