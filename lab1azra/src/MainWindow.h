//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MainView.h"
#include "ToolBar.h"

class MainWindow : public gui::Window
{
protected:
    gui::Image _imgStart;
    gui::Image _imgStop;
    ToolBar _mainToolBar;
    
    MainView _mainView;
protected:
    
    bool shouldClose() override
    {
        return true;
    }
    
public:
    MainWindow()
    : gui::Window(gui::Size(1200, 1200))
    {
        setTitle("Maze - Salih");
        setToolBar(_mainToolBar);
        _mainToolBar.forwardMessagesTo(&_mainView);
       // _mainToolBar.setAsMain(this);
        setCentralView(&_mainView);
    }
    
    ~MainWindow()
    {
    }

};
