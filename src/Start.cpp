#include <iostream>
#include <thread>
#include <vector>

#include "Car.h"
#include "Bike.h"
#include "Street.h"
#include "Intersection.h"
#include "Graphics.h"


void createTrafficObjects_Cross(std::vector<std::shared_ptr<Street>> &streets, 
                                std::vector<std::shared_ptr<Intersection>> &intersections, 
                                std::vector<std::shared_ptr<Vehicle>> &vehicles, 
				std::string &filename, int nCars, int nBikes)
{
    // assign filename of corresponding city map
    filename = "../data/cross.jpg";

    // init traffic objects
    int nIntersections = 5;
    for (size_t ni = 0; ni < nIntersections; ni++)
    {
        intersections.push_back(std::make_shared<Intersection>(ni));
    }
    // position intersections in pixel coordinates
    intersections.at(0)->setPosition(650, 1275);	// bottom
    intersections.at(1)->setPosition(25, 650);		// left
    intersections.at(2)->setPosition(650, 25);		// top
    intersections.at(3)->setPosition(1275, 650);	// right
    intersections.at(4)->setPosition(650, 650);		// center

    // create streets and connect traffic objects
    int nStreets = 4;
    for (size_t ns = 0; ns < nStreets; ns++)
    {
        streets.push_back(std::make_shared<Street>(ns));
        streets.at(ns)->setInIntersection(intersections.at(ns));
        streets.at(ns)->setOutIntersection(intersections.at(4));
    }

    // add cars to streets
    for (size_t nv = 0; nv < nCars; nv++)
    {
        vehicles.push_back(std::make_shared<Car>(nv));
    }
    // add bikes to streets
    for (size_t nv = 0; nv < nBikes; nv++)
    {
        vehicles.push_back(std::make_shared<Bike>(nv));
    }
    // add origin and destination to each vehicle
    // car(s)
    vehicles.at(0)->setCurrentStreet(streets.at(0));
    vehicles.at(0)->setCurrentDestination(intersections.at(4));
    vehicles.at(1)->setCurrentStreet(streets.at(1));
    vehicles.at(1)->setCurrentDestination(intersections.at(4));
    vehicles.at(2)->setCurrentStreet(streets.at(3));
    vehicles.at(2)->setCurrentDestination(intersections.at(4));
    // bike(s)
    vehicles.at(3)->setCurrentStreet(streets.at(0));
    vehicles.at(3)->setCurrentDestination(intersections.at(4));
    vehicles.at(4)->setCurrentStreet(streets.at(1));
    vehicles.at(4)->setCurrentDestination(intersections.at(4));
}


/* Main function */
int main()
{
    /* PART 1 : Set up traffic objects */

    // create and connect intersections and streets
    std::vector<std::shared_ptr<Street>> streets;
    std::vector<std::shared_ptr<Intersection>> intersections;
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::string backgroundImg;
    int nCars = 3;
    int nBikes = 2;
    createTrafficObjects_Cross(streets, intersections, vehicles, backgroundImg, nCars, nBikes);

    /* PART 2 : Simulate traffic objects */

    // simulate intersection
    std::for_each(intersections.begin(), intersections.end(), [](std::shared_ptr<Intersection> &i) {
        i->simulate();
    });

    // simulate vehicles
    std::for_each(vehicles.begin(), vehicles.end(), [](std::shared_ptr<Vehicle> &v) {
        v->simulate();
    });

    /* PART 3 : Launch visualization */

    // add all objects into common vector
    std::vector<std::shared_ptr<TrafficObject>> trafficObjects;
    std::for_each(intersections.begin(), intersections.end(), [&trafficObjects](std::shared_ptr<Intersection> &intersection) {
        std::shared_ptr<TrafficObject> trafficObject = std::dynamic_pointer_cast<TrafficObject>(intersection);
        trafficObjects.push_back(trafficObject);
    });

    std::for_each(vehicles.begin(), vehicles.end(), [&trafficObjects](std::shared_ptr<Vehicle> &vehicles) {
        std::shared_ptr<TrafficObject> trafficObject = std::dynamic_pointer_cast<TrafficObject>(vehicles);
        trafficObjects.push_back(trafficObject);
    });

    // draw all objects in vector
    Graphics *graphics = new Graphics();
    graphics->setBgFilename(backgroundImg);
    graphics->setTrafficObjects(trafficObjects);
    graphics->simulate();
}
