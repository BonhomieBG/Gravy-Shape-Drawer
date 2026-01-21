// File name: MyCircle.h
// Description: Header for MyCircle class.
// 
// Time Spent: About #
// Challenges: Converting Java string methods to C++ equivalents.
//
// Revision History
// Date: By: Action
// 11/20/2025 Created the MyCircle.h header
// 

#ifndef MYCIRCLE_H
#define MYCIRCLE_H

#include "My2DShape.h"
#include "MyPoint.h"

#include <string>
#include <cmath>

class MyCircle: public My2DShape {
public:
    MyCircle(MyPoint* center, double radius);
    MyCircle(MyPoint* topLeft, MyPoint* bottomRight);

    virtual ~MyCircle() = default;

    double calculateArea() const override;
    double calculatePerimeter() const override;
    double getDiameter() const;
    MyPoint* getCenter() const;
    std::string getName() const override;
    double getRadius() const;
    QGraphicsItem* toQShape() const;

private:
    double radius;
    MyPoint* centerPoint_{0};
};
#endif // MYCIRCLE_H