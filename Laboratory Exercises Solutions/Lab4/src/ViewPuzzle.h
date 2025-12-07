#pragma once
#include <gui/Canvas.h>
#include "PuzzleState.h"

class ViewPuzzle : public gui::Canvas
{
private:
    PuzzleState _puzzleState;

    // Member variables to store the canvas size
    int _canvasWidth = 0;
    int _canvasHeight = 0;

protected:
    // This method will be called when the canvas is resized
    void onResize(const gui::Size& newSize) override;

    void onDraw(const gui::Rect& rect) override;

public:
    ViewPuzzle();
    void setState(const PuzzleState& newState);
};