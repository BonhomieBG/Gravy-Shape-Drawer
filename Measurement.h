#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <iostream>

class Measurement {
public:
  enum Size {
    Milli = 1;
    Centi = 10;
    Deci = 100;
    Normal  = 1000;
    Deca= 10000;
    Hecto = 100000;
    Kilo = 1000000;
   }
  enum Kind {
    Meter;
    
    }

#endif 