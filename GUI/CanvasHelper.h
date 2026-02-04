#ifndef CANVASHELPER_H
#define CANVASHELPER_H

#include <stack>
#include <functional>

#include "../ShapeManager.h"
#include "../MyPoint.h"
#include "../DisableCopyMove.h"

class CanvasPrivate;
class QGraphicsView;
class MyShape;
class GUIHandler;
class CanvasHelper: public ShapeManager {
    public:
        DISABLE_COPY_MOVE(CanvasHelper)
        CanvasHelper(QGraphicsView* drawingPane);
        ~CanvasHelper();

        void addShape(std::shared_ptr<MyShape> shape);
        void removeShape(int index);
        void removeAllShapes();
        void undoShape();
        void redoShape();

        void AutoCreateRectangleFromPoints(MyPoint p1, MyPoint p2);
        void AutoCreateSquareFromPoints(MyPoint p1, MyPoint p2);
        void AutoCreateCircleFromPoints(MyPoint p1, MyPoint p2);
        void AutoCreateTriangleFromPoints(MyPoint p1, MyPoint p2, MyPoint p3);
        void AutoCreateLineFromPoints(MyPoint p1, MyPoint p2);

        void clearCanvas(QGraphicsView* view);
        void setGUIHandler(GUIHandler* handler);

        void addCircle();
        void addLine();
        void addTriangle();
        void addSquare();
        void addRectangle();

        void updatePropertyPannel();

    private:
        QGraphicsView* drawingPane;
        std::function<void()> updateCallback;
        std::unique_ptr<CanvasPrivate> canvasPrivate;
        GUIHandler* guihandler;
};

#endif // CANVASHELPER_H