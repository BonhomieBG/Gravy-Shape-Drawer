// File name: IllegalTriangleException.h
// Description: Exception class representing an illegal triangle error.
// 
// Time Spent: About 25 mins
// Challenges: None
//
// Revision History
// Date: By: Action
// 11/16/2025 Created the IllegalTriangleException.h exception class
//

#ifndef ILLEGALTRIANGLEEXCEPTION_H
#define ILLEGALTRIANGLEEXCEPTION_H

#include <exception>
#include <string>
#include <sstream>

class IllegalTriangleException : public std::exception {
private:
    double sideA_{0.0}, sideB_{0.0}, sideC_{0.0};
    std::string message_;
public:
    IllegalTriangleException(const std::string& message, double a, double b, double c)
        : sideA_(a), sideB_(b), sideC_(c), message_(message) {}

    explicit IllegalTriangleException(const std::string& message)
        : message_(message) {}

    IllegalTriangleException ()
        : message_("The provided sides do not form a valid triangle.") {}

    virtual ~IllegalTriangleException() = default;

    double getSideA() const noexcept{ return sideA_; }
    double getSideB() const noexcept{ return sideB_; }
    double getSideC() const noexcept{ return sideC_; }

    const char* what() const noexcept override {
        return message_.c_str();
    }
};
    
#endif