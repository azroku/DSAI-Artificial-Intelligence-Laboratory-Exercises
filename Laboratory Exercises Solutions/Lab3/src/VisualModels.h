#pragma once
#include <gui/Image.h>
#include "Containers.h"
#include "Truck.h"

const gui::CoordType CONTAINER_WIDTH = 100.0;
const gui::CoordType CONTAINER_HEIGHT = 70.0;
const gui::CoordType CONTAINER_SPACING_Y = 75.0;
const gui::CoordType CONTAINER_SPACING_X = 105.0;

const gui::CoordType TRUCK_WIDTH = 300.0;
const gui::CoordType TRUCK_HEIGHT = 100.0;
const gui::CoordType SHIP_HEIGHT = 400.0;

const gui::CoordType TRUCK_BED_X_OFFSET = -120.0;
const gui::CoordType TRUCK_BED_Y_OFFSET = 20;

struct VisualContainer {
    const Container* logicalContainer;
    gui::Image* image;
    
    gui::Point pos;
    gui::Point targetPos;
    bool isMoving = false;
    
    VisualContainer(const Container* logical, gui::Image* img, gui::Point startPos)
        : logicalContainer(logical), image(img), pos(startPos), targetPos(startPos) {}
};

struct VisualTruck {
    gui::Point pos;
    TruckType type;
    int containerCount = 0;
    
    VisualTruck(gui::Point p, TruckType t) : pos(p), type(t) {}
};
