#include "MouseHandler.h"
#include "../MyShape.h"
#include "CanvasHelper.h"
#include "../MyCircle.h"
#include "../MyTriangle.h"
#include "../MyLine.h"
#include "../MySquare.h"
#include "../MyRectangle.h"
#include "../MyPoint.h"
#include "../Utility.hpp"
#include "../InvalidCoordinateException.h"
#include "../Multithread.hpp"

#include <vector>
#include <QLabel>
#include <QFrame>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>
#include <cmath>
#include <memory>
#include <exception>

class MouseHandlerPrivate{
    public:
        std::string currentShape;
        QGraphicsScene* scene;
        QGraphicsView* canvasView;
        QFrame* Frame;
        QLabel* coordinatePreview;
        CanvasHelper* canvas;
        bool customDrawing = false;
        std::shared_ptr<std::vector<MyPoint>> clickedPoints;
        std::vector<QGraphicsEllipseItem*> visualPoints;

        MouseHandlerPrivate(CanvasHelper* canvasPtr) : canvas(canvasPtr) {
        };

        ~MouseHandlerPrivate() = default;

        void handleClick(double x, double y){
            if (currentShape.empty()){
                try{
                    std::shared_ptr<MyPoint> point = std::make_shared<MyPoint>(x, y);
                    clickedPoints->push_back(*point);
                    customDrawing = true;
                } catch (const InvalidCoordinateException& e){
                    showErrorAlert("Invalid Coordinate", e.what());
                }
                return;
            }
            
            try{
                std::shared_ptr<MyPoint> point =  std::make_shared<MyPoint>(x, y);
                clickedPoints->push_back(*point);

                QGraphicsEllipseItem* dot = new QGraphicsEllipseItem(x-3, y-3, 6, 6);
                QPen drawPen = QPen(Qt::white);
                dot->setBrush(QBrush(Qt::black));
                dot->setPen(drawPen);
                dot->pen().setWidth(1);
                visualPoints.push_back(dot);
                scene->addItem(dot);

                if (clickedPoints->size() >= getRequiredClicks()){
                    createShape();
                    resetDrawing();
                }
            } catch (const InvalidCoordinateException& e){
                showErrorAlert("Invalid Coordinate", e.what());
                resetDrawing();
            }
        };

        // Free drawing mode
        void handleDrag(double x, double y){
            try{
                // only add point when it is far enough from the line
                if (clickedPoints->empty()){
                std::shared_ptr<MyPoint> point =  std::make_shared<MyPoint>(x, y);
                clickedPoints->push_back(*point);
                } else{
                    std::shared_ptr<MyPoint> lastPoint = std::make_shared<MyPoint>(clickedPoints->back());
                    double distance = std::sqrt(std::pow(x - lastPoint->getX(), 2) + std::pow(y - lastPoint->getY(), 2));

                    // Only add point if distance is greater than 100 pixels threshold
                    double threshold = 100.0; // pixels
                    if (distance >= threshold){
                        std::shared_ptr<MyPoint> point =  std::make_shared<MyPoint>(x, y);
                        clickedPoints->push_back(*point);

                        QGraphicsEllipseItem* dot = new QGraphicsEllipseItem(x-1, y-1, 2, 2);
                        dot->setBrush(QBrush(Qt::black));
                        visualPoints.push_back(dot);
                        scene->addItem(dot);
                    }
                }
            } catch (const InvalidCoordinateException& e){
                // Intentionally Skip invalid coordinate during drag
            }
        };

        void finalizeCustomDrawing(){
            if (clickedPoints->size() <2){
                resetDrawing();
                customDrawing = false;
                return;
            }

            try {
                double totalLength = 0.0;
                
                // Create all line segments
                for (size_t i = 0; i < clickedPoints->size() - 1; i++){
                    MyPoint *p1 = &clickedPoints->at(i);
                    MyPoint *p2 = &clickedPoints->at(i + 1);
                    auto line = std::make_shared<MyLine> (p1,p2);
                    
                    // Create line in canvas
                    canvas->addShape(line);
                    
                    // Calculate segment length
                    totalLength += std::sqrt(
                        std::pow(p2->getX() - p1->getX(), 2) + 
                        std::pow(p2->getY() - p1->getY(), 2)
                    );
                }

                std::ostringstream oss;
                oss.precision(2);
                oss << std::fixed << "Custom Drawing Complete | Total Length= " << totalLength
                    << " | Segments: " << (clickedPoints->size()-1);
                coordinatePreview->setText(QString::fromStdString(oss.str()));

            } catch (const std::exception& e){
                showErrorAlert("Error Creating Custom Drawing", e.what());
            }
            
            // reset for next drawing
            resetDrawing();
            customDrawing = false;
        };

        void createShape() const{
            try{
                if (currentShape == "LINE"){
                    canvas->AutoCreateLineFromPoints(clickedPoints->at(0), clickedPoints->at(1));
                } else if (currentShape == "RECTANGLE"){
                    canvas->AutoCreateRectangleFromPoints(clickedPoints->at(0), clickedPoints->at(1));
                } else if (currentShape == "SQUARE"){
                    canvas->AutoCreateSquareFromPoints(clickedPoints->at(0), clickedPoints->at(1));
                } else if (currentShape == "CIRCLE"){
                    canvas->AutoCreateCircleFromPoints(clickedPoints->at(0), clickedPoints->at(1));
                } else if (currentShape == "TRIANGLE"){
                    canvas->AutoCreateTriangleFromPoints(clickedPoints->at(0), clickedPoints->at(1), clickedPoints->at(2));
                }
            }catch (std::exception& e){
                showErrorAlert("Shape Creation Error", e.what(), e);
            }
        };

        int getRequiredClicks() const{
            if (customDrawing){
                return 0; // custom drawing mode
            } else {
                if (currentShape == "LINE" || currentShape == "RECTANGLE" || currentShape == "SQUARE" || currentShape == "CIRCLE") {
                return 2;
                } else if (currentShape == "TRIANGLE") {
                return 3;
                }
                showErrorAlert("Invalid Shape Type", "The shape type is not recognized.");
                return 0;
            }
        };

        bool isInsideCircle(double x, double y, const MyCircle& circle) const{
            double cx = circle.getCenter()->getX();
            double cy = circle.getCenter()->getY();
            double radius = circle.getRadius();
            double distance = std::sqrt(std::pow(x-cx, 2) + std::pow(y-cy, 2));
            return distance <= radius;
        };

        bool isInsideRectangle(double x, double y, const MyRectangle& rectangle) const{
            double minX = findMin(rectangle.getTopLeft()->getX(), rectangle.getBottomRight()->getX());
            double maxX = findMax(rectangle.getTopLeft()->getX(), rectangle.getBottomRight()->getX());
            double minY = findMin(rectangle.getTopLeft()->getY(), rectangle.getBottomRight()->getY());
            double maxY = findMax(rectangle.getTopLeft()->getY(), rectangle.getBottomRight()->getY());
            return x >= minX && x <= maxX && y >= minY && y <= maxY;
        };
        
        // Square and rectangle have same logic for point inclusion
        bool isInsideSquare(double x, double y, const MySquare& square) const{
            double minX = findMin(square.getTopLeft()->getX(), square.getBottomRight()->getX());
            double maxX = findMax(square.getTopLeft()->getX(), square.getBottomRight()->getX());
            double minY = findMin(square.getTopLeft()->getY(), square.getBottomRight()->getY());
            double maxY = findMax(square.getTopLeft()->getY(), square.getBottomRight()->getY());
            return x >= minX && x <= maxX && y >= minY && y <= maxY;
        };

        bool isInsideTriangle(double x, double y, const MyTriangle& triangle) const{
            double x1 = triangle.getPoint1()->getX();
            double y1 = triangle.getPoint1()->getY();
            double x2 = triangle.getPoint2()->getX();
            double y2 = triangle.getPoint2()->getY();
            double x3 = triangle.getPoint3()->getX();
            double y3 = triangle.getPoint3()->getY();

            // check if the point is inside a triangle using barycentric technique
            double demoninator = ((y2-y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
            double a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / demoninator;
            double b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / demoninator;
            double c = 1 - a - b;

            return a>=0 && a<=1 && b>=0 && b<=1 && c>=0 && c<=1;

        };

        bool isNearLine(double x, double y, const MyLine& line) const{
            double x1 = line.getStartPoint()->getX();
            double y1 = line.getStartPoint()->getY();
            double x2 = line.getEndPoint()->getX();
            double y2 = line.getEndPoint()->getY();
            // calculate distance from point to line segment
            double lineLength = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
            double distance = std::abs((y2 - y1)*x - (x2 - x1)*y + x2*y1 - y2*x1) / lineLength;

            // Check point if it is near a line
            double threshold = 5.0; // pixels
            double minX = findMin(x1, x2) - threshold;
            double maxX = findMax(x1, x2) + threshold;
            double minY = findMin(y1, y2) - threshold;
            double maxY = findMax(y1, y2) + threshold;
            return distance < threshold && x >= minX && x <= maxX && y >= minY && y <= maxY;
        };

        static double findMin(double x, double y){
            return std::min(x, y);
        }
        static double findMax(double x, double y){
            return std::max(x, y);
        }

        std::string* getShapePtr() {
            return &currentShape;
        }

        std::string getCurrentShape() const {
            return currentShape;
        }

        void setCurrentShape(const std::string& shape){
            currentShape = shape;
        }

        std::string* getShapePtr(const std::string& shape){
            currentShape = shape;
            return &currentShape;
        }

        void resetDrawing() {
            clickedPoints->clear();

            // remove visual points from scene
            for (auto& dot: visualPoints){
                scene->removeItem(dot);
                delete dot;
            }
            visualPoints.clear();
        };
};

MouseHandler::MouseHandler(CanvasHelper* canvas, QLabel* coordinatePreview, QGraphicsView* drawingPane)
    : QGraphicsScene(), mouseHandlerPrivate(new MouseHandlerPrivate(canvas)){
    this->canvas = canvas;
    this->coordinatePreview = coordinatePreview;
    this->drawingPane = drawingPane;
    this->currentX = 0.0;
    this->currentY = 0.0;
    this->shapePtr = mouseHandlerPrivate->getShapePtr();

    mouseHandlerPrivate->scene =this;
    mouseHandlerPrivate->canvasView = drawingPane;
    mouseHandlerPrivate->coordinatePreview = coordinatePreview;
    mouseHandlerPrivate->clickedPoints=std::make_shared<std::vector<MyPoint>>();
}

MouseHandler::~MouseHandler(){
    delete mouseHandlerPrivate;
}

double MouseHandler::getMouseX() const{
    return currentX;
}

double MouseHandler::getMouseY() const{
    return currentY;
}

std::string* MouseHandler::getCurrentShapeType() const{
    return shapePtr;
}

void MouseHandler::cancelCurrentDrawing() const{
    mouseHandlerPrivate->resetDrawing();
    if (shapePtr) {
        shapePtr->clear();
    }
}

void MouseHandler::setShapeType(std::string shapeType) {
    try {
        shapeType = toUpperCase(shapeType);
        if (shapeType != "LINE" && shapeType != "RECTANGLE" && shapeType != "SQUARE" &&
           shapeType != "CIRCLE" && shapeType != "TRIANGLE" && !shapeType.empty()) {
           showErrorAlert("Error", "Unsupported Shape Type");
           return;
       }
       mouseHandlerPrivate->setCurrentShape(shapeType);
       this->shapePtr = mouseHandlerPrivate->getShapePtr();
    }
    catch (const std::exception& e) {
        showErrorAlert("Error", e.what());
    }
}

void MouseHandler::handler(QGraphicsSceneMouseEvent* event){
    // Store current mouse coordinates
    QPointF point = event->scenePos();
    currentX = point.x();
    currentY = point.y();

    if (event->type() == QEvent::GraphicsSceneMouseMove){
        // Handle dragging if mouse button is pressed
        if (event->buttons() & Qt::LeftButton) {
            mouseHandlerPrivate->handleDrag(currentX, currentY);
        }
        
        // update coordinate as mouse moves
        std::ostringstream coordinate;
        coordinate.precision(1);
        coordinate << "X: " << std::fixed << currentX << ", Y: " << currentY;

        // Show free drawing progress if in custom drawing mode
        if (mouseHandlerPrivate->customDrawing && mouseHandlerPrivate->currentShape.empty()) {
            coordinate << " | Free Drawing: " << mouseHandlerPrivate->clickedPoints->size() << " points";
        } else if (getCurrentShapeType() != nullptr && !mouseHandlerPrivate->clickedPoints->empty()){
            coordinate << " | " << *getCurrentShapeType() << ": Point " 
                      << mouseHandlerPrivate->clickedPoints->size() << "/"
                      << mouseHandlerPrivate->getRequiredClicks();
        }
        coordinatePreview->setText(QString::fromStdString(coordinate.str()));
    
        if (!canvas->getAllShapes().empty()){
            // Vector array start from 0, last value is at size-1
            for (int i = canvas->getAllShapes().size()-1; i>= 0; i--){
                std::shared_ptr<MyShape> currentShape = canvas->getShape(i);
                QGraphicsItem* item = currentShape->toQShape();
                const bool circle = dynamic_cast<MyCircle*>(currentShape.get())? true: false;
                const bool triangle = dynamic_cast<MyTriangle*>(currentShape.get())? true: false;
                const bool square = dynamic_cast<MySquare*>(currentShape.get())? true: false;
                const bool rectangle = dynamic_cast<MyRectangle*>(currentShape.get())? true: false;
                const bool line = dynamic_cast<MyLine*>(currentShape.get())? true: false;

                if (circle && item->type() == QGraphicsEllipseItem::Type){
                    MyCircle* circle = dynamic_cast<MyCircle*>(currentShape.get());
                    if (mouseHandlerPrivate->isInsideCircle(currentX, currentY, *circle)){
                        std::ostringstream info;
                        info.precision(2);
                        info << std::fixed
                             << " | Center Point= " << circle->getCenter()->getX() << "," << circle->getCenter()->getY()
                             << " | Radius= " << circle->getRadius() 
                             << " | Area= " << circle->calculateArea() 
                             << " | Perimeter= " << circle->calculatePerimeter();
                        coordinatePreview->setText(QString::fromStdString(info.str()));
                        break;
                    }
                }
                else if (rectangle && item->type() == QGraphicsRectItem::Type){
                    MyRectangle* rectangle = dynamic_cast<MyRectangle*>(currentShape.get());
                    if (mouseHandlerPrivate->isInsideRectangle(currentX, currentY, *rectangle)){
                        std::ostringstream info;
                        info.precision(2);
                        info << std::fixed
                             << " | Width= " << rectangle->getWidth()
                             << " | Height= " << rectangle->getHeight() 
                             << " | Area= " << rectangle->calculateArea() 
                             << " | Perimeter= " << rectangle->calculatePerimeter();
                        coordinatePreview->setText(QString::fromStdString(info.str()));
                        break;
                    }
                }
                else if (square && item->type() == QGraphicsRectItem::Type){
                    MySquare* square = dynamic_cast<MySquare*>(currentShape.get());
                    if (mouseHandlerPrivate->isInsideSquare(currentX, currentY, *square)){
                        std::ostringstream info;
                        info.precision(2);
                        info << std::fixed
                             << " | Side Length= " << square->getSide()
                             << " | Area= " << square->calculateArea() 
                             << " | Perimeter= " << square->calculatePerimeter();
                        coordinatePreview->setText(QString::fromStdString(info.str()));
                        break;
                    }
                }
                else if (triangle && item->type() == QGraphicsPolygonItem::Type){
                    MyTriangle* triangle = dynamic_cast<MyTriangle*>(currentShape.get());
                    if (mouseHandlerPrivate->isInsideTriangle(currentX, currentY, *triangle)){
                        std::ostringstream info;
                        info.precision(2);
                        info << std::fixed << "Point1= " << triangle->getPoint1()->getX() << ","<< triangle->getPoint1()->getY() << ", Point2= " << triangle->getPoint2()->getX() << "," << triangle->getPoint2()->getY() << ", Point3= " << triangle->getPoint3()->getX() << ","<< triangle->getPoint3()->getY()
                             << " | Side 1= " << triangle->getSide1() << ", Side 2= " << triangle->getSide2() << ", Side 3=" << triangle->getSide3()
                             << " | Area= " << triangle->calculateArea() 
                             << " | Perimeter= " << triangle->calculatePerimeter();
                        coordinatePreview->setText(QString::fromStdString(info.str()));
                        break;
                    }
                }
                else if (line && item->type() == QGraphicsLineItem::Type){
                    MyLine* line = dynamic_cast<MyLine*>(currentShape.get());
                    if (mouseHandlerPrivate->isNearLine(currentX, currentY, *line)){
                        std::ostringstream info;
                        info.precision(2);
                        info << std::fixed << "Start Point X and Y= " << line->getStartPoint()->getX() << ","<< line->getStartPoint()->getY() << ", End Point X and Y= " << line->getEndPoint()->getX() << ","<< line->getEndPoint()->getY()
                             << " | Length= " << line->calculateLength();
                        coordinatePreview->setText(QString::fromStdString(info.str()));
                        break;
                    }
                }
            }
        }
    } else if (event->type() == QEvent::GraphicsSceneMousePress){
        // When mouse is clicked
        mouseHandlerPrivate->handleClick(currentX, currentY);
    } else if (event->type() == QEvent::GraphicsSceneMouseRelease){
        // finalize custom drawing on mouse release
        if (mouseHandlerPrivate->customDrawing && mouseHandlerPrivate->currentShape.empty()){
            mouseHandlerPrivate->finalizeCustomDrawing();
        }
    }
}

void MouseHandler::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    QPointF point = event->scenePos();

    //check if mouse is within scene bounds
    if (sceneRect().contains(point)){handler(event);}
    else{
        coordinatePreview->setText("X: --, Y: --");
        handler(event);
    }
}

void MouseHandler::mousePressEvent(QGraphicsSceneMouseEvent* event){
    handler(event);
}

void MouseHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
    handler(event);
}