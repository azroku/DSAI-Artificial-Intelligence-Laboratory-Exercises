#pragma once
#include <gui/Application.h>
#include "MainWindow.h" // We need to know about our main window

class Application : public gui::Application
{
protected:
    // This is the core function that NatID calls to create the first window.
    gui::Window* createInitialWindow() override
    {
        return new MainWindow();
    }

public:
    // The constructor simply passes the command-line arguments to the base class.
    Application(int argc, const char** argv)
        : gui::Application(argc, argv)
    {
    }
};