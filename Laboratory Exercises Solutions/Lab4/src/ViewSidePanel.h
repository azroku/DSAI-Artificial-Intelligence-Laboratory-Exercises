#pragma once
#include <gui/View.h>
#include <gui/VerticalLayout.h>
#include <gui/Button.h>
#include <gui/Label.h>
#include <functional>
#include <string> // Use standard string

class ViewSidePanel : public gui::View
{
private:
    gui::VerticalLayout _vLayout;
    gui::Button _btnSolve;
    gui::Label _lblStatsTitle;
    gui::Label _lblExploredNodes;
    gui::Label _lblPathLength;
    std::function<void()> _onSolveClicked;

public:
    ViewSidePanel()
        : _vLayout(5),
        // Initialize with plain, safe C-style strings.
        _btnSolve("Solve with A*"),
        _lblStatsTitle("Statistics:"),
        _lblExploredNodes("Explored Nodes: 0"),
        _lblPathLength("Path Length: 0")
    {
        _vLayout.append(_btnSolve);
        _vLayout.appendSpacer(20);
        _vLayout.append(_lblStatsTitle);
        _vLayout.append(_lblExploredNodes);
        _vLayout.append(_lblPathLength);
        setLayout(&_vLayout);

        _btnSolve.onClick([this]() {
            if (_onSolveClicked) {
                _onSolveClicked();
            }
            });
    }

    void setOnSolveClicked(std::function<void()> callback) {
        _onSolveClicked = std::move(callback);
    }

    void updateStats(int exploredCount, int pathLength) {
        // Use standard C++ strings.
        std::string explored_str = "Explored Nodes: " + std::to_string(exploredCount);
        _lblExploredNodes.setTitle(explored_str.c_str());

        std::string path_str = "Path Length: " + std::to_string(pathLength);
        _lblPathLength.setTitle(path_str.c_str());
    }
};