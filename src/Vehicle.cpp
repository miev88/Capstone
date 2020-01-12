#include "Intersection.h"
#include "Vehicle.h"

Vehicle::Vehicle(size_t name) : TrafficObject(name)
{
    _currStreet = nullptr;
    _posStreet = 0.0;
    _speed = 0.0;
}

void Vehicle::setCurrentDestination(std::shared_ptr<Intersection> destination)
{
    // update destination
    _currDestination = destination;

    // reset simulation parameters
    _posStreet = 0.0;
}

void Vehicle::simulate()
{
    // launch drive function in a thread
    threads.emplace_back(std::thread(&Vehicle::drive, this));
}
