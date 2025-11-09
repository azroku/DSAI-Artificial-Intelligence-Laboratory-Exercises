#pragma once
#include <gui/Window.h>
#include "MainView.h"

class MainWindow : public gui::Window
{
protected:
    MainView mainView;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 510, 510))
    {
        this->setTitle("Maze Visualization");
        this->setCentralView(&this->mainView);
    }
};
