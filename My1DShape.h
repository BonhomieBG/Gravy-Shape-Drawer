#ifndef MY1DSHAPE_H
#define MY1DSHAPE_H

#include "Comparable.h"
#include "MyShape.h"
#include "MyPoint.h"
#include "InvalidCoordinateException.h"

#include <string>

class My1DShape: public MyShape, public Comparable{
    private:
        MyPoint* startPoint;
        MyPoint* endPoint;
    public:
        My1DShape(MyPoint* start, MyPoint* end, std::string* color);
        My1DShape(MyPoint* start, MyPoint* end);
        virtual ~My1DShape();

        virtual double calculateLength() const = 0;
        MyPoint* getStartPoint() const;
        MyPoint* getEndPoint() const;
        int compareTo(Comparable& other) const noexcept;
        virtual bool operator==(const My1DShape& other) const;
        virtual My1DShape* max (My1DShape* o1, My1DShape* o2) const;
};

#endif // MY1DSHAPE_H