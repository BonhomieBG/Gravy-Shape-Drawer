// File name: MyTriangle.h
// Description: header for MyTriangle class.
// 
// Time Spent: About #
// Challenges: Converting Java string methods to C++ equivalents.
//
// Revision History
// Date: By: Action
// 11/18/2025 Created the MyTriangle.h implementation
// 
#ifndef MYTRIANGLE_H
#define MYTRIANGLE_H

#include "My2DShape.h"
#include "MyPoint.h"
#include "InvalidCoordinateException.h"
#include "IllegalTriangleException.h"

#include <string>
#include <cmath>
#include <stdexcept>

class MyTrianglePrivate;

class MyTriangle: public My2DShape {
public:
    MyTriangle(MyPoint* p1, MyPoint* p2, MyPoint* p3);
    MyTriangle(double side1, double side2, double side3);

    virtual ~MyTriangle()= default;  // Need to define to delete trianglePr

    double calculateArea() const override;
    double calculatePerimeter() const override;
    double calculateArea(double s1, double s2, double s3) const;
    std::string getName() const;

    double getSide1() const;
    double getSide2() const;
    double getSide3() const;
    MyPoint* getPoint1() const;
    MyPoint* getPoint2() const;
    MyPoint* getPoint3() const;

    QGraphicsItem* toQShape() const override;

private:
    double side1, side2, side3;
    MyPoint* p1, *p2, *p3;
    MyTrianglePrivate* trianglePr;
};
#endif // MYTRIANGLE_H