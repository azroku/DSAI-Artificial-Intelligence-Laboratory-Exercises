#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <td/ColorID.h>
#include <td/Circle.h>
#include "Maze.h"
#include <vector>
#include <set>

class MyView : public gui::Canvas {
private:
    Maze maze;
    gui::Shape shape;
    std::vector<std::pair<int, int>> solutionPath;
    std::set<std::pair<int, int>> visitedNodes;

    const gui::CoordType CANVAS_WIDTH = 500.0;
    const gui::CoordType CANVAS_HEIGHT = 500.0;
    
    const gui::CoordType cellSize = 30.0;

protected:

    void onDraw(const gui::Rect& rect) override {
        td::ColorID wallColor = td::ColorID::DarkGray;
        td::ColorID pathColor = td::ColorID::White;
        td::ColorID startColor = td::ColorID::Green;
        td::ColorID endColor = td::ColorID::Red;
        td::ColorID visitedColor = td::ColorID::LightBlue;
        td::ColorID solutionColor = td::ColorID::Yellow;

        int mazeWidth = this->maze.getWidth();
        int mazeHeight = this->maze.getHeight();

        if (mazeWidth == 0 || mazeHeight == 0) return;

        gui::CoordType totalMazeWidth = mazeWidth * cellSize;
        gui::CoordType totalMazeHeight = mazeHeight * cellSize;
        gui::CoordType xOffset = (CANVAS_WIDTH > totalMazeWidth) ? (CANVAS_WIDTH - totalMazeWidth) / 2.0 : 0;
        gui::CoordType yOffset = (CANVAS_HEIGHT > totalMazeHeight) ? (CANVAS_HEIGHT - totalMazeHeight) / 2.0 : 0;
        
        this->shape.createRect(gui::Rect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT));
        this->shape.drawFill(pathColor);

        for (int r = 0; r < mazeHeight; ++r) {
            for (int c = 0; c < mazeWidth; ++c) {
                 if (this->maze.isWall(r, c)) {
                     gui::CoordType left = xOffset + c * cellSize;
                     gui::CoordType top = yOffset + r * cellSize;
                     this->shape.createRect(gui::Rect(left, top, left + cellSize, top + cellSize));
                     this->shape.drawFill(wallColor);
                 }
            }
        }
        
        for (const auto& node : this->visitedNodes) {
             gui::CoordType left = xOffset + node.second * cellSize;
             gui::CoordType top = yOffset + node.first * cellSize;
             this->shape.createRect(gui::Rect(left, top, left + cellSize, top + cellSize));
             this->shape.drawFill(visitedColor);
        }
        
        for (const auto& node : this->solutionPath) {
             gui::CoordType left = xOffset + node.second * cellSize;
             gui::CoordType top = yOffset + node.first * cellSize;
             this->shape.createRect(gui::Rect(left, top, left + cellSize, top + cellSize));
             this->shape.drawFill(solutionColor);
        }

        std::pair<int, int> start = this->maze.getStart();
        std::pair<int, int> end = this->maze.getEnd();
        gui::CoordType radius = cellSize / 2.0;

        gui::CoordType startLeft = xOffset + start.second * cellSize;
        gui::CoordType startTop = yOffset + start.first * cellSize;
        this->shape.createCircle(td::Circle(td::Point(startLeft + radius, startTop + radius), radius));
        this->shape.drawFill(startColor);

        gui::CoordType endLeft = xOffset + end.second * cellSize;
        gui::CoordType endTop = yOffset + end.first * cellSize;
        this->shape.createCircle(td::Circle(td::Point(endLeft + radius, endTop + radius), radius));
        this->shape.drawFill(endColor);
    }

public:
    MyView() : maze("maze.txt") {
        this->reDraw();
    }

    void solveWithBFS() {
        auto result = bfs(this->maze);
        this->solutionPath = result.first;
        this->visitedNodes = result.second;
        this->reDraw();
    }

    void solveWithDFS() {
        auto result = dfs(this->maze);
        this->solutionPath = result.first;
        this->visitedNodes = result.second;
        this->reDraw();
    }
    
    void clear() {
        this->solutionPath.clear();
        this->visitedNodes.clear();
        this->reDraw();
    }
};
