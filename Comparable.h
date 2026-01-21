#ifndef COMPARABLE_H
#define COMPARABLE_H

#include <string>
class Comparable {
public:
    virtual ~Comparable()= default; // Virtual destructor for proper cleanup
    virtual int compareTo(Comparable& other) const noexcept= 0; // Pure virtual method for comparison
};
// Need review

#endif // COMPARABLE_H