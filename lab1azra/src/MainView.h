//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include <gui/Transformation.h>
#include <gui/Font.h>
#include "Maze.h"


class MainView : public gui::Canvas
{
private:
protected:
    bool showPath = false;
    std::vector<Node> path;
    gui::Shape _bomber;
    Maze maze;
    int mazeWidth = 0;
    int mazeHeight = 0;
    int rectWidth = 0;
    int rectHeight = 0;
    td::ColorID color = td::ColorID::Green;
protected:

    void createShapes()
    {
        gui::Rect kva = gui::Rect(0, 0, rectWidth, rectHeight);
        _bomber.createRect(kva);
    }

public:
    MainView()
    {
        createShapes();
    }

    void onDraw(const gui::Rect& rect) override
    {

        gui::Transformation zoomTr;
        double zoom = 1.; //experiment with this and see impact
        double scrollX = 0;
        double scrollY = 0;
        zoomTr.translate(scrollX, scrollY);
        zoomTr.scale(zoom);
        zoomTr.appendToContext(); //this is transformationfor all objects

        for (int i = 0; i < mazeHeight; i++) {
            for (int j = 0; j < mazeWidth; j++) {
                td::ColorID color;
                switch (maze.get(i, j)) {
                case '#':
                    color = td::ColorID::Red;
                    break;
                case 'S':
                    color = td::ColorID::Green;
                    break;
                case 'E':
                    color = td::ColorID::Yellow;
                    break;
                default:
                    color = td::ColorID::White;
                    break;
                }
                {
                    gui::Transformation::saveContext();
                    gui::Transformation tr;
                    // tr.rotateDeg(rotationDeg);
                     tr.translate(j* rectWidth, i * rectHeight);
                     tr.appendToContext();
                    _bomber.drawFillAndWire(color,td::ColorID::Black);
                    //  gui::Point pt(-1,-5);
                      //_strETF.draw(pt, gui::Font::ID::SystemNormal, td::ColorID::Yellow); //text will be translated and rotated as well
                    gui::Transformation::restoreContext();
                }
               }
            }
        if (showPath) {
            for (Node n : path) {
                {
                    gui::Transformation::saveContext();
                    gui::Transformation tr;
                    // tr.rotateDeg(rotationDeg);
                    tr.translate(n.second * rectWidth, n.first * rectHeight);
                    tr.appendToContext();
                    _bomber.drawFillAndWire(td::ColorID::DarkCyan, td::ColorID::Black);
                    //  gui::Point pt(-1,-5);
                      //_strETF.draw(pt, gui::Font::ID::SystemNormal, td::ColorID::Yellow); //text will be translated and rotated as well
                    gui::Transformation::restoreContext();
                }
                showPath = false;
            }
        }
        //{
        //    gui::Transformation::saveContext();
        //    gui::Transformation tr;
        //    tr.translate(700, 200);
        //    // tr.rotateDeg(rotationDeg);
        //    tr.appendToContext();
        //    _bomber.drawFill(td::ColorID::DarkBlue);
        //    //  gui::Point pt(-1,-5);
        //      //_strETF.draw(pt, gui::Font::ID::SystemNormal, td::ColorID::Yellow); //text will be translated and rotated as well
        //    gui::Transformation::restoreContext();
        //}


    }
    void drawMaze() {
        

        reDraw();
    }
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        if (actionID == 0) {
            maze.loadFromFile("../maze.txt");
            mazeWidth = maze.getWidth();
            mazeHeight = maze.getHeight();

            rectHeight = int(950 / mazeHeight);
            rectWidth = int(1200 / mazeWidth);
            createShapes();
            reDraw();
        }
        if (actionID == 1) {
            path= maze.bfs(maze);
            showPath = true;
            reDraw();
        }
        if (actionID == 2) {
            path = maze.dfs(maze);
            showPath = true;
            reDraw();
        }
        return true;
    }


};