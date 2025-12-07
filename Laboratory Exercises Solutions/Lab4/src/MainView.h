#pragma once
#include <gui/View.h>
#include <gui/SplitterLayout.h>
#include "ViewPuzzle.h"
#include "ViewSidePanel.h"
#include "AStarSearch.h"
#include "Heuristics.h"

class MainView : public gui::View
{
private:
    gui::SplitterLayout _splitter;
    ViewPuzzle _puzzleView;
    ViewSidePanel _sidePanel;

public:
    MainView()
        : _splitter(gui::SplitterLayout::Orientation::Horizontal, gui::SplitterLayout::AuxiliaryCell::Second)
    {
        _splitter.setContent(_puzzleView, _sidePanel);
        setLayout(&_splitter);

        _sidePanel.setOnSolveClicked([this]() {
            const PuzzleState startState({ {1, 2, 3}, {0, 4, 6}, {7, 5, 8} });
            const PuzzleState goalState({ {1, 2, 3}, {4, 5, 6}, {7, 8, 0} });

            std::vector<PuzzleState> path = aStarSearch(startState, goalState, manhattanDistance);

            if (!path.empty()) {
                // Number of explored nodes is not yet tracked, so pass 0 for now.
                _sidePanel.updateStats(0, path.size() - 1);

                // Instantly show the final state.
                _puzzleView.setState(path.back());
            }
            else {
                _sidePanel.updateStats(0, 0);
            }
            });
    }
};