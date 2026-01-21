// File name: InvalidRadiusException.h
// Description: Exception class representing an invalid radius error.
// 
// Time Spent: About 10 mins
// Challenges: None
//
// Revision History
// Date: By: Action
// 11/16/2025 Created the InvalidRadiusException.h exception class
// Modification version of IllegalTriangleException.h

#ifndef INVALIDRADIUSEXCEPTION_H
#define INVALIDRADIUSEXCEPTION_H

#include <exception>
#include <string>
#include <sstream>

class InvalidRadiusException : public std::exception {
private:
    double radius_{0.0};
    std::string message_;

public:
    InvalidRadiusException(const std::string& message, double radius)
        : radius_(radius), message_(message) {}

    explicit InvalidRadiusException(const std::string& message)
        : message_(message) {}

    InvalidRadiusException ()
        : message_("The provided radius is invalid.") {}

    virtual ~InvalidRadiusException() = default;

    double getRadius() const noexcept { return radius_; }
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};
#endif