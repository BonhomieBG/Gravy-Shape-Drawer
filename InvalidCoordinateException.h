// File name: InvalidCoordinateException.h
// Description: Exception class representing an invalid coordinate error.
// 
// Time Spent: About 10 mins
// Challenges: None
//
// Revision History
// Date: By: Action
// 11/16/2025 Created the InvalidCoordinateException.h exception class
// Modification version of IllegalTriangleException.h

#ifndef INVALIDCOORDINATEEXCEPTION_H
#define INVALIDCOORDINATEEXCEPTION_H

#include <exception>
#include <string>
#include <sstream>

class InvalidCoordinateException : public std::exception {
private:
    double x_{0.0}, y_{0.0};
    std::string message_;

public:
    InvalidCoordinateException(const std::string& message, double x, double y)
        : x_(x), y_(y), message_(message) {}
    
    explicit InvalidCoordinateException(const std::string& message)
        : message_(message) {}

    InvalidCoordinateException ():
        message_("The provided coordinates are invalid.") {}

    virtual ~InvalidCoordinateException() = default;

    double getX() const noexcept { return x_; }
    double getY() const noexcept { return y_; }

    const char* what() const noexcept override {
        return message_.c_str();
    }
};
    
#endif