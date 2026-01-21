// File name: MySquare.h
// Description: header for MySquare class.
// 
// Time Spent: About #
// Challenges: Converting Java string methods to C++ equivalents.
//
// Revision History
// Date: By: Action
// 11/18/2025 Created the MySquare.cpp implementation
// 
#ifndef MYSQUARE_H
#define MYSQUARE_H

#include "My2DShape.h"
#include "MyPoint.h"

#include <string>
class MySquare: public My2DShape {
public:
    MySquare(MyPoint* topLeft, double side);
    MySquare(MyPoint* topLeft, MyPoint* bottomRight);

    virtual ~MySquare() = default;

    double calculateArea() const override;
    double calculatePerimeter() const override;
    std::string getName() const;
    double getSide() const;

    QGraphicsItem* toQShape() const override;

private:
    double side;
};
#endif // MYSQUARE_H