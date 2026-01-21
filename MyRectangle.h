// File name: MyRectangle.h
// Description: Header for MyRectangle class.
// 
// Time Spent: About #
// Challenges: Converting Java string methods to C++ equivalents.
//
// Revision History
// Date: By: Action
// 11/18/2025 Created the MyRectangle.h header
// 
#ifndef MYRECTANGLE_H
#define MYRECTANGLE_H

#include "My2DShape.h"
#include "MyPoint.h"

#include <string>
class MyRectangle: public My2DShape {
public:
    MyRectangle(MyPoint* topLeft, double width, double height);
    MyRectangle(MyPoint* topLeft, MyPoint* bottomRight);

    virtual ~MyRectangle() = default;

    double calculateArea() const override;
    double calculatePerimeter() const override;
    std::string getName() const;
    double getHeight() const;
    double getWidth() const;
    QGraphicsItem* toQShape() const override;

private:
    double width;
    double height;
};

#endif // MYRECTANGLE_H