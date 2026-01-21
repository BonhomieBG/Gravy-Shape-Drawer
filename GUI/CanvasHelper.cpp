#include "CanvasHelper.h"
#include "../MyShape.h"
#include "../Utility.hpp"
#include "../ShapeManager.h"
#include "../Multithread.hpp"
#include "../MyLine.h"
#include "../MyRectangle.h"
#include "../MyCircle.h"
#include "../MySquare.h"
#include "../MyTriangle.h"
#include "GUIHandler.h"
#include "../InvalidCoordinateException.h"

#include <stdexcept>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QInputDialog>
#include <QString>
#include <memory>

// Private class to encapsulate CanvasHelper's internal data (PIMPL, Pointer to Implementation idiom)
// Outside class can't see or access this class 
class CanvasPrivate{
    public:
        std::stack<std::vector<std::shared_ptr<MyShape>>> stack;

        CanvasPrivate() = default;
        ~CanvasPrivate() = default;

        bool isSameShape(QGraphicsItem* qitem, std::shared_ptr<MyShape> shape){
            if (auto line = dynamic_cast<QGraphicsLineItem*>(qitem)){
                if (MyLine* myline = dynamic_cast<MyLine*>(shape.get())){
                    return std::abs(line->line().x1() - myline->getStartPoint()->getX()) < 0.1 &&
                        std::abs(line->line().y1() - myline->getStartPoint()->getY()) < 0.1 &&
                        std::abs(line->line().x2() - myline->getEndPoint()->getX()) < 0.1 &&
                        std::abs(line->line().y2() - myline->getEndPoint()->getY()) < 0.1;
                } else if (auto rectangle = dynamic_cast<QGraphicsRectItem*>(qitem)){
                if (MyRectangle* rec = dynamic_cast<MyRectangle*>(shape.get())){
                    return std::abs(rectangle->rect().x() - rec->getTopLeft()->getX()) < 0.1 &&
                        std::abs(rectangle->rect().y() - rec->getTopLeft()->getY()) < 0.1 &&
                        std::abs(rectangle->rect().width() - rec->getWidth()) < 0.1 &&
                        std::abs(rectangle->rect().height() - rec->getHeight()) < 0.1;
                    }
                } else if (auto circle = dynamic_cast<QGraphicsEllipseItem*>(qitem)){
                if (MyCircle* cir = dynamic_cast<MyCircle*>(shape.get())){
                    return std::abs(circle->rect().x() - cir->getCenter()->getX() + cir->getRadius()) < 0.1 &&
                        std::abs(circle->rect().y() - cir->getCenter()->getY() + cir->getRadius()) < 0.1 &&
                        std::abs(circle->rect().width() - cir->getRadius()*2) < 0.1 &&
                        std::abs(circle->rect().height() - cir->getRadius()*2) < 0.1;
                    }
                } else if (auto square = dynamic_cast<QGraphicsRectItem*>(qitem)){
                if (MySquare* sq = dynamic_cast<MySquare*>(shape.get())){
                    return std::abs(square->rect().x() - sq->getTopLeft()->getX()) < 0.1 &&
                    std::abs(square->rect().y() - sq->getTopLeft()->getY()) < 0.1 &&
                    std::abs(square->rect().width() - sq->getSide()) < 0.1 &&
                    std::abs(square->rect().height() - sq->getSide()) < 0.1;
                    }
                } else if (auto triangle = dynamic_cast<QGraphicsPolygonItem*>(qitem)){
                if (MyTriangle* tri = dynamic_cast<MyTriangle*>(shape.get())){
                    QPointF p1 = triangle->polygon().at(0);
                    QPointF p2 = triangle->polygon().at(1);
                    QPointF p3 = triangle->polygon().at(2);
                    return std::abs(p1.x() - tri->getPoint1()->getX()) < 0.1 &&
                        std::abs(p1.y() - tri->getPoint1()->getY()) < 0.1 &&
                        std::abs(p2.x() - tri->getPoint2()->getX()) < 0.1 &&
                        std::abs(p2.y() - tri->getPoint2()->getY()) < 0.1 &&
                        std::abs(p3.x() - tri->getPoint3()->getX()) < 0.1 &&
                        std::abs(p3.y() - tri->getPoint3()->getY()) < 0.1;
                    }
                } else{
                    return false;
                }
            }
        }

        std::shared_ptr<MyPoint> captureMouse(double x, double y){
            try{
                std::shared_ptr<MyPoint> point = std::make_shared<MyPoint>(x, y);
                return point;
            } catch (InvalidCoordinateException e){
                showErrorAlert("Coordinate Error", e.what(), e);
                return nullptr;
            }
        }

        void pushStack(std::vector<std::shared_ptr<MyShape>> currentShapes){
        if (currentShapes.empty()){
            showErrorAlert("Error", "There is no shape to save.");
            return;
        }
            stack.push(currentShapes);
        }

        void clearStack(){
            while (!stack.empty()){
            stack.pop();
            }
        }

        std::vector<std::shared_ptr<MyShape>> popStack(){ // 1 shape only
        try{
            if (stack.empty()){
                showErrorAlert("Error", "No saved shape to restore.");
            }
            auto shapestoRestore = stack.top();
            stack.pop();
            return shapestoRestore;
        } catch (std::exception& e){
            showErrorAlert("Error", e.what(), e);
            return {};
        }
        }
};

CanvasHelper::~CanvasHelper() = default;

CanvasHelper::CanvasHelper(QGraphicsView* drawingPane) : ShapeManager() {
    this->drawingPane = drawingPane;
    this->canvasPrivate = std::make_unique<CanvasPrivate>();
    this->guihandler = nullptr;  // Set in GUIHandler constructor
}

void CanvasHelper::setGUIHandler(GUIHandler* handler){
    guihandler = handler;
}

void CanvasHelper::addShape(std::shared_ptr<MyShape> shape) {
    if (shape == nullptr) {
        showErrorAlert("Error Adding Shape", "No Shape found");
        return;
    }
    try{
        ShapeManager::addShape(shape);
        QGraphicsItem* qshape = shape->toQShape();
        if (qshape == nullptr) {
            showErrorAlert("Error Adding Shape", "Failed to convert shape to QT item.");
            return;
        }
        drawingPane->scene()->addItem(qshape);
        if (updateCallback) {
            updateCallback();
        }
    } catch (std::exception& e){
        showErrorAlert("Error Adding Shape", e.what(), e);
    }
}

void CanvasHelper::removeShape(int index) {
    if (index >= 0 && index < static_cast<int>(ShapeManager::getAllShapes().size())) {
        auto removedShape = ShapeManager::getShape(index);

        if (removedShape) {
            // Get the graphics item and remove from scene
            QGraphicsItem* qitem = removedShape->toQShape();
            if (qitem && drawingPane->scene()) {
                drawingPane->scene()->removeItem(qitem);
                delete qitem;
            }
            // Remove from ShapeManager
            ShapeManager::removeShape(index);
            
            // Push to stack for undo
            canvasPrivate->pushStack({removedShape});
            
            // Call update callback
            if (updateCallback) {
                updateCallback();
            }
        }
    } else {
        showErrorAlert("Error Removing Shape", "Index out of bounds.");
    }
}

// Use remove all shape for conformation dialog
void CanvasHelper::clearCanvas(QGraphicsView* view){
    if (view->scene()){
        view->scene()->clear();
        ShapeManager::removeAllShapes();
        canvasPrivate->clearStack();
    }
}

void CanvasHelper::removeAllShapes(){
    try{
    std::string Yes, No;
    bool result;
    showWarningCustom("Clear Canvas?", "Are you sure you want to clear the canvas? This action cannot be undone.", Yes, No, result);
    if (result){
        clearCanvas(drawingPane);
    }
    } catch (std::exception& e) {
        showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::undoShape(){
try {
    int lastShapeIndex = static_cast<int>(ShapeManager::getAllShapes().size()) - 1;
    if (lastShapeIndex <= 0){
        showErrorAlert("Undo Error", "No shape to undo.");
        return;
    }
    std::shared_ptr <MyShape> removedShape = ShapeManager::getShape(lastShapeIndex);
    canvasPrivate->pushStack({removedShape});
    QGraphicsItem* qitem = removedShape->toQShape();
        if (qitem && drawingPane->scene()) {
            drawingPane->scene()->removeItem(qitem);
            delete qitem;
        }
    ShapeManager::removeShape(lastShapeIndex);
    if (updateCallback) {
        updateCallback();
    }
} catch (std::exception& e){
    showErrorAlert("Error", e.what(), e);
}
}

void CanvasHelper::redoShape(){
    try{
        if (canvasPrivate->stack.empty()){
            showErrorAlert("Redo Error", "No shape to redo.");
        }
        if (!canvasPrivate->stack.empty()){
            auto shapetoRestore = canvasPrivate->popStack(); // restore only 1 shape
            ShapeManager::addShape (shapetoRestore[0]);
            QGraphicsItem* qshape = shapetoRestore[0]->toQShape();
            if (qshape == nullptr) {
                showErrorAlert("Error Redoing Shape", "Failed to convert shape to QT item.");
                return;
            }
            drawingPane->scene()->addItem(qshape);
            if (updateCallback) {
                updateCallback();
            }
        }
    } catch (std::exception& e){
        showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::AutoCreateLineFromPoints(MyPoint p1, MyPoint p2){
    try{
        auto ptr1 = std::make_shared<MyPoint>(p1);
        auto ptr2 = std::make_shared<MyPoint>(p2);
        auto line = std::make_shared<MyLine>(ptr1.get(),ptr2.get());

        if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && (guihandler->gridLine<0)){
            guihandler->gridLine = 1.0;
        } else if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && guihandler->gridLine>0){
            line->setGridLineWidth(guihandler->gridLine);
        } else {
            guihandler->gridLine = 1.0;
        }

        if (guihandler->isStroked && !guihandler->userStrokeColor.empty()){
            line->setStrokeColor(&guihandler->userStrokeColor);
        }
        addShape(line);
    } catch (InvalidCoordinateException& e){
        showErrorAlert("Error", e.what(), e);
    } catch (std::exception& e){
        showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::AutoCreateCircleFromPoints(MyPoint p1, MyPoint p2){
    try {
        auto ptr1 = std::make_shared<MyPoint>(p1);
        auto ptr2 = std::make_shared<MyPoint>(p2);
        auto circle = std::make_shared<MyCircle>(ptr1.get(), ptr2.get()); 

        if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && (guihandler->gridLine<0)){
            guihandler->gridLine = 1.0;
        } else if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && guihandler->gridLine>0){
            circle->setGridLineWidth(guihandler->gridLine);
        } else {
            guihandler->gridLine = 1.0;
        }

        if (guihandler->isStroked && !guihandler->userStrokeColor.empty()){
            circle->setStrokeColor(&guihandler->userStrokeColor);
        }

        if (guihandler->isFilled && !guihandler->userFillColor.empty()){
            circle->setFillColor(&guihandler->userFillColor);
        }
        addShape(circle);
    } catch (InvalidCoordinateException &e){showErrorAlert("Error", e.what(), e);
    } catch (std::exception& e){showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::AutoCreateRectangleFromPoints(MyPoint p1, MyPoint p2){
    try {
        auto ptr1 = std::make_shared<MyPoint>(p1);
        auto ptr2 = std::make_shared<MyPoint>(p2);
        auto rectangle = std::make_shared<MyRectangle>(ptr1.get(), ptr2.get()); 

        if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && (guihandler->gridLine<0)){
            guihandler->gridLine = 1.0;
        } else if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && guihandler->gridLine>0){
            rectangle->setGridLineWidth(guihandler->gridLine);
        } else {
            guihandler->gridLine = 1.0;
        }

        if (guihandler->isStroked && !guihandler->userStrokeColor.empty()){
            rectangle->setStrokeColor(&guihandler->userStrokeColor);
        }

        if (guihandler->isFilled && !guihandler->userFillColor.empty()){
            rectangle->setFillColor(&guihandler->userFillColor);
        }
        addShape(rectangle);
    } catch (InvalidCoordinateException &e){showErrorAlert("Error", e.what(), e);
    } catch (std::exception& e){showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::AutoCreateSquareFromPoints(MyPoint p1, MyPoint p2){
    try {
        auto ptr1 = std::make_shared<MyPoint>(p1);
        auto ptr2 = std::make_shared<MyPoint>(p2);
        auto square = std::make_shared<MySquare>(ptr1.get(), ptr2.get()); 

        if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && (guihandler->gridLine<0)){
            guihandler->gridLine = 1.0;
        } else if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && guihandler->gridLine>0){
            square->setGridLineWidth(guihandler->gridLine);
        } else {
            guihandler->gridLine = 1.0;
        }

        if (guihandler->isStroked && !guihandler->userStrokeColor.empty()){
            square->setStrokeColor(&guihandler->userStrokeColor);
        }

        if (guihandler->isFilled && !guihandler->userFillColor.empty()){
            square->setFillColor(&guihandler->userFillColor);
        }
        addShape(square);
    } catch (InvalidCoordinateException &e){showErrorAlert("Error", e.what(), e);
    } catch (std::exception& e){showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::AutoCreateTriangleFromPoints(MyPoint p1, MyPoint p2, MyPoint p3){
    try {
        auto ptr1 = std::make_shared<MyPoint>(p1);
        auto ptr2 = std::make_shared<MyPoint>(p2);
        auto ptr3 = std::make_shared<MyPoint>(p3);
        auto triangle = std::make_shared<MyTriangle>(ptr1.get(), ptr2.get(), ptr3.get()); 

        if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && (guihandler->gridLine<0)){
            guihandler->gridLine = 1.0;
        } else if (!std::isnan(guihandler->gridLine) && std::isfinite(guihandler->gridLine) && guihandler->gridLine>0){
            triangle->setGridLineWidth(guihandler->gridLine);
        } else {
            guihandler->gridLine = 1.0;
        }

        if (guihandler->isStroked && !guihandler->userStrokeColor.empty()){
            triangle->setStrokeColor(&guihandler->userStrokeColor);
        }

        if (guihandler->isFilled && !guihandler->userFillColor.empty()){
            triangle->setFillColor(&guihandler->userFillColor);
        }
        addShape(triangle);
    } catch (InvalidCoordinateException &e){showErrorAlert("Error", e.what(), e);
    } catch (std::exception& e){showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::addCircle(){
    try{
        bool ok;
        std::string stroke, fill;
        double r, x, y;

        QString X = QInputDialog::getText(nullptr, "Add Circle", "Enter Center Point X: ", QLineEdit::Normal, "0.0", &ok);

        if (!X.isEmpty()){
            x = X.toDouble();
        } else {
            showErrorAlert("Error", "X must be valid"); return;
        }
        
        QString Y = QInputDialog::getText(nullptr, "Add Circle", "Enter Center Point Y: ", QLineEdit::Normal, "0.0", &ok);

        if (!Y.isEmpty()){
            y = Y.toDouble();
        } else {
            showErrorAlert("Error", "Y must be valid"); return;
        }

        QString Radius = QInputDialog::getText(nullptr, "Add Circle", "Enter Radius: ", QLineEdit::Normal, "0.0", &ok);

        if (!Radius.isEmpty()){
            r = Radius.toDouble();
        } else {showErrorAlert("Error", "Radius must be valid"); return;
        }

        QString Stroke = QInputDialog::getText(nullptr, "Add Circle", "Enter Stroke Color: ", QLineEdit::Normal, "default:black", &ok);
        QString Fill = QInputDialog::getText(nullptr, "Add Circle", "Enter Fill Color: ", QLineEdit::Normal, "default:transparent (disabled filling)", &ok);

        auto center = std::make_shared<MyPoint>(x, y);
        auto circle = std::make_shared<MyCircle>(center.get(), r);

        if (!Stroke.isEmpty()){
            circle->ChangeStrokedStatus(true);
            stroke = Stroke.toStdString();
            circle->setStrokeColor(&stroke);
        } else {stroke = "";}
        if (!Fill.isEmpty()){
            circle->ChangeFilledStatus(true);
            fill = Fill.toStdString();
            circle->setFillColor(&fill);
        } else {fill = "";}

        addShape(circle);
        showInfoAlert("Add Circle", "Successfully Added Circle");
    } catch (std::exception& e){
        showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::addLine(){
    try{
        bool ok;
        std::string stroke, fill;
        double x1, y1, x2, y2;

        QString X1 = QInputDialog::getText(nullptr, "Add Line", "Enter Start Point X: ", QLineEdit::Normal, "0.0", &ok);

        if (!X1.isEmpty()){
            x1 = X1.toDouble();
        } else {
            showErrorAlert("Error", "X must be valid"); return;
        }

        QString Y1 = QInputDialog::getText(nullptr, "Add Line", "Enter Start Point Y: ", QLineEdit::Normal, "0.0", &ok);

        if (!Y1.isEmpty()){
            y1 = Y1.toDouble();
        } else {
            showErrorAlert("Error", "Y must be valid"); return;
        }

        QString X2 = QInputDialog::getText(nullptr, "Add Line", "Enter End Point X: ", QLineEdit::Normal, "0.0", &ok);

        if (!X2.isEmpty()){
            x2 = X2.toDouble();
        } else {
            showErrorAlert("Error", "X must be valid"); return;
        }

        QString Y2 = QInputDialog::getText(nullptr, "Add Line", "Enter End Point Y: ", QLineEdit::Normal, "0.0", &ok);
        
        if (!Y2.isEmpty()){
            y2 = Y2.toDouble();
        } else {
            showErrorAlert("Error", "Y must be valid"); return;
        }

        QString Stroke = QInputDialog::getText(nullptr, "Add Line", "Enter Stroke Color: ", QLineEdit::Normal, "default:black", &ok);
        
        auto x = std::make_shared<MyPoint>(x1, y1);
        auto y = std::make_shared<MyPoint>(x2, y2);
        auto line = std::make_shared<MyLine>(x.get(), y.get());

        if (!Stroke.isEmpty()){
            line->ChangeStrokedStatus(true);
            stroke = Stroke.toStdString();
            line->setStrokeColor(&stroke);
        } else {
            stroke = "";
        }

        addShape(line);
        showInfoAlert("Add Circle", "Successfully Added Circle");  
    } catch (std::exception& e){
        showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::addRectangle(){
    try{
        bool ok;
        std::string stroke, fill;
        double width, height, x, y;

        QString X = QInputDialog::getText(nullptr, "Add Rectangle", "Enter Start Point X: ", QLineEdit::Normal, "0.0", &ok);

        if (!X.isEmpty()){
            x = X.toDouble();
        } else {
            showErrorAlert("Error", "X must be valid"); return;
        }
        
        QString Y = QInputDialog::getText(nullptr, "Add Rectangle", "Enter Start Point Y: ", QLineEdit::Normal, "0.0", &ok);

        if (!Y.isEmpty()){
            y = Y.toDouble();
        } else {
            showErrorAlert("Error", "Y must be valid"); return;
        }
            
        QString Width = QInputDialog::getText(nullptr, "Add Rectangle", "Enter Width: ", QLineEdit::Normal, "0.0", &ok);

        if (!Width.isEmpty()){
            width = Width.toDouble();
        } else {showErrorAlert("Error", "Radius must be valid"); return;
        }

        QString Height = QInputDialog::getText(nullptr, "Add Rectangle", "Enter Height: ", QLineEdit::Normal, "0.0", &ok);

        if (!Height.isEmpty()){
            height = Height.toDouble();
        } else {showErrorAlert("Error", "Height must be valid"); return;
        }

        QString Stroke = QInputDialog::getText(nullptr, "Add Rectangle", "Enter Stroke Color: ", QLineEdit::Normal, "default:black", &ok);
        QString Fill = QInputDialog::getText(nullptr, "Add Rectangle", "Enter Fill Color: ", QLineEdit::Normal, "default:transparent (disabled filling)", &ok);

        auto startpoint = std::make_shared<MyPoint>(x, y);
        auto rectangle = std::make_shared<MyRectangle>(startpoint.get(), width, height);

        if (!Stroke.isEmpty()){
            rectangle->ChangeStrokedStatus(true);
            stroke = Stroke.toStdString();
            rectangle->setStrokeColor(&stroke);
        } else {stroke = "";}
        if (!Fill.isEmpty()){
            rectangle->ChangeFilledStatus(true);
            fill = Fill.toStdString();
            rectangle->setFillColor(&fill);
        } else {fill = "";}

        addShape(rectangle);
        showInfoAlert("Add Rectangle", "Successfully Added Rectangle");
    } catch (std::exception& e){
        showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::addTriangle(){
    try{
        bool ok;
        std::string stroke, fill;
        std::shared_ptr<MyPoint> p1, p2, p3;

        QString PX1 = QInputDialog::getText(nullptr, "Add Triangle", "Enter X First Point: ", QLineEdit::Normal, "0.0", &ok);
        QString PY1 = QInputDialog::getText(nullptr, "Add Triangle", "Enter Y First Point: ", QLineEdit::Normal, "0.0", &ok);

        if (!PX1.isEmpty() && !PY1.isEmpty()){
            double px1, py1;
            px1 = PX1.toDouble();
            py1 = PY1.toDouble();
            p1 = std::make_shared <MyPoint> (px1, py1); // get my point to use with triangle points
        } else {
            showErrorAlert("Error", "Points must be valid"); return;}
        
        QString PX2 = QInputDialog::getText(nullptr, "Add Triangle", "Enter X Second Point: ", QLineEdit::Normal, "0.0", &ok);
        QString PY2 = QInputDialog::getText(nullptr, "Add Triangle", "Enter Y Second Point: ", QLineEdit::Normal, "0.0", &ok);
        
        if (!PX2.isEmpty() && !PY2.isEmpty()){
            double px1, py1;
            px1 = PX2.toDouble();
            py1 = PY2.toDouble();
            p2 = std::make_shared <MyPoint> (px1, py1); // get my point to use with triangle points
        } else {
            showErrorAlert("Error", "Points must be valid"); return;}

        QString PX3 = QInputDialog::getText(nullptr, "Add Triangle", "Enter X Third Point: ", QLineEdit::Normal, "0.0", &ok);
        QString PY3 = QInputDialog::getText(nullptr, "Add Triangle", "Enter Y Third Point: ", QLineEdit::Normal, "0.0", &ok);

        if (!PX3.isEmpty() && !PY3.isEmpty()){
            double px1, py1;
            px1 = PX3.toDouble();
            py1 = PY3.toDouble();
            p3 = std::make_shared <MyPoint> (px1, py1); // get my point to use with triangle points
        } else {
            showErrorAlert("Error", "Points must be valid"); return;}

        QString Stroke = QInputDialog::getText(nullptr, "Add Triangle", "Enter Stroke Color: ", QLineEdit::Normal, "default:black", &ok);
        QString Fill = QInputDialog::getText(nullptr, "Add Triangle", "Enter Fill Color: ", QLineEdit::Normal, "default:transparent (disabled filling)", &ok);

        auto triangle = std::make_shared<MyTriangle>(p1.get(), p2.get(), p3.get());

        if (!Stroke.isEmpty()){
            triangle->ChangeStrokedStatus(true);
            stroke = Stroke.toStdString();
            triangle->setStrokeColor(&stroke);
        } else {stroke = "";}
        if (!Fill.isEmpty()){
            triangle->ChangeFilledStatus(true);
            fill = Fill.toStdString();
            triangle->setFillColor(&fill);
        } else {fill = "";}

        addShape(triangle);
        showInfoAlert("Add Triangle", "Successfully Added Triangle");
    } catch (std::exception& e){
        showErrorAlert("Error", e.what(), e);
    }
}

void CanvasHelper::addSquare(){
    try{
        bool ok;
        std::string stroke, fill;
        double side, x, y;

        QString X = QInputDialog::getText(nullptr, "Add Circle", "Enter Start Point X: ", QLineEdit::Normal, "0.0", &ok);

        if (!X.isEmpty()){
            x = X.toDouble();
        } else {
            showErrorAlert("Error", "X must be valid"); return;}
        
        QString Y = QInputDialog::getText(nullptr, "Add Circle", "Enter Start Point Y: ", QLineEdit::Normal, "0.0", &ok);

        if (!Y.isEmpty()){
            y = Y.toDouble();
        } else {
            showErrorAlert("Error", "Y must be valid"); return;}
    
        QString Side = QInputDialog::getText(nullptr, "Add Circle", "Enter Side: ", QLineEdit::Normal, "0.0", &ok);

        if (!Side.isEmpty()){
            side = Side.toDouble();
        } else {
            showErrorAlert("Error", "Side must be valid"); return;}

        QString Stroke = QInputDialog::getText(nullptr, "Add Circle", "Enter Stroke Color: ", QLineEdit::Normal, "default:black", &ok);
        QString Fill = QInputDialog::getText(nullptr, "Add Circle", "Enter Fill Color: ", QLineEdit::Normal, "default:transparent (disabled filling)", &ok);

        auto start = std::make_shared<MyPoint>(x, y);
        auto square = std::make_shared<MySquare>(start.get(),side);

        if (!Stroke.isEmpty()){
            square->ChangeStrokedStatus(true);
            stroke = Stroke.toStdString();
            square->setStrokeColor(&stroke);
        } else {stroke = "";}
        if (!Fill.isEmpty()){
            square->ChangeFilledStatus(true);
            fill = Fill.toStdString();
            square->setFillColor(&fill);
        } else {fill = "";}

        addShape(square);
        showInfoAlert("Add Circle", "Successfully Added Square");
    } catch (std::exception& e){
        showErrorAlert("Error", e.what(), e);
    }
}