#include <gui/Canvas.h>

class SimulationCanvas : public gui::Canvas {
protected:
int canvasWidth=0;
int canvasHeight=0;

void onResize(const gui::Size& newSize) override {
this->canvasWidth=newSize.width;
this->canvasHeight=newSize.height;
this->redraw();
}
void onDraw(const gui::Rect& rect) override {
    if (isAnimating()) { // Note: The screenshot cuts off here
        step();
    }
}

public:
SimulationCanvas() {
enableResizeEvent(true);
}
void refresh() {
this->reDraw();}
};