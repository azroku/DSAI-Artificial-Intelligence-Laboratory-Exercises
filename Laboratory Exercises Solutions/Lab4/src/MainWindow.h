#pragma once
#include <gui/Window.h>
#include "MainView.h" 

class MainWindow : public gui::Window
{
private:
    MainView _mainView; 

public:
    MainWindow()
        : gui::Window(gui::Size(800, 600)) // We can make the window a bit wider now
    {
        setTitle(tr("8-Puzzle Solver"));

        // Set our new MainView as the central content of the window.
        setCentralView(&_mainView);
    }
};