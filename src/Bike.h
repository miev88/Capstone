#ifndef BIKE_H
#define BIKE_H

#include "Vehicle.h"

class Bike : public Vehicle
{
public:
    // constructor
    Bike(size_t name);

private:
    // typical behaviour methods
    void drive();
};

#endif
