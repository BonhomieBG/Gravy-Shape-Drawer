// File name: MyRectangle.cpp
// Description: Implementation for MyRectangle class.
// 
// Time Spent: About #
// Challenges: Converting Java string methods to C++ equivalents.
//
// Revision History
// Date: By: Action
// 11/19/2025 Created the MyRectangle.cpp implementation
// 

#include "MyRectangle.h"
#include "MyPoint.h"
#include "My2DShape.h"
#include "Utility.hpp"

#include <string>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QString>

MyRectangle::MyRectangle(MyPoint* topLeft, double width, double height) : My2DShape(topLeft, makeBottomRight(topLeft, width, height)){
    if (std::isnan(width) || std::isnan(height) || width <= 0.0 || height <= 0.0) {
        throw std::invalid_argument("Width and height must be positive numbers.");
    }
    if (!std::isfinite(width) || !std::isfinite(height)) {
        throw std::invalid_argument("Width and height cannot be infinite.");
    }
    if (topLeft == nullptr) {
        throw std::invalid_argument("Top left point cannot be null.");
    }

    this->width = width;
    this->height = height;
}

MyRectangle::MyRectangle(MyPoint* topLeft, MyPoint* bottomRight) : My2DShape(topLeft, bottomRight){
    if (topLeft == nullptr || bottomRight == nullptr) {
        throw std::invalid_argument("Top left and bottom right points cannot be null.");
    }
    this->width= std::abs(bottomRight->getX() - topLeft->getX());
    this->height= std::abs(bottomRight->getY() - topLeft->getY());
}

double MyRectangle::calculateArea() const {
    return width * height;
}

double MyRectangle::calculatePerimeter() const {
    return 2 * (width + height);
}

double MyRectangle::getWidth() const {
    return width;
}

double MyRectangle::getHeight() const {
    return height;
}

std::string MyRectangle::getName() const {
    return "Rectangle";
}

QGraphicsItem* MyRectangle::toQShape() const{
    QGraphicsRectItem* rectangle = new QGraphicsRectItem();
    if (gridLineWidth > 0 && !getStrokeColor().empty()) {
        rectangle->setPen(QPen(QColor::fromString(QString::fromStdString(getStrokeColor())), gridLineWidth));
    } else if (gridLineWidth <= 0 && !getStrokeColor().empty()){
        rectangle->setPen(QPen(QColor::fromString(QString::fromStdString(getStrokeColor())), 1.0));
    } else {
        rectangle->setPen(QPen(QColor::fromString(QString::fromStdString(getDefaultStrokeColor())), 1.0));
    }

    if (getFillStatus() && !getFillColor().empty()) {
        rectangle->setBrush(QBrush(QColor::fromString(QString::fromStdString(getFillColor()))));
    } else if (getFillStatus() && getFillColor().empty()){
        rectangle->setBrush(QBrush(QColor::fromString(QString::fromStdString(getDefaultFillColor()))));
    }

    // Calculate actual top-left corner for correct drawing in all directions
    double x1 = getTopLeft()->getX();
    double y1 = getTopLeft()->getY();
    double x2 = getBottomRight()->getX();
    double y2 = getBottomRight()->getY();
    
    double actualX = std::min(x1, x2);
    double actualY = std::min(y1, y2);
    
    rectangle->setRect(actualX, actualY, width, height);

    return rectangle;
}

// End of MyRectangle.cpp