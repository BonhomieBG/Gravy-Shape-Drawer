// File name: MySquare.cpp
// Description: Implementation for MySquare class.
// 
// Time Spent: About #
// Challenges: Converting Java string methods to C++ equivalents.
//
// Revision History
// Date: By: Action
// 11/19/2025 Created the MySquare.cpp implementation
// 

#include "MySquare.h"
#include "MyPoint.h"
#include "My2DShape.h"
#include "Utility.hpp"

#include <string>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <QGraphicsPathItem>
#include <QGraphicsItem>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QString>

MySquare::MySquare(MyPoint* topLeft, double side) : My2DShape(topLeft, makeBottomRight(topLeft, side, side)){
    if (std::isnan(side)) {
        throw std::invalid_argument("Side must be a positive number.");
    }
    if (!std::isfinite(side)) {
        throw std::invalid_argument("Side cannot be infinite.");
    }
    if (topLeft == nullptr) {
        throw std::invalid_argument("Top left point cannot be null.");
    }
    this->side = side;
}

MySquare::MySquare(MyPoint* topLeft, MyPoint* bottomRight) : My2DShape(topLeft, bottomRight){
    if (topLeft == nullptr || bottomRight == nullptr) {
        throw std::invalid_argument("Top left and bottom right points cannot be null.");
    }
    double width= std::abs(bottomRight->getX() - topLeft->getX());
    double height= std::abs(bottomRight->getY() - topLeft->getY());

    this->side = std::min(width, height);
}

double MySquare::calculateArea() const {
    return side * side;
}
double MySquare::calculatePerimeter() const {
    return 4 * side;
}

std::string MySquare::getName() const{
    return "Square";
}

double MySquare::getSide() const {
    return side;
}

// Square still buggy
QGraphicsItem* MySquare::toQShape() const{
    QGraphicsPathItem* square = new QGraphicsPathItem();
    QPainterPath path;
    if (gridLineWidth > 0 && !getStrokeColor().empty()) {
        square->setPen(QPen(QColor::fromString(QString::fromStdString(getStrokeColor())), gridLineWidth));
    } else if (gridLineWidth <= 0 && !getStrokeColor().empty()){
        square->setPen(QPen(QColor::fromString(QString::fromStdString(getStrokeColor())), 1.0));
    } else {
        square->setPen(QPen(QColor::fromString(QString::fromStdString(getDefaultStrokeColor())), 1.0));
    }

    if (getFillStatus() && !getFillColor().empty()) {
        square->setBrush(QBrush(QColor::fromString(QString::fromStdString(getFillColor()))));
    } else if (getFillStatus() && getFillColor().empty()){
        square->setBrush(QBrush(QColor::fromString(QString::fromStdString(getDefaultFillColor()))));
    }

    // Calculate actual top-left corner for correct drawing in all directions
    double x1 = getTopLeft()->getX();
    double y1 = getTopLeft()->getY();
    double x2 = getBottomRight()->getX();
    double y2 = getBottomRight()->getY();
    
    double actualX = std::min(x1, x2);
    double actualY = std::min(y1, y2);
    
    path.addRect(actualX, actualY, side, side);
    square->setPath(path);

    return square;
}

// End of MySquare.cpp