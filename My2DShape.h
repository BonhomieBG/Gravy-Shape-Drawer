// File name: My2DShape.h
// Description: Class to handle 2D shapes.
// 
// Time Spent: About 10 mins
// Challenges: None
//
// Revision History
// Date: By: Action
// 11/16/2025 Created the My2DShape.h class
// 

#ifndef MY2DSHAPE_H
#define MY2DSHAPE_H

#include "Comparable.h"
#include "MyShape.h"
#include "MyPoint.h"
#include <string>

class My2DShape : public MyShape, public Comparable {
public: 
    My2DShape(MyPoint* topleft, MyPoint* bottomright);
    My2DShape(MyPoint* topleft, MyPoint* bottomright, std::string& color);

    virtual ~My2DShape() = default;

    // Abstract methods
    virtual double calculateArea() const = 0;
    virtual double calculatePerimeter() const = 0;
    
    // Concrete methods
    const MyPoint* getTopLeft() const;
    const MyPoint* getBottomRight() const;

    bool operator==(My2DShape& other) const;
    int compareTo(Comparable& other) const noexcept;
    My2DShape* max(My2DShape* o1, My2DShape* o2) const;
    
private:
    MyPoint* topLeft;
    MyPoint* bottomRight;

    // Helper methods to prevent invalid state
    void setTopLeftInternal(MyPoint* topLeft);
    void setBottomRightInternal(MyPoint* bottomRight);
    void setBoundingBoxInternal(MyPoint* topLeft, MyPoint* bottomRight);
};
#endif // MY2DSHAPE_H

