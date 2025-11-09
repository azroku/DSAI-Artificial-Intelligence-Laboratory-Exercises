#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include "SimulationCanvas.h"
    // ... other includes

    enum class SimState { Idle, UnloadingToxic, /* ... */, Done };

    class MainView : public gui::View {
    protected:
        Ship ship;
        Port port;
        std::vector<VisualContainer> visualContainers;
        std::vector<VisualTruck> visualTrucks;
        SimulationCanvas canvas;
        gui::Button btnStart, btnStep, btnReset;
        SimState currentState = SimState::Idle;
        SimulationCanvas canvas;
        gui::Button btnStart;

    public:
        MainView() {
            void resetSimulation();
            void step();
            bool onClick(gui::Button * pBtn) override;
        }

        bool onClick(gui::Button* pBtn) override {
            if (pBtn == &btnStart) {
                if (canvas.isAnimating()) {
                    canvas.stopAnimation();
                }
                else {
                    canvas.startAnimation();
                }
                return true;
            }
            return false;
        }