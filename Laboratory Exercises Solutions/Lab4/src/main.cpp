#include "Application.h"
#include <gui/WinMain.h>
#include <iostream>

int main(int argc, const char* argv[])
{
    // 1. Create the application object
    Application app(argc, argv);

    // 2. Initialize the application with a default language (English)
    app.init("EN");

    // 3. Start the application's event loop
    return app.run();
}