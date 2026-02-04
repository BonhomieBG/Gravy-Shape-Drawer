#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#define Pound 453592.37 // convert to miligram

#include <iostream>
#include <string>
#include <udouble>

class Measurement {
    public:
    enum Size {
        Milli = 1,
        Centi = 10,
        Deci = 100,
        Normal  = 1000,
        Deca= 10000,
        Hecto = 100000,
        Kilo = 1000000
    };

    // Pound to Gram (for display)
    std::string PtoG(udouble value){
        double temp = Pound;
        temp *= value;
        std::string display = std::to_string(temp);
        return std::string (display);
    }
    // Pound to gram double (for calculate)
    udouble PtoGD(udouble value){
        double temp = Pound;
        temp *= value;
        return temp;
    }
    // Gram to Pound
    udouble GtoPD(udouble value){
        double temp = Pound;
        temp /= value;
        return temp;
    }
    std::string GtoP(udouble value){
        double temp = Pound;
        temp /= value;
        std::string display = std::to_string(temp);
        return std::string (display);
    }
};
#endif 