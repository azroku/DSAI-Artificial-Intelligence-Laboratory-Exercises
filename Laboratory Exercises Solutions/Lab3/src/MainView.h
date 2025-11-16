#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include <gui/HorizontalLayout.h>
#include <gui/VerticalLayout.h>
#include <gui/Timer.h>
#include "SimulationCanvas.h"
#include "Port.h"

enum class SimState { Idle, UnloadingToxic, UnloadingRefrigerated, UnloadingToSort, LoadingFromSort, Done };

class MainView : public gui::View {
protected:
    Ship ship;
    Port port;

    std::vector<VisualContainer> visualContainers;
    std::vector<VisualTruck> visualTrucks;
    std::vector<std::vector<const Container*>> visualSortingYard;
    
    SimulationCanvas canvas;
    
    gui::Image imgToxic, imgRefrigerated, imgOrdinary, imgFragile;
    
    gui::Button btnStart, btnStep, btnReset;
    gui::HorizontalLayout buttonLayout;
    gui::VerticalLayout mainLayout;
    gui::Timer timer;

    SimState currentState = SimState::Idle;
    size_t logicIndex = 0;

public:
    MainView()
        : ship("Evergreen", 150000),
          canvas(visualContainers, visualTrucks),
          btnStart("Start/Pause"), btnStep("Step"), btnReset("Reset"),
          buttonLayout(3), mainLayout(2),
          timer(this, 0.05, false)
    {
        std::string assetPath = "C:/Users/HP/Work/CPProjects/DSAI-AI-Laboratory-Exercises/Laboratory Exercises finished/Laboratory Exercise 03/assets/";
        imgToxic.load(assetPath + "container_toxic.png");
        imgRefrigerated.load(assetPath + "container_refrigerated.png");
        imgOrdinary.load(assetPath + "container_ordinary.png");
        imgFragile.load(assetPath + "container_fragile.png");

        buttonLayout.append(btnStart);
        buttonLayout.append(btnStep);
        buttonLayout.append(btnReset);
        mainLayout << buttonLayout << canvas;
        this->setLayout(&mainLayout);

        resetSimulation();
    }

    void resetSimulation() {
        timer.stop();
        
        visualContainers.clear();
        visualTrucks.clear();
        visualSortingYard.clear();
        ship = Ship("Evergreen", 150000);

        ship.add_container(ToxicContainer("TOX001", 9000, ToxicityLevel::High));
        ship.add_container(RefrigeratedContainer("REF001", 7500, 4));
        ship.add_container(OrdinaryContainer("ORD001", 6000));
        ship.add_container(OrdinaryContainer("ORD003", 9500));
        ship.add_container(FragileContainer("FRG001", 3000));
        ship.add_container(OrdinaryContainer("ORD002", 4000));
        ship.add_container(ToxicContainer("TOX002", 8000, ToxicityLevel::Medium));
        ship.add_container(ToxicContainer("TOX003", 8000, ToxicityLevel::Medium));

        
        gui::CoordType startX1 = 70.0;
        gui::CoordType startX2 = startX1 + CONTAINER_SPACING_X;
        gui::CoordType startY = 70.0;
        
        int col = 0;
        int row = 0;
        
        auto add_visuals = [&](const auto& container_vec, gui::Image* img) mutable {
            for (const auto& c : container_vec) {
                gui::CoordType currentX = (col == 0) ? startX1 : startX2;
                gui::CoordType currentY = startY + (row * CONTAINER_SPACING_Y);
                visualContainers.emplace_back(&c, img, gui::Point(currentX, currentY));
                
                col++;
                if (col > 1) {
                    col = 0;
                    row++;
                }
            }
        };

        add_visuals(ship.get_toxic_cargo(), &imgToxic);
        add_visuals(ship.get_refrigerated_cargo(), &imgRefrigerated);
        add_visuals(ship.get_ordinary_cargo(), &imgOrdinary);
        add_visuals(ship.get_fragile_cargo(), &imgFragile);

        currentState = SimState::UnloadingToxic;
        logicIndex = 0;
        canvas.refresh();
    }

    VisualContainer* findVisualContainer(const Container* logical) {
        for (auto& vc : visualContainers) {
            if (vc.logicalContainer->getId() == logical->getId()) {
                return &vc;
            }
        }
        return nullptr;
    }

    void step() {
        bool isAnimating = false;
        for (auto& vc : visualContainers) {
            if (vc.isMoving) {
                isAnimating = true;
                vc.pos.x += (vc.targetPos.x - vc.pos.x) * 0.15;
                vc.pos.y += (vc.targetPos.y - vc.pos.y) * 0.15;
                if (std::abs(vc.pos.x - vc.targetPos.x) < 2 && std::abs(vc.pos.y - vc.targetPos.y) < 2) {
                    vc.pos = vc.targetPos;
                    vc.isMoving = false;
                }
            }
        }
        if (isAnimating) return;

        auto place_on_truck = [&](auto& truck, const Container& c) {
            if (auto* vc = findVisualContainer(&c)) {
                
                const int max_cols = 2;
                int current_col = truck.containerCount % max_cols;
                int current_row = truck.containerCount / max_cols;

                int effective_col = (max_cols - 1) - current_col;

                gui::CoordType targetY = truck.pos.y + TRUCK_BED_Y_OFFSET - (current_row * CONTAINER_HEIGHT);
                
                gui::CoordType targetX = truck.pos.x + TRUCK_BED_X_OFFSET + (effective_col * CONTAINER_SPACING_X);

                vc->targetPos = gui::Point(targetX, targetY);
                vc->isMoving = true;
                truck.containerCount++;
            }
        };

        switch (currentState) {
            case SimState::UnloadingToxic:
                if (logicIndex < ship.get_toxic_cargo().size()) {
                    if (visualTrucks.empty() || visualTrucks.back().type != TruckType::ToxicWaste) {
                        visualTrucks.emplace_back(gui::Point(1050, 100), TruckType::ToxicWaste);
                    }
                    place_on_truck(visualTrucks.back(), ship.get_toxic_cargo()[logicIndex]);
                    logicIndex++;
                } else {
                    currentState = SimState::UnloadingRefrigerated;
                    logicIndex = 0;
                }
                break;
            
            case SimState::UnloadingRefrigerated:
                if (logicIndex < ship.get_refrigerated_cargo().size()) {
                    if (visualTrucks.back().type != TruckType::Refrigerated) {
                         visualTrucks.emplace_back(gui::Point(1050, 250), TruckType::Refrigerated);
                    }
                    place_on_truck(visualTrucks.back(), ship.get_refrigerated_cargo()[logicIndex]);
                    logicIndex++;
                } else {
                    currentState = SimState::UnloadingToSort;
                    logicIndex = 0;
                }
                break;

            case SimState::UnloadingToSort: {
                std::vector<const Container*> ptr_cargo;
                for(const auto& c : ship.get_ordinary_cargo()) ptr_cargo.push_back(&c);
                for(const auto& c : ship.get_fragile_cargo()) ptr_cargo.push_back(&c);

                if (logicIndex < ptr_cargo.size()) {
                    const Container* c_ptr = ptr_cargo[logicIndex];
                    if (auto* vc = findVisualContainer(c_ptr)) {
                        bool placed = false;
                        for (size_t i = 0; i < visualSortingYard.size(); ++i) {
                            if (!visualSortingYard[i].empty() && c_ptr->getWeight() <= visualSortingYard[i].back()->getWeight()) {
                                vc->targetPos = gui::Point(420 + i * CONTAINER_SPACING_X, 750 - CONTAINER_HEIGHT - (visualSortingYard[i].size() * CONTAINER_SPACING_Y));
                                vc->isMoving = true;
                                visualSortingYard[i].push_back(c_ptr);
                                placed = true;
                                break;
                            }
                        }
                        if (!placed) {
                            visualSortingYard.push_back({c_ptr});
                            vc->targetPos = gui::Point(420 + (visualSortingYard.size() - 1) * CONTAINER_SPACING_X, 750 - CONTAINER_HEIGHT);
                            vc->isMoving = true;
                        }
                    }
                    logicIndex++;
                } else {
                    currentState = SimState::LoadingFromSort;
                    logicIndex = 0;
                }
                break;
            }

            case SimState::LoadingFromSort: {
                int heaviest_idx = -1;
                int max_weight = -1;
                for (size_t i = 0; i < visualSortingYard.size(); ++i) {
                    if (!visualSortingYard[i].empty()) {
                        if (visualSortingYard[i].back()->getWeight() > max_weight) {
                            max_weight = visualSortingYard[i].back()->getWeight();
                            heaviest_idx = i;
                        }
                    }
                }
                if (heaviest_idx != -1) {
                    if (visualTrucks.back().type != TruckType::Standard) {
                         visualTrucks.emplace_back(gui::Point(1050, 400), TruckType::Standard);
                    }
                    const Container* c = visualSortingYard[heaviest_idx].back();
                    place_on_truck(visualTrucks.back(), *c);
                    visualSortingYard[heaviest_idx].pop_back();
                } else {
                    currentState = SimState::Done;
                }
                break;
            }

            case SimState::Idle:
            case SimState::Done:
                timer.stop();
                break;
        }
    }

    bool onTimer(gui::Timer* pTimer) override {
        if (pTimer == &timer) {
            step();
            canvas.refresh();
            return true;
        }
        return false;
    }

    bool onClick(gui::Button* pBtn) override {
        if (pBtn == &btnStart) {
            if (timer.isRunning()) {
                timer.stop();
            } else {
                timer.start();
            }
            return true;
        }
        if (pBtn == &btnStep) {
            step();
            canvas.refresh();
            return true;
        }
        if (pBtn == &btnReset) {
            resetSimulation();
            return true;
        }
        return false;
    }
};
