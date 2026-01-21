// File name: MyLine.h
// Description: Handle my line class.
// 
// Time Spent: About 10 mins
// Challenges: #
//
// Revision History
// Date: By: Action
// 11/20/2025 Created the MyLine.h class

#ifndef MYLINE_H
#define MYLINE_H

#include "My1DShape.h"
#include "MyPoint.h"

#include <string>
#include <sstream>

class MyLine : public My1DShape {
public:
    MyLine(MyPoint* startPoint, MyPoint* endPoint, std::string* color);
    MyLine(MyPoint* startPoint, MyPoint* endPoint);
    virtual ~MyLine() = default;

    double calculateLength() const override;

    std::string getName() const override;
    QGraphicsItem* toQShape() const override;
};

#endif // MYLINE_H