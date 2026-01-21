#ifndef MYSHAPE_H
#define MYSHAPE_H

#include <string>
#include <stdexcept>

class QGraphicsItem;
// Using pointer instead of reference to allow null values
class MyShape {
    private:
        std::string Fillcolor;
        std::string StrokeColor;
        const std::string DefaultFillColor = "transparent";
        const std::string DefaultStrokeColor = "black";
        const std::string* pDefaultStroke = &DefaultStrokeColor;
        const std::string* pDefaultFill = &DefaultFillColor;
        bool stroke;
        bool filled;
    protected:
        double gridLineWidth;

    public:
        MyShape();
        MyShape(std::string* color);
        MyShape(std::string* fillColor, std::string* strokeColor);
        virtual ~MyShape();

        virtual std::string getName() const = 0;
        std::string getFillColor() const;
        std::string getStrokeColor() const;
        void setFillColor(std::string* color);
        void setStrokeColor(std::string* color);
        std::string getDefaultFillColor() const;
        std::string getDefaultStrokeColor() const;
        bool ChangeStrokedStatus(bool status);
        bool ChangeFilledStatus(bool status);
        bool getStrokeStatus() const;
        bool getFillStatus() const;
        std::string NametoLower();
        std::string NametoUpper();

        virtual QGraphicsItem* toQShape() const = 0;
        void setGridLineWidth(double width);
        double getGridLineWidth() const;
};

#endif // MYSHAPE_H