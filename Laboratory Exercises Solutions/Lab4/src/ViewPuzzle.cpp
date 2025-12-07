#include "ViewPuzzle.h"
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include <gui/Font.h>

ViewPuzzle::ViewPuzzle()
    : _puzzleState({ {1, 2, 3}, {4, 5, 6}, {7, 0, 8} })
{
    // 1. Crucial step: Enable the resize event for this canvas
    enableResizeEvent(true);
}

void ViewPuzzle::setState(const PuzzleState& newState)
{
    _puzzleState = newState;
    reDraw();
}

// 2. Implement the onResize method
void ViewPuzzle::onResize(const gui::Size& newSize)
{
    // Store the new dimensions in our member variables
    _canvasWidth = newSize.width;
    _canvasHeight = newSize.height;
    // It's good practice to redraw after a resize
    reDraw();
}

void ViewPuzzle::onDraw(const gui::Rect& rect)
{
    gui::Shape shape;
    gui::DrawableString drawableString;

    const int tileWidth = _canvasWidth / 3;
    const int tileHeight = _canvasHeight / 3;

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int tileValue = _puzzleState.board[r][c];

            gui::Rect tileRect(c * tileWidth, r * tileHeight, (c + 1) * tileWidth, (r + 1) * tileHeight);
            shape.createRect(tileRect);

            if (tileValue == 0)
            {
                shape.drawFillAndWire(td::ColorID::Gray, td::ColorID::Black);
            }
            else
            {
                shape.drawFillAndWire(td::ColorID::LightSteelBlue, td::ColorID::Black);

                // 1. Create a standard C++ string from the integer
                std::string num_std_str = std::to_string(tileValue);

                // 2. Assign it to the DrawableString using the '=' operator
                drawableString = num_std_str.c_str();

                // 3. Measure the text size using the correct 'measure' method
                gui::Size textSize;
                drawableString.measure(gui::Font::ID::SystemNormal, textSize);

                // 4. Calculate the position using member access (.width, .height)
                gui::Point textPos(
                    tileRect.left + (tileWidth - textSize.width) / 2,
                    tileRect.top + (tileHeight - textSize.height) / 2
                );

                // 5. Draw the text (this call was already correct)
                drawableString.draw(textPos, gui::Font::ID::SystemNormal, td::ColorID::Black);
            }
        }
    }
}