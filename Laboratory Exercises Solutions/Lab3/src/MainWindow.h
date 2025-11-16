#pragma once
#include <gui/Window.h>
#include "MainView.h"

class MainWindow : public gui::Window {
protected:
    MainView view;
public:
    MainWindow() : gui::Window(gui::Geometry(50, 50, 1440, 800)) {
        this->setTitle("Port Logistics Simulation");
        this->setCentralView(&view);
    }
};
