// File name: MyLine.cpp
// Description: Handle Line drawing
// 
// Time Spent: About 15 mins
// Challenges: #
//
// Revision History
// Date: By: Action
// 11/20/2025 Created the MyLine.cpp class

#include "MyLine.h"
#include "MyShape.h"
#include "My1DShape.h"
#include "Utility.hpp"

#include <string>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QPen>
#include <QColor>
#include <QString>


MyLine::MyLine (MyPoint* startPoint, MyPoint* endPoint, std::string* color)
    : My1DShape (startPoint, endPoint, color) {
    if (startPoint == nullptr || endPoint == nullptr) {
        throw std::invalid_argument("Start point and end point cannot be null.");
    }
    if (color != nullptr && !color->empty()) {
        validationCheckString(*color);
        setStrokeColor(color);
    }
}

MyLine::MyLine (MyPoint* startPoint, MyPoint* endPoint)
    : My1DShape (startPoint, endPoint) {
    if (startPoint == nullptr || endPoint == nullptr) {
        throw std::invalid_argument("Start point and end point cannot be null.");
    }
}

double MyLine::calculateLength() const {
    const MyPoint* start = getStartPoint();
    const MyPoint* end = getEndPoint();

    double deltaX = end->getX() - start->getX();
    double deltaY = end->getY() - start->getY();
    return std::sqrt (deltaX * deltaX + deltaY * deltaY);
}

std::string MyLine::getName() const{
    return "Line";
}

QGraphicsItem* MyLine::toQShape() const{
    QGraphicsLineItem* line = new QGraphicsLineItem();
    if (gridLineWidth > 0 && !getStrokeColor().empty()) {
        line->setPen(QPen(QColor::fromString(QString::fromStdString(getStrokeColor())), gridLineWidth));
    } else if (gridLineWidth <= 0 && !getStrokeColor().empty()){
        line->setPen(QPen(QColor::fromString(QString::fromStdString(getStrokeColor())), 1.0));
    } else {
        line->setPen(QPen(QColor::fromString(QString::fromStdString(getDefaultStrokeColor())), 1.0));
    }

    line->setLine(getStartPoint()->getX(), getStartPoint()->getY(),
                  getEndPoint()->getX(), getEndPoint()->getY());
    return line;
}

// End of Line.cpp
