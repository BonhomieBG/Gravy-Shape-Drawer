// COPYRIGHT (C) 2026 Tekhour Khov. All Right reserved
// This File is Free and you can redistribute it and/or modify it and is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// this class is to be use as a type class for double value that shouldn't be negative
// usage with include "udouble.h" header
// or with include <udouble> if placed in path enviroment
// udouble is created with inspiration from uint, eliminate unnescessary if statement on double precision value that shouldn't be negative

#ifndef udouble_H
#define udouble_H

#include <stdexcept>
#include <iostream>

class udouble{
    double value;

public:
    udouble(double v){
        if (v<0.00){
            throw std::invalid_argument("Value must be non negative");
        }
        value = v;
    };
    udouble(){
        value = 0.00;
    }
    
    operator double() const { 
        return value;
    }

    // Arithmetic operators
    udouble operator+ (const udouble& other) const { 
        return udouble(value + other.value); 
    }

    udouble operator- (const udouble& other) const { 
        double result = value - other.value;
        if (result < 0.0) result = 0.0; return udouble(result);
    }

    udouble operator* (const udouble& other) const {
        return udouble(value * other.value);
    }

    udouble operator/ (const udouble& other) const {
        if (other.value == 0.0) throw std::invalid_argument("Division by zero");
        return udouble(value / other.value); 
    }

    udouble& operator+=(const udouble& other) {
        value += other.value; return *this; 
    }
    udouble& operator-=(const udouble& other) { 
        value -= other.value; 
        if (value < 0.0) value = 0.0; return *this; 
    }

    udouble& operator*=(const udouble& other) { 
        value *= other.value; 
        return *this; 
    }

    udouble& operator/=(const udouble& other) { 
        if (other.value == 0.0) throw std::invalid_argument("Division by zero");
        value /= other.value; 
        return *this; 
    }

    udouble operator+ (const double& other) const { 
        return udouble(value + other); 
    }

    udouble operator- (const double& other) const { 
        double result = value - other;
        if (result < 0.0) result = 0.0; return udouble(result);
    }

    udouble operator* (const double& other) const {
        return udouble(value * other);
    }

    udouble operator/ (const double& other) const {
        if (other == 0.0) throw std::invalid_argument("Division by zero");
        return udouble(value / other); 
    }

    udouble& operator+=(const double& other) {
        value += other; return *this; 
    }
    udouble& operator-=(const double& other) { 
        value -= other; 
        if (value < 0.0) value = 0.0; return *this; 
    }

    udouble& operator*=(const double& other) { 
        value *= other; 
        return *this; 
    }

    udouble& operator/=(const double& other) { 
        if (other == 0.0) throw std::invalid_argument("Division by zero");
        value /= other; 
        return *this; 
    }

    // Comparison operators
    bool operator==(const udouble& other) const { 
        return value == other.value;
    }
    bool operator!=(const udouble& other) const { 
        return value != other.value; 
    }
    bool operator<(const udouble& other) const { 
        return value < other.value; 
    }
    bool operator<=(const udouble& other) const { 
        return value <= other.value; 
    }
    bool operator>(const udouble& other) const { 
        return value > other.value; 
    }
    bool operator>=(const udouble& other) const { 
        return value >= other.value; 
    }

    bool operator==(const double& other) const { 
        return value == other;
    }
    bool operator!=(const double& other) const { 
        return value != other; 
    }
    bool operator<(const double& other) const { 
        return value < other; 
    }
    bool operator<=(const double& other) const { 
        return value <= other; 
    }
    bool operator>(const double& other) const { 
        return value > other; 
    }
    bool operator>=(const double& other) const { 
        return value >= other; 
    }

    // Assignment from double
    udouble& operator=(double v) { if (v < 0.0) throw std::invalid_argument("Value must be non negative"); value = v; return *this; }

    // Stream output << and input >>
    friend std::ostream& operator<<(std::ostream& os, const udouble& obj) { os << obj.value; return os; }
    friend std::istream& operator>>(std::istream& is, udouble& obj) { 
        double value;
        is >> value;
        if (value < 0.0) {
            is.setstate(std::ios::failbit);
        } else {
            obj.value = value;
        }
        return is;
    }
};


#endif
