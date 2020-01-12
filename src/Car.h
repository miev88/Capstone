#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"

class Car : public Vehicle
{
public:
    // constructor
    Car(size_t name);

private:
    // typical behaviour methods
    void drive();
};

#endif
