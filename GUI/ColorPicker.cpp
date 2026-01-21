#include "ColorPicker.h"
#include "../Utility.hpp"
#include "GUIHandler.h"

#include <functional>
#include <memory>
#include <QBrush>
#include <QLinearGradient>
#include <QColor>
#include <stdexcept>

// Initialize static members
std::function<void()> ColorPicker::updateCallback;
bool ColorPicker::useGradient = false;
std::string ColorPicker::gradientStartColor;
std::string ColorPicker::gradientEndColor;
std::string ColorPicker::gradientDirection;
const std::string ColorPicker::GDefaultStart = "white";
const std::string ColorPicker::GDefaultEnd = "black";
const std::string ColorPicker::GDefaultDirection = "HORIZONTAL";
int ColorPicker::r = -1;
int ColorPicker::g = -1;
int ColorPicker::b = -1;

bool ColorPicker::isUseGradient()
{
    return useGradient;
}

void ColorPicker::setUseGradient(bool option){
    useGradient = option;
}

void ColorPicker::setGradientDirection(std::string* direction){
    if (*direction == "DIAGONAL LEFT" || *direction == "diagonal left"){
        gradientDirection = "DIAGONAL LEFT";
        if (updateCallback) updateCallback();
        return;
    } else if (*direction == "diagonal right" || *direction == "DIAGONAL RIGHT"){
        gradientDirection = "DIAGONAL RIGHT";
        if (updateCallback) updateCallback();
        return;
    }

    std::string checked = validationCheckStringUp (*direction);
    gradientDirection = checked; // createGradient will catch invalid direction;
    if (updateCallback) updateCallback();
}

std::shared_ptr<std::string> ColorPicker::getGradientStartColor(){
    return std::shared_ptr<std::string>(&gradientStartColor, [](void*){});
}

std::shared_ptr<std::string> ColorPicker::getGradientEndColor(){
    return std::shared_ptr<std::string>(&gradientEndColor, [](void*){});
}

std::shared_ptr<std::string> ColorPicker::getGradientDirection(){
    return std::shared_ptr<std::string>(&gradientDirection, [](void*){});
}

void ColorPicker::resetGradientColor(){
    gradientStartColor = GDefaultStart;
    gradientEndColor = GDefaultEnd;
    if (updateCallback) updateCallback();
}

void ColorPicker::setGradientColors(std::string* start, std::string* end){
    try{
    if (start == nullptr){
        gradientStartColor = GDefaultStart;
    } else{
        gradientStartColor = validationCheckString(*start);
        QColor color = QColor::fromString(gradientStartColor);
    }

    if (end == nullptr){
        gradientEndColor = GDefaultEnd;
    } else{
        gradientEndColor = validationCheckString(*end);
        QColor color = QColor::fromString(gradientEndColor);
    }

    if (updateCallback) updateCallback();
}
    catch (std::exception& e){
        gradientStartColor = GDefaultStart;
        gradientEndColor = GDefaultEnd;
        showErrorAlert ("Error", e.what(), e);
    }
}

void ColorPicker::resetGradientSetting(){
    useGradient = false;
    gradientStartColor = GDefaultStart;
    gradientEndColor = GDefaultEnd;
    gradientDirection = "HORIZONTAL";
    if (updateCallback) updateCallback();
}

QLinearGradient ColorPicker::createGradient(){
    try{
        auto direction = getGradientDirection();
        if (gradientStartColor.empty()) {gradientStartColor= GDefaultStart;}
        if (gradientEndColor.empty()) {gradientEndColor = GDefaultEnd;}

        if (*direction == "VERTICAL") {
            QLinearGradient gradient (QPointF (0, 0), QPointF(0, 1));
            std::string rgbM = getColoringMode();
            if (rgbM == "RGB Mode") {
            gradient.setColorAt(0.0, QColor::fromRgb(r,g,b));
            gradient.setColorAt(1.0, QColor::fromString(GDefaultEnd));
            } else{
            gradient.setColorAt(0.0, QColor::fromString(gradientStartColor));
            gradient.setColorAt(1.0, QColor::fromString(gradientEndColor));
            }
            return gradient;
        }
        if (*direction == "HORIZONTAL") {
            QLinearGradient gradient (QPointF (0, 0), QPointF(1, 0));
            std::string rgbM = getColoringMode();
            if (rgbM == "RGB Mode") {
            gradient.setColorAt(0.0, QColor::fromRgb(r,g,b));
            gradient.setColorAt(1.0, QColor::fromString(GDefaultEnd));
            } else{
            gradient.setColorAt(0.0, QColor::fromString(gradientStartColor));
            gradient.setColorAt(1.0, QColor::fromString(gradientEndColor));
            }
            return gradient;
            return gradient;
        }
        if (*direction == "DIAGONAL LEFT") {
            QLinearGradient gradient (QPointF (1, 0), QPointF(0, 1));
            std::string rgbM = getColoringMode();
            if (rgbM == "RGB Mode") {
            gradient.setColorAt(0.0, QColor::fromRgb(r,g,b));
            gradient.setColorAt(1.0, QColor::fromString(GDefaultEnd));
            } else{
            gradient.setColorAt(0.0, QColor::fromString(gradientStartColor));
            gradient.setColorAt(1.0, QColor::fromString(gradientEndColor));
            }
            return gradient;
        }

        if (*direction == "DIAGONAL RIGHT") {
            QLinearGradient gradient (QPointF (0, 0), QPointF(1, 1));
            std::string rgbM = getColoringMode();
            if (rgbM == "RGB Mode") {
            gradient.setColorAt(0.0, QColor::fromRgb(r,g,b));
            gradient.setColorAt(1.0, QColor::fromString(GDefaultEnd));
            } else{
            gradient.setColorAt(0.0, QColor::fromString(gradientStartColor));
            gradient.setColorAt(1.0, QColor::fromString(gradientEndColor));
            }
            return gradient;
        }
        return QLinearGradient();
    } catch (std::exception& e){
        showErrorAlert("Error", e.what(), e);
        return QLinearGradient();
    }
}

QLinearGradient ColorPicker::createGradient(std::string* color1, std::string* color2){
    auto direction = getGradientDirection();
    std::string fcolor = validationCheckString(*color1);
    std::string scolor = validationCheckString(*color2);

    try {
        if (*direction == "VERTICAL"){
            QLinearGradient gradient (QPointF (0, 0), QPointF(0,1));
            gradient.setColorAt(0.0, QColor::fromString(fcolor));
            gradient.setColorAt(1.0, QColor::fromString(scolor));
            return gradient;
        }
        if (*direction == "HORIZONTAL"){
            QLinearGradient gradient (QPointF (0, 0), QPointF(1,0));
            gradient.setColorAt(0.0, QColor::fromString(fcolor));
            gradient.setColorAt(1.0, QColor::fromString(scolor));
            return gradient;
        }
        if (*direction == "DIAGONAL RIGHT"){
            QLinearGradient gradient (QPointF (0, 0), QPointF(1,1));
            gradient.setColorAt(0.0, QColor::fromString(fcolor));
            gradient.setColorAt(1.0, QColor::fromString(scolor));
            return gradient;
        }
        if (*direction == "DIAGONAL LEFT"){
            QLinearGradient gradient (QPointF (1, 0), QPointF(0,1));
            gradient.setColorAt(0.0, QColor::fromString(fcolor));
            gradient.setColorAt(1.0, QColor::fromString(scolor));
            return gradient;
        }
    } catch (std::exception &e){
        showErrorAlert("Error", e.what());
    }
    return QLinearGradient();
}

// Set rgb value
void ColorPicker::setRGB(int R, int G, int B){
    if (R>=0 && G>=0 && B>=0 && R<=255 && G<=255 && B<=255){
    r=R;
    g=G;
    b=B;
    if (updateCallback) updateCallback();
    } else return;
}

void ColorPicker::getRGB(){
    std::ostringstream oss;
    oss << "R: " << r << " G: " << g << " B: " << b;

    showInfoAlert("RGB Value: ", oss.str());
}

std::string ColorPicker::getColoringMode(){
    if (r>=0 && g>=0 && b>=0 && r<=255 && g<=255 && b<=255){
        return "RGB Mode";
    } else{
        return "Default (NO RGB)";
    }
}

void ColorPicker::resetRGB(){
    r= -1;
    g= -1;
    b= -1;
}