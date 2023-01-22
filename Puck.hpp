#include <iostream>
#include <vector>
#include <tuple>
#include <ctime>
#include <math.h>


/*
    PUCK.HPP
    Header file for the Puck and PuckLibrary classes.
*/


// The geometric distance formula, put into a macro for easy use.
#define distFormula(x1, y1, x2, y2) (sqrt(pow(x2-x1, 2)) + sqrt(pow(y2-y1, 2)))


// Chose to make a location object to hold x and y values instead of using tuples, used only for the parkingSpot array.
class Location {
    public:
        int x;
        int y;

        Location(int x, int y) {
            this->x = x;
            this->y = y;
        }
};


/*
    This class is a representation of the puck object.
    It contains location, parking spot information, id, and if the puck has worked.
*/
class Puck {
    public:

        int id;
        bool hasWorked = false;

        /* Will contain the randomly initialized position values, before the parkingSpot is assigned. */
        int x;
        int y;

        /*
            The intention with this spot variable is to store a puck's current parking spot for its initialization.
            I will use the corresponding index of the parkingSpot vector to indicate which parking spot the puck is in.
            -1 means that the puck hasn't yet been moved to a parking spot.
            Starts at 0, goes up to 8 
            Is not used in favor of the track array once initialization is finished.
        */
        int spot = -1;
        
        Puck(int id) {
            this->id = id;
            hasWorked = false;
        }

        // Id getters and setters
        int getId() {
            return id;
        }

        void setId(int id) {
            this->id = id;
        }

        // Spot getters and setters
        int getSpot() {
            return spot;
        }

        void setSpot(int spot) {
            this->spot = spot;
        }

};

class PuckLibrary {
    public:

        // Holds all possible parking spots, in order of traversal.
        std::vector<Location> parkingSpots {
                                                Location(180, 60),
                                                Location(300, 60),
                                                Location(420, 60),
                                                Location(420, 180),
                                                Location(300, 180),
                                                Location(180, 180),
                                                Location(180, 300),
                                                Location(300, 300),
                                                Location(420, 300)
                                            };

        // Will hold the pucks when initialized.
        std::vector<Puck> pucks;

        int numPucks = -1;
        int maxPucks = 9;
        int numSpots = 9;

        // Holds order of the pucks, as they sit on the track. Initialized to -1, so we know when a spot is empty.
        // There are 10 positions because the 10th position is a holding spot for when the pucks are moving back to the start of the track.
        int track[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        

        PuckLibrary() {} // Unused

        // Main functions
        void Work(int puckNumber);
        int getPuckSpot(int puckNumber);
        bool Move(int puckNumber);
        bool isSpotTaken(int spotIndex);
        void closeGaps();
        void movePucksToSpots();
        void Initialize();
        void printPucks();
        void start();
  
};