// File name: MyTriangle.cpp
// Description: implementation for MyTriangle class.
// 
// Time Spent: About 4h
// Challenges: Converting Java string methods to C++ equivalents.
//
// Revision History
// Date: By: Action
// 11/18/2025 Created the MyTriangle implementation
// 11/22/2025 Added area and perimeter calculation and fixing to String errors

#include "MyTriangle.h"
#include "MyPoint.h"
#include "My2DShape.h"
#include "Utility.hpp"
#include <udouble>

#include <string>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPolygonF>
#include <QPointF>
#include <QString>
#include <QGraphicsPolygonItem>

class MyTrianglePrivate{
    public:
        // Unique helper for triangle only ** private **
    void isValidCoordinate(MyPoint* p){
        if (p == nullptr) {
            throw InvalidCoordinateException("Triangle Draw points can't be null.");
        }
        if (std::isnan(p->getX()) || std::isnan(p->getY()) || std::isinf(p->getX()) || std::isinf(p->getY())) {
            throw InvalidCoordinateException("Triangle Draw points must be valid numbers and not infinite.");
        }
    }

    void validateSides( double a, double b, double c){
        if (std::isnan(a) || std::isnan(b) || std::isnan(c)) {
            throw IllegalTriangleException("Triangle sides must be positive numbers.");
        }
        if (!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(c)) {
            throw IllegalTriangleException("Triangle sides cannot be infinite.");
        }
        // Use epsilon for floating-point comparison tolerance else triangle drawing error due to floating point non exact
        const double epsilon = 1e-10; //0.0000000001
        if (a + b < c - epsilon || a + c < b - epsilon || b + c < a - epsilon) {
            throw IllegalTriangleException("The sum of any two sides must be greater than the third side.");
        }
    }

    MyPoint calculateTopLeft(MyPoint* p1, MyPoint* p2, MyPoint* p3){
        isValidCoordinate(p1);
        isValidCoordinate(p2);
        isValidCoordinate(p3);

        double minX = std::min(std::min(p1->getX(), p2->getX()), p3->getX());
        double maxY = std::max(std::max(p1->getY(), p2->getY()), p3->getY());
        return MyPoint(minX, maxY);
    }

    MyPoint calculateBottomRight(MyPoint* p1, MyPoint* p2, MyPoint* p3){
        isValidCoordinate(p1);
        isValidCoordinate(p2);
        isValidCoordinate(p3);

        double maxX = std::max(std::max(p1->getX(), p2->getX()), p3->getX());
        double minY = std::min(std::min(p1->getY(), p2->getY()), p3->getY());
        return MyPoint(maxX, minY);
    }
};

MyTriangle::MyTriangle(MyPoint* p1, MyPoint *p2, MyPoint* p3)
    : My2DShape(
        new MyPoint(std::min({p1->getX(), p2->getX(), p3->getX()}), 
                    std::min({p1->getY(), p2->getY(), p3->getY()})),
        new MyPoint(std::max({p1->getX(), p2->getX(), p3->getX()}), 
                    std::max({p1->getY(), p2->getY(), p3->getY()}))),
      trianglePr(new MyTrianglePrivate())
{
    // Initialize points first
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    
    // Validate sides
    trianglePr->validateSides(p1->distance(*p2), p2->distance(*p3), p3->distance(*p1));

    // Calculate sides
    this->side1 = p1->distance(*p2);
    this->side2 = p2->distance(*p3);
    this->side3 = p3->distance(*p1);
}

MyTriangle::MyTriangle(double side1, double side2, double side3)
    : My2DShape(new MyPoint(0, 0), new MyPoint(side1, (2*calculateArea(side1, side2, side3))/side1)),
      trianglePr(new MyTrianglePrivate())
{
    this->side1 = side1;
    this->side2 = side2;
    this->side3 = side3;
    trianglePr->validateSides(side1, side2, side3);

    // Create points based on sides
    this->p1 = new MyPoint(0, 0);
    this->p2 = new MyPoint(side1, 0);
    double height = (2*calculateArea(side1, side2, side3)) / side1;
    this->p3 = new MyPoint(side1/2.0, height);
}

double MyTriangle::calculateArea(double s1, double s2, double s3) const {
    double s = (s1+s2+s3)/2.0;
    return std::sqrt(s*(s-s1)*(s-s2)*(s-s3));
}

double MyTriangle::calculateArea() const {
    return calculateArea(side1, side2, side3);
}

double MyTriangle::calculatePerimeter() const {
    return side1 + side2 + side3;
}

double MyTriangle::getSide1() const {
    return side1; 
}

double MyTriangle::getSide2() const {
    return side2; 
}

double MyTriangle::getSide3() const {
    return side3; 
}

MyPoint* MyTriangle::getPoint1() const {
    return p1; 
}

MyPoint* MyTriangle::getPoint2() const {
    return p2; 
}

MyPoint* MyTriangle::getPoint3() const {
    return p3; 
}

std::string MyTriangle::getName() const {
    return "Triangle";
}

QGraphicsItem* MyTriangle::toQShape() const{
    QGraphicsPolygonItem* triangle = new QGraphicsPolygonItem();

    if (gridLineWidth > 0 && !getStrokeColor().empty()) {
        triangle->setPen(QPen(QColor::fromString(QString::fromStdString(getStrokeColor())), gridLineWidth));
    } else if (gridLineWidth <= 0 && !getStrokeColor().empty()){
        triangle->setPen(QPen(QColor::fromString(QString::fromStdString(getStrokeColor())), 1.0));
    } else {
        triangle->setPen(QPen(QColor::fromString(QString::fromStdString(getDefaultStrokeColor())), 1.0));
    }

    if (getFillStatus() && !getFillColor().empty()) {
        triangle->setBrush(QBrush(QColor::fromString(QString::fromStdString(getFillColor()))));
    } else if (getFillStatus() && getFillColor().empty()){
        triangle->setBrush(QBrush(QColor::fromString(QString::fromStdString(getDefaultFillColor()))));
    }

    triangle->setPolygon(QPolygonF()<< QPointF(p1->getX(), p1->getY()) 
                                      << QPointF(p2->getX(), p2->getY()) 
                                      << QPointF(p3->getX(), p3->getY()));
    return triangle;
}

