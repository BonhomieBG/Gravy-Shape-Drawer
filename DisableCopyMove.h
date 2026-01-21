#ifndef DISABLECOPYMOVE_H
#define DISABLECOPYMOVE_H

#include <cstddef>
#include <cstdint>
#include <memory>

// Macros for disable copy, move constructors and assignment operators.
// This is useful for class that need to take ownership of resources and should not be copied or moved.
// Copying or moving such objects could lead to resource leaks, double deletions, or other undefined behaviors.

#define DISABLE_COPY_MOVE(X) \
    X(const X &other)            = delete; \
    X(X &&other)                 = delete; \
    X &operator=(const X &other) = delete; \
    X &operator=(X &&other)      = delete;

// Disable copy and move for d_constructor as well
#define DISABLE_COPY_MOVE_DEFAULT(X) \
    X()                          = delete; \
    X(const X &other)            = delete; \
    X(X &&other)                 = delete; \
    X &operator=(const X &other) = delete; \
    X &operator=(X &&other)      = delete;


#endif // DISABLECOPYMOVE_H