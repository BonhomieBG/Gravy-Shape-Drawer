// File name: MyCircle.cpp
// Description: Implementation for MyCircle class.
// 
// Time Spent: About #
// Challenges: Converting Java string methods to C++ equivalents.
//
// Revision History
// Date: By: Action
// 11/20/2025 Created the MyCircle.cpp implementation

#define M_PI 3.14159265358979323846

#include "MyCircle.h"
#include "MyPoint.h"
#include "My2DShape.h"
#include "Utility.hpp"
#include "InvalidRadiusException.h"

#include <stdexcept>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QColor>
#include <QPen>

MyCircle::MyCircle(MyPoint* center, double radius) : My2DShape(new MyPoint(center->getX()-radius, center->getY()-radius), new MyPoint(center->getX()+radius, center->getY()+radius)) {
    if (std::isnan(radius)) {
        throw InvalidRadiusException("Radius must be a positive number.");
    }
    if (!std::isfinite(radius)) {
        throw InvalidRadiusException("Radius cannot be infinite.");
    }
    if (center == nullptr) {
        throw std::invalid_argument("Center point cannot be null.");
    }
    this->radius = radius;
    this->centerPoint_ = center;
}

MyCircle::MyCircle(MyPoint* topLeft, MyPoint* bottomRight) : My2DShape(topLeft, bottomRight) {
    if (topLeft == nullptr || bottomRight == nullptr) {
        throw std::invalid_argument("Top left and bottom right points cannot be null.");
    }
    double centerX = (topLeft->getX() + bottomRight->getX()) / 2.0;
    double centerY = (topLeft->getY() + bottomRight->getY()) / 2.0;

    MyPoint* center = new MyPoint (centerX, centerY);
    this->radius = std::min(std::abs(bottomRight->getX() - topLeft->getX()), std::abs(bottomRight->getY() - topLeft->getY())) / 2;
    this->centerPoint_ = center;
}

double MyCircle::calculateArea() const {
    return M_PI * radius * radius;
}

double MyCircle::calculatePerimeter() const {
    return 2 * M_PI * radius;
}

double MyCircle::getDiameter() const {
    return 2 * radius;
}

std::string MyCircle::getName() const {
    return "Circle";
}

double MyCircle::getRadius() const {
    return radius;
}

MyPoint* MyCircle::getCenter() const {
    return centerPoint_;
}

QGraphicsItem* MyCircle::toQShape() const{
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem();
    if (gridLineWidth > 0 && !getStrokeColor().empty()) {
        circle->setPen(QPen(QColor(getStrokeColor().c_str()), gridLineWidth));
    } else if (gridLineWidth <= 0){
        circle->setPen(QPen(QColor(getDefaultStrokeColor().c_str()), 1.0));
    }
    if (getFillStatus() && !getFillColor().empty()) {
        circle->setBrush(QBrush(QColor(getFillColor().c_str())));
    } else if (getFillStatus() && getFillColor().empty()){
        circle->setBrush(QBrush(QColor(getDefaultFillColor().c_str())));
    }

    circle->setRect(centerPoint_->getX() - radius,
                       centerPoint_->getY() - radius,
                       2 * radius,
                       2 * radius);

    return circle;
}
