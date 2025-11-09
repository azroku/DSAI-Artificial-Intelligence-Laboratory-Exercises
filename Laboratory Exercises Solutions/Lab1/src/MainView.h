#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include <gui/HorizontalLayout.h>
#include <gui/VerticalLayout.h>
#include "MyView.h"

class MainView : public gui::View
{
protected:
    MyView mazeCanvas;
    
    gui::Button bfsButton;
    gui::Button dfsButton;
    gui::Button clearButton;
    
    gui::HorizontalLayout buttonLayout;
    gui::VerticalLayout mainLayout;

public:
    MainView()
    : bfsButton("Solve with BFS")
    , dfsButton("Solve with DFS")
    , clearButton("Clear")
    , buttonLayout(3)
    , mainLayout(2)
    {
        this->buttonLayout.append(this->bfsButton);
        this->buttonLayout.append(this->dfsButton);
        this->buttonLayout.append(this->clearButton);
        
        this->mainLayout.append(this->buttonLayout);
        this->mainLayout.append(this->mazeCanvas);
        
        this->setLayout(&this->mainLayout);
    }

    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &this->bfsButton) {
            this->mazeCanvas.solveWithBFS();
            return true;
        }
        
        if (pBtn == &this->dfsButton) {
            this->mazeCanvas.solveWithDFS();
            return true;
        }

        if (pBtn == &this->clearButton) {
            this->mazeCanvas.clear();
            return true;
        }
        
        return false;
    }
};
