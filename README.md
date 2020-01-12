# V2V-Communication-At-Intersection
A smart intersection that regulates the traffic flow without traffic lights

This is the capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./capstone`.

## Chosen project
This project extends the Concurrent-Traffic-Simulator built in the fourth project of this Nanodegree. Here below I indicate the new features that I included.

- **Start.cpp** : It contains the main function. I modified the signature of ```createTrafficObjects_Cross``` so that it now takes two integers, one for the number of cars and one for the number of bikes in the simulation. ```Car``` and ```Bike``` are child classes of ```Vehicle```. Moreover, I changed the background image and updated the coordinates for intersections and streets.
- **Graphics.cpp** : I modified the implementation of ```drawTrafficObjects``` so that bike objects are visualized as squares, whereas car objects are visualized as circles.
- **TrafficObject.h** : I modified the enumeration ```ObjectType```. I replaced ```objectVehicle``` with ```objectCar``` and```objectBike```. Moreover, I modified the signature of the constructor that now takes an argument of type ```size_t``` which is saved in the protected variable ```_name```, which can be retrieved from outside with the new getter method ```getName```. 
- **TrafficObject.cpp** : The protected variable ```_name``` is initialized with an initializer list in the modified constructor.
- **Vehicle.h** : I modified the constructor signature (s. TrafficObject), made the method ```drive``` virtual, and added the methods ```GetCurrStreet```and ```GetCurrDestination```. Moreover, I made protected the members that were private.
- **Car.cpp** : ```_type``` and ```_speed``` are assigned the respective values. It implements ```drive```. I improved this function so that now a car drives on its own lane. 
- **Bike.cpp** : see **Car.cpp**, with the main difference that a bike drives on the very right side of the its own lane.
- **Intersection.h** : I added the public method ```permitEntryToVehicleFromTheRight```.
- **Intersection.cpp** : I modified the method ```processVehicleQueue```. It calls ```permitEntryToFirstInQueue``` if there is only one vehicle in the queue or if the intersection is not the central one. Otherwise, it calls ```permitEntryToVehicleFromTheRight```. Moreover, I implemented ```permitEntryToVehicleFromTheRight```: A vehicle coming from a street with a lower number (note: streets are numerated clockwise, with the street at the bottom being number zero) takes precedence because it comes from the right. The exception is if the second vehicle in the queue is coming from street named 0. In this case the other vehicle takes precedence. If one car and one bike come from the same street, the bike takes precedence. **These rules apply only to the first two vehicles in the queue at each call of this method.**

I also removed **TrafficLight.h** and **TrafficLight.cpp**, and ```TrafficLight``` objects from this program.
