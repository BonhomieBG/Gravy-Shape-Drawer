#include "MyShape.h"
#include "Utility.hpp"

#include <stdexcept>

MyShape::MyShape(){
    this->Fillcolor = DefaultFillColor;
    this->StrokeColor = DefaultStrokeColor;
    this->stroke = true;
    this->filled = false;
    this->gridLineWidth = 1.0;
}

MyShape::MyShape(std::string* color) : MyShape() {
    if (color != nullptr && !color->empty()){
        if (!isAlphaWord(*color)){
            throw std::invalid_argument("Color must be an alphabetic word.");
        }
        *color = trim(*color);
        *color = toLowerCase(*color);
        setFillColor(color);
        setStrokeColor(color);
    }
}

MyShape::MyShape(std::string* fillColor, std::string* strokeColor): MyShape(){
    if (fillColor != nullptr && !fillColor->empty()){
        if (!isAlphaWord(*fillColor)){
            throw std::invalid_argument("Fill color must be an alphabetic word.");
        }
    } else if (fillColor == nullptr && fillColor->empty()){
        *fillColor = *pDefaultFill;
    }
    *fillColor = trim(*fillColor);
    *fillColor = toLowerCase(*fillColor);
    setFillColor(fillColor);
    
    if (strokeColor != nullptr && !strokeColor->empty()){
        if (!isAlphaWord(*strokeColor)){
            throw std::invalid_argument("Stroke color must be an alphabetic word.");
        }
    } else if (strokeColor == nullptr && strokeColor->empty()){
        *strokeColor = *pDefaultStroke;
    }
    *strokeColor = trim(*strokeColor);
    *strokeColor = toLowerCase(*strokeColor);
    setStrokeColor(strokeColor);
}

MyShape::~MyShape(){}

std::string MyShape::getFillColor() const {
    if (Fillcolor.empty()){
        return DefaultFillColor;
    }
    return Fillcolor;
}

std::string MyShape::getStrokeColor() const {
    if (StrokeColor.empty()){
        return DefaultStrokeColor;
    }
    return StrokeColor;
}

std::string MyShape::getDefaultFillColor() const{
    return DefaultFillColor;
}

std::string MyShape::getDefaultStrokeColor() const{
    return DefaultStrokeColor;
}

void MyShape::setFillColor(std::string* color){
    if (color != nullptr){
        Fillcolor = *color;
    } else {
        Fillcolor = DefaultFillColor;
    }
}

void MyShape::setStrokeColor(std::string* color){
    if (color != nullptr){
        StrokeColor = *color;
    } else {
        StrokeColor = DefaultStrokeColor;
    }
}

bool MyShape::getStrokeStatus() const {
    return stroke;
}

bool MyShape::getFillStatus() const {
    return filled;
}

bool MyShape::ChangeStrokedStatus(bool status){
    stroke = status;
    return stroke;
}

bool MyShape::ChangeFilledStatus(bool status){
    filled = status;
    return filled;
}

void MyShape::setGridLineWidth(double width){
    gridLineWidth = width;
}

double MyShape::getGridLineWidth() const {
    return gridLineWidth;
}

std::string MyShape::NametoLower(){
    std::string name = getName();
    return toLowerCase(name);
}

std::string MyShape::NametoUpper(){
    std::string name = getName();
    return toUpperCase(name);
}