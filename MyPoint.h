#ifndef MYPOINT_H
#define MYPOINT_H

#include "InvalidCoordinateException.h"

#include <stdexcept>
#include <string>

class MyPoint {
public: 
    MyPoint(double x, double y);
    
    virtual ~MyPoint() = default;

    double getX() const noexcept;
    double getY() const noexcept;
    void setX(const double x);
    void setY(const double y);

    double distance(MyPoint& other) const; // Accept direct reference
    double distance(MyPoint* other) const; // Accept pointer for null checks
    auto calculateButtomRight(MyPoint* p1, MyPoint* p2) const;
    auto calculateTopLeft(MyPoint* p1, MyPoint* p2) const;

    bool operator==(const MyPoint& other) const noexcept;
private:
    double x_{0.0};
    double y_{0.0};
};

#endif // MYPOINT_H