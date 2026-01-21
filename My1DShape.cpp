#include "My1DShape.h"
#include "MyShape.h"
#include "Utility.hpp"

#include <stdexcept>
#include <sstream>
#include <cmath>
#include <string>

My1DShape::My1DShape (MyPoint* start, MyPoint* end)
    : MyShape(), startPoint(start), endPoint(end){
        if (startPoint == endPoint) {
            throw InvalidCoordinateException("Start point and end point cannot be the same.");
        }
        if (startPoint == nullptr || endPoint == nullptr) {
            throw InvalidCoordinateException("Start point and end point cannot be null.");
        }
    }

My1DShape::My1DShape(MyPoint* start, MyPoint* end, std::string* color) : MyShape(color), startPoint(start), endPoint(end) {
        if (startPoint == endPoint) {
            throw InvalidCoordinateException("Start point and end point cannot be the same.");
        }
        if (startPoint == nullptr || endPoint == nullptr) {
            throw InvalidCoordinateException("Start point and end point cannot be null.");
        }
        if (!color->empty()){
            validationCheckString(*color);
            setStrokeColor(color); // 1D shape uses stroke color only
        }
    }

My1DShape::~My1DShape(){};

MyPoint* My1DShape::getStartPoint() const{
    return startPoint;
}

MyPoint* My1DShape::getEndPoint() const{
    return endPoint;
}

bool My1DShape::operator==(const My1DShape& other) const {
    auto eqPtr = [](const MyPoint* p1, const MyPoint* p2) -> bool {
        if (p1 == nullptr && p2 == nullptr) return true;
        if (p1 == nullptr || p2 == nullptr) return false;
        return (*p1 == *p2);
    };
    return eqPtr(startPoint, other.startPoint) &&
           eqPtr(endPoint, other.endPoint);
}

My1DShape* My1DShape::max(My1DShape* o1, My1DShape* o2) const {
    if (o1 == nullptr || o2 == nullptr) {
        throw InvalidCoordinateException("Both shape can't be null.");
    }
    if (o1 == nullptr) return (o2);
    if (o2 == nullptr) return (o1);
    return (o1->compareTo(*o2)>=0)? o1:o2;
}

int My1DShape::compareTo(Comparable& other) const noexcept{
    auto p = dynamic_cast<const My1DShape*>(&other);
    if (!p) showErrorAlert("Error", "compareTo: incompatible type");
    double a = calculateLength();
    double b = p->calculateLength();
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}