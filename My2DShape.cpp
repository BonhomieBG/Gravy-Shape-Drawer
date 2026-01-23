#include "My2DShape.h"
#include "Utility.hpp"
#include "MyPoint.h"

#include <stdexcept>
#include <sstream>
#include <cmath>

My2DShape::My2DShape(MyPoint* left, MyPoint* right)
    : MyShape(), topLeft(left), bottomRight(right) {
    if (topLeft == nullptr || bottomRight == nullptr) {
        throw std::invalid_argument("Top-left and bottom-right points cannot be null.");
    }
    if (*topLeft == *bottomRight) {
        throw std::invalid_argument("Top-Left and Bottom-Right points cannot be the same.");
    }
}

My2DShape::My2DShape(MyPoint* left, MyPoint* right, std::string& color)
    : MyShape(), topLeft(left), bottomRight(right) {
    if (topLeft == nullptr || bottomRight == nullptr) {
        throw std::invalid_argument("Top-left and bottom-right points cannot be null.");
    }
    if (*topLeft == *bottomRight) {
        throw std::invalid_argument("Top-Left and Bottom-Right points cannot be the same.");
    }
    if (!color.empty()) {
        validationCheckString(color);
        setStrokeColor(&color);
        setFillColor(&color);
    }
}

// Validators
void My2DShape::setTopLeftInternal(MyPoint* topLeft) {
    if (topLeft == nullptr) {
        throw std::invalid_argument("Top-Left point cannot be null.");
    }
    if (*topLeft == *bottomRight) {
        throw std::invalid_argument("Top-Left cannot be the same as Bottom-Right.");
    }
    this->topLeft = topLeft;
}

void My2DShape::setBottomRightInternal(MyPoint* bottomRight) {
    if (bottomRight == nullptr) {
        throw std::invalid_argument("Bottom-Right point cannot be null.");
    }
    if (*topLeft == *bottomRight) {
        throw std::invalid_argument("Bottom-Right cannot be the same as Top-Left.");
    }
    this->bottomRight = bottomRight;
}

void My2DShape::setBoundingBoxInternal(MyPoint* topLeft, MyPoint* bottomRight) {
    if (topLeft == nullptr || bottomRight == nullptr) {
        throw std::invalid_argument("Top-left and bottom-right points cannot be null.");
    }
    if (*topLeft == *bottomRight) {
        throw std::invalid_argument("Top-Left and Bottom-Right value cannot be the same.");
    }
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
}
// End of Validators

const MyPoint* My2DShape::getTopLeft() const {
    return topLeft;
}
const MyPoint* My2DShape::getBottomRight() const {
    return bottomRight;
}

bool My2DShape::operator==(My2DShape& other) const {
    const double eps = 1e-12; // Tolerance for floating-point comparison
    bool areaEqual = std::fabs(calculateArea() - other.calculateArea()) <= eps;

    auto ptrEqual = [](const MyPoint* a, const MyPoint* b) -> bool {
        if (a == nullptr && b == nullptr) return true;
        if (a == nullptr || b == nullptr) return false;
        return (*a == *b); // requires MyPoint::operator==
    }; // top left and bottom right pointer comparison

    bool cornersEqual = ptrEqual(topLeft, other.topLeft) && ptrEqual(bottomRight, other.bottomRight);
    bool colorEqual = getFillColor() == other.getFillColor();
    return areaEqual && cornersEqual && const_cast<My2DShape*>(this)->getFillStatus() == other.getFillStatus() && colorEqual;
}

int My2DShape::compareTo(Comparable& other) const noexcept{
    auto p = dynamic_cast<const My2DShape*>(&other);
    if (!p) showErrorAlert("Error", "compareTo: incompatible type");
    double a = calculateArea();
    double b = p->calculateArea();
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

My2DShape* My2DShape::max(My2DShape* o1, My2DShape* o2) const {
    if (o1 == nullptr && o2 == nullptr) return nullptr;
    if (o1 == nullptr) return o2;
    if (o2 == nullptr) return o1;
    return (o1->compareTo(*o2) >= 0) ? o1 : o2;
}
