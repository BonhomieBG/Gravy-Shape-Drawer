// File name: MyPoint.cpp
// Description: Handle point for drawing shapes.
// 
// Time Spent: About 45 mins
// Challenges: #
//
// Revision History
// Date: By: Action
// 11/18/2025 Created the MyPoint.cpp class

#include "MyPoint.h"
#include "InvalidCoordinateException.h"

#include <string>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <memory>

MyPoint::MyPoint(double x, double y){
    if (std::isnan(x) || std::isnan(y) || !std::isfinite(x) || !std::isfinite(y)) {
        throw InvalidCoordinateException("Coordinates must be valid and cannot be NaN or Infinite.");
    }
    // Note: Negative coordinates are allowed for graphics contexts
    x_ = x;
    y_ = y;
}

double MyPoint::getX() const noexcept {
    return x_;
}

double MyPoint::getY() const noexcept {
    return y_;
}

void MyPoint::setX(double x){
    if (std::isnan(x) || !std::isfinite(x)) {
        throw InvalidCoordinateException("X coordinate must be valid and cannot be NaN or Infinite.");
    }
    // Note: Negative coordinates are allowed for graphics contexts
    x_ = x;
}

void MyPoint::setY(double y){
    if (std::isnan(y) || !std::isfinite(y)) {
        throw InvalidCoordinateException("Y coordinate must be valid and cannot be NaN or Infinite.");
    }
    // Note: Negative coordinates are allowed for graphics contexts
    y_ = y;
}

double MyPoint::distance(MyPoint& other) const {
    return std::sqrt (std::pow(other.getX() - x_, 2) + std::pow(other.getY() - y_, 2));
}

double MyPoint::distance(MyPoint* other) const {
    if (other == nullptr) {
        throw InvalidCoordinateException("Cannot calculate distance to a null point.");
    }
    return distance(*other);
}

bool MyPoint::operator==(const MyPoint& other) const noexcept {
    uint64_t ax, ay, bx, by;
    std::memcpy(&ax, &x_, sizeof x_);
    std::memcpy(&ay, &y_, sizeof x_);
    std::memcpy(&bx, &other.x_, sizeof other.x_);
    std::memcpy(&by, &other.y_, sizeof other.y_);
    return ax == bx && ay == by;
}

auto MyPoint::calculateTopLeft(MyPoint* p1, MyPoint* p2) const{
    double minX = std::min(p1->getX(), p2->getX());
    double minY = std::min(p1->getY(), p2->getY());
    return std::make_shared<MyPoint>(minX, minY);
}

auto MyPoint::calculateButtomRight(MyPoint* p1, MyPoint* p2) const{
    double maxX = std::max(p1->getX(), p2->getX());
    double maxY = std::max(p1->getY(), p2->getY());
    return std::make_shared<MyPoint>(maxX, maxY);
}
// End of MyPoint.cpp