#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <vector>
#include <string>
#include <memory>
#include <QGraphicsScene>

class MyPoint;
class QGraphicsEllipseItem;
class CanvasHelper;
class QLabel;
class QFrame;
class MouseHandlerPrivate;

class MouseHandler: public QGraphicsScene{
public:
    MouseHandler(CanvasHelper* canvas, QLabel* coordinatePreview, QGraphicsView* drawingPane);
    ~MouseHandler();

    void setShapeType(std::string shapeType);
    void cancelCurrentDrawing() const;
    std::string* getCurrentShapeType() const;
    double getMouseX() const;
    double getMouseY() const;
    void handler(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    std::string* shapePtr;
    CanvasHelper* canvas;
    QLabel* coordinatePreview;
    QGraphicsView* drawingPane;
    double currentX;
    double currentY;
    MouseHandlerPrivate* mouseHandlerPrivate;
};

#endif // MOUSEHANDLER_H