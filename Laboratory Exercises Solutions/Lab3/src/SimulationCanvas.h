#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/Image.h>
#include <gui/DrawableString.h>
#include <td/Types.h>
#include "VisualModels.h"

class SimulationCanvas : public gui::Canvas {
private:
    gui::Shape shape;
    
    gui::Image imgShip;
    gui::Image imgTruck;

    gui::DrawableString labelShip;
    gui::DrawableString labelSortingYard;
    gui::DrawableString labelTrucks;

    const std::vector<VisualContainer>& visualContainers;
    const std::vector<VisualTruck>& visualTrucks;
    
    int canvasWidth = 0;
    int canvasHeight = 0;

protected:
    void onResize(const gui::Size& newSize) override {
        this->canvasWidth = newSize.width;
        this->canvasHeight = newSize.height;
        this->reDraw();
    }

    void onDraw(const gui::Rect& rect) override {
        td::ColorID backgroundColor = td::ColorID::White;
        gui::Rect shipArea(50, 50, 350, 750);
        gui::Rect sortingYardArea(400, 50, 800, 750);
        gui::Rect truckArea(850, 50, 1350, 750);

        this->shape.createRect(gui::Rect(0, 0, canvasWidth, canvasHeight));
        this->shape.drawFill(backgroundColor);

        labelShip.draw(gui::Rect(shipArea.left, shipArea.top - 30, shipArea.right, shipArea.top - 5), gui::Font::ID::SystemBold, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
        labelSortingYard.draw(gui::Rect(sortingYardArea.left, sortingYardArea.top - 30, sortingYardArea.right, sortingYardArea.top - 5), gui::Font::ID::SystemBold, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);
        labelTrucks.draw(gui::Rect(truckArea.left, truckArea.top - 30, truckArea.right, truckArea.top - 5), gui::Font::ID::SystemBold, td::ColorID::Black, td::TextAlignment::Center, td::VAlignment::Center);

        this->shape.createRect(sortingYardArea);
        this->shape.drawWire(td::ColorID::Black);

        gui::Rect shipDrawingRect(shipArea.left, shipArea.bottom - SHIP_HEIGHT, shipArea.right, shipArea.bottom);
        imgShip.draw(shipDrawingRect);

        for (const auto& vc : visualContainers) {
            gui::Rect destRect(vc.pos.x, vc.pos.y, vc.pos.x + CONTAINER_WIDTH, vc.pos.y + CONTAINER_HEIGHT);
            vc.image->draw(destRect);
        }

        for (const auto& truck : visualTrucks) {
            imgTruck.draw(gui::Rect(truck.pos.x, truck.pos.y, truck.pos.x + TRUCK_WIDTH, truck.pos.y + TRUCK_HEIGHT));
        }
    }

public:
    SimulationCanvas(const std::vector<VisualContainer>& containers, const std::vector<VisualTruck>& trucks)
        : gui::Canvas(),
          visualContainers(containers),
          visualTrucks(trucks)
    {
        enableResizeEvent(true);
        labelShip = "Ship Cargo";
        labelSortingYard = "Sorting Yard";
        labelTrucks = "Trucks";
        
        std::string assetPath = "C:/Users/HP/Work/CPProjects/DSAI-AI-Laboratory-Exercises/Laboratory Exercises finished/Laboratory Exercise 03/assets/";
        imgShip.load(assetPath + "ship.png");
        imgTruck.load(assetPath + "truck.png");
    }
    
    void refresh() {
        reDraw();
    }
};
