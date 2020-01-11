#include <iostream>
#include <random>
#include "Street.h"
#include "Intersection.h"
#include "Vehicle.h"

Vehicle::Vehicle(size_t name) : TrafficObject(name)
{
    _currStreet = nullptr;
    _posStreet = 0.0;
    _type = ObjectType::objectVehicle;
    _speed = 400; // m/s
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

// virtual function which is executed in a thread
void Vehicle::drive()
{
    // print id of the current thread
    std::unique_lock<std::mutex> lck(_mtx);
    std::cout << "Vehicle #" << _id << "::drive: thread id = " << std::this_thread::get_id() << std::endl;
    lck.unlock();

    // initalize variables
    bool hasEnteredIntersection = false;
    double cycleDuration = 1; // duration of a single simulation cycle in ms
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= cycleDuration)
        {
            // update position with a constant velocity motion model
            _posStreet += _speed * timeSinceLastUpdate / 1000;

            // compute completion rate of current street
            double completion = _posStreet / _currStreet->getLength();

            // compute current pixel position on street based on driving direction
            std::shared_ptr<Intersection> i1, i2;
            i2 = _currDestination;
            i1 = i2->getID() == _currStreet->getInIntersection()->getID() ? _currStreet->getOutIntersection() : _currStreet->getInIntersection();

            double x1, y1, x2, y2, xv, yv, dx, dy;
            i1->getPosition(x1, y1);		// get coordinates Intersection-origin
            i2->getPosition(x2, y2);		// get coordinates Intersection-destination

	    // make vehicle drive in its lane
	    int delta = 50;
	    if (_currStreet->getName() == 0) {
	       // if moving from bottom to center
	       if (_currDestination->getName() == 4)  		x1 += delta;	
	       // if moving from center to bottom
               else if (_currDestination->getName() == 0)	x1 -= delta;
	    }
	    else if (_currStreet->getName() == 2) {
	       // if moving from top to center
	       if (_currDestination->getName() == 4)  		x1 -= delta;	
	       // if moving from center to top
               else if (_currDestination->getName() == 2)	x1 += delta;
	    }
	    else if (_currStreet->getName() == 1) {
	       // if moving from left to center
	       if (_currDestination->getName() == 4)  		y1 += delta;
	       // if moving from center to left
               else if (_currDestination->getName() == 1)	y1 -= delta;
	    }
	    else if (_currStreet->getName() == 3) {
	       // if moving from right to center
	       if (_currDestination->getName() == 4)  		y1 -= delta;
	       // if moving from center to right
               else if (_currDestination->getName() == 3)	y1 += delta;
	    }

	    // new position based on line equation in parameter form
	    dx = x2 - x1;
            dy = y2 - y1;
            xv = x1 + completion * dx;
	    yv = y1 + completion * dy;
            this->setPosition(xv, yv);

            // check whether halting position in front of destination has been reached
            if (completion >= 0.7 && !hasEnteredIntersection)
            {
                // request entry to the current intersection (using async)
                auto ftrEntryGranted = std::async(&Intersection::addVehicleToQueue, _currDestination, get_shared_this());

                // wait until entry has been granted
                ftrEntryGranted.get();

                // slow down and set intersection flag
                _speed /= 10.0;
                hasEnteredIntersection = true;
            }

            // check whether intersection has been crossed
            if (completion >= 1.0 && hasEnteredIntersection)
            {
                // choose next street and destination
                std::vector<std::shared_ptr<Street>> streetOptions = _currDestination->queryStreets(_currStreet);
                std::shared_ptr<Street> nextStreet;
                if (streetOptions.size() > 0)
                {
		    //std::cout << '\n' << "V_name: " << this->getName() << " - S_name: " << _currStreet->getName() << '\n';
		    if (_currStreet->getName() == 0)
	            {
			for (auto street : streetOptions) 
			{
			   if (street->getName() == 2)	{ nextStreet = street; }
 		 	}
		    }
		    else if (_currStreet->getName() == 1)
	            {
			for (auto street : streetOptions) 
			{
			   if (street->getName() == 0)	{ nextStreet = street; }
 		 	}
		    }
		    else if (_currStreet->getName() == 2)
	            {
			for (auto street : streetOptions) 
			{
			   if (street->getName() == 0)	{ nextStreet = street; }
 		 	}
		    }
		    else if (_currStreet->getName() == 3)
	            {
			for (auto street : streetOptions) 
			{
			   if (street->getName() == 0)	{ nextStreet = street; }
 		 	}
		    }
		    else 
		    {
			// pick one street at random and query intersection to enter this street
		        std::random_device rd;
		        std::mt19937 eng(rd());
		        std::uniform_int_distribution<> distr(0, streetOptions.size() - 1);
		        nextStreet = streetOptions.at(distr(eng));
 		    }
		}
                else
                {
                    // this street is a dead-end, so drive back the same way
                    nextStreet = _currStreet;
                }
                
                // pick the one intersection at which the vehicle is currently not
                std::shared_ptr<Intersection> nextIntersection = nextStreet->getInIntersection()->getID() == _currDestination->getID() ? nextStreet->getOutIntersection() : nextStreet->getInIntersection(); 

                // send signal to intersection that vehicle has left the intersection
                _currDestination->vehicleHasLeft(get_shared_this());

                // assign new street and destination
                this->setCurrentDestination(nextIntersection);
                this->setCurrentStreet(nextStreet);

                // reset speed and intersection flag
                _speed *= 10.0;
                hasEnteredIntersection = false;
            }

            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();
        }
    } // eof simulation loop
}
