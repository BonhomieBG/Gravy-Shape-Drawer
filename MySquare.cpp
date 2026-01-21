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
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QBrush>
#include <QColor>
#include <QPen>

MySquare::MySquare(MyPoint* topLeft, double side) : My2DShape(topLeft, makeBottomRight(topLeft, side, side)){
    if (std::isnan(side) || side <= 0) {
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

QGraphicsItem* MySquare::toQShape() const{
    QGraphicsRectItem* square = new QGraphicsRectItem();
    if (gridLineWidth > 0 && !getStrokeColor().empty()) {
        square->setPen(QPen(QColor(getStrokeColor().c_str()), gridLineWidth));
    } else if (gridLineWidth <= 0){
        square->setPen(QPen(QColor(getDefaultStrokeColor().c_str()), 1.0));
    }

    if (getFillStatus() && !getFillColor().empty()) {
        square->setBrush(QBrush(QColor(getFillColor().c_str())));
    } else if (getFillStatus() && getFillColor().empty()){
        square->setBrush(QBrush(QColor(getDefaultFillColor().c_str())));
    }

    square->setRect(getTopLeft()->getX(),
                       getTopLeft()->getY(),
                       side,
                       side);

    return square;
}

// End of MySquare.cpp