#include <iostream>
#include <vector>
#include <tuple>
#include <ctime>
#include <math.h>


// The geometric distance formula, put into a macro for easy use.
#define distFormula(x1, y1, x2, y2) (sqrt(pow(x2-x1, 2)) + sqrt(pow(y2-y1, 2)))

class Location {
    public:
        int x;
        int y;

        Location(int x, int y) {
            this->x = x;
            this->y = y;
        }
};

class Puck {
    public:

        int id;
        bool hasWorked;

        /* Will contain the randomly initialized position values, before the parkingSpot is assigned. */
        int x;
        int y;

        /* The intention with this spot variable is to store a puck's current parking spot.
           I will use the corresponding index of the parkingSpot vector to indicate which parking spot the puck is in.
           -1 means that the puck hasn't yet been moved to a parking spot.
           Starts at 0, goes up to 8 */
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

        // hasWorked getters and setters
        bool getHasWorked() {
            return hasWorked;
        }

        void setHasWorked(int hasWorked) {
            this->hasWorked = hasWorked;
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

        std::vector<Puck> pucks;

        int numPucks = -1;
        int maxPucks = 9;
        int numSpots = 9;
        

        PuckLibrary() {}


        /*
            Asynchronous work function, called when pucks reach the end of the track.
        */
        void Work(int puckNumber) {
            std::cout << "Puck " << puckNumber << " working...\n"; 
        }

        
        /*
            ELABORATE FURTHER -------------------------
            This function either moves the pucks as far forward as possible or to the ending/starting locations.
        */
        void Move(int puckNumber) {

            if (pucks[puckNumber].spot == numSpots) { // Puck is at the end, start working. Then, move the puck out of the parking spots.
                Work(puckNumber);
                pucks[puckNumber].spot = -1;

            } else if (pucks[puckNumber].spot == -1) { // Puck has finished working, move it back to the beginning.
                pucks[puckNumber].spot = 1;

            } else { // Move the puck as far forward as it will go without hitting another puck.
                // TODO --------------------------------------------------------------------------------------------------
            }
        }


        /*
            Used to find if a parking spot is already occupied.
        */
        bool isSpotTaken(int spotIndex) {
            for(int i = 0; i < numPucks; i++) {
                if(pucks[i].spot == spotIndex) {
                    return true;
                }
            }
            return false;
        }


        /*
            This function is only intended to be called once, after the pucks are initialized with their random position values.
            It will iterate through the pucks[] array and use the distance formula to find the closest parking spot for each puck in order, while avoiding parking spots already taken.
            This is accomplished in O(n^2) time complexity.
        */
        void movePucksToSpots() {

            // Loop through each puck, in the order they were instantiated.
            for (int puckIndex = 0; puckIndex < numPucks; puckIndex++) {

                // Create a variable to hold the pucks minimum distance to any given point.
                int minDistToPoint;

                // Loop through all parking spots.
                for (int spotIndex = 0; spotIndex < numSpots; spotIndex++) {

                    // Establish an initial value for the minimum.
                    if(spotIndex == 0) {
                        minDistToPoint = distFormula(pucks[puckIndex].x, pucks[puckIndex].y, parkingSpots[spotIndex].x, parkingSpots[spotIndex].y);
                        std::cout << "DEBUG: New min found for puck " << puckIndex << ", dist: " << minDistToPoint << ", spotindex: " << spotIndex << std::endl;
                        pucks[puckIndex].setSpot(spotIndex);

                    } else {
                        //Calculate the distance from the puck and spot in question, then make sure its not taken. If the distance is less than the previous minimum, set the minimum and spot variables accordingly.
                        int currDist = distFormula(pucks[puckIndex].x, pucks[puckIndex].y, parkingSpots[spotIndex].x, parkingSpots[spotIndex].y);

                        if (currDist < minDistToPoint && !isSpotTaken(spotIndex)) {
                            minDistToPoint = currDist;
                            std::cout << "DEBUG: New min found for puck " << puckIndex << ", dist: " << minDistToPoint << ", spotindex: " << spotIndex << std::endl;
                            pucks[puckIndex].setSpot(spotIndex);
                        }

                    }
                }
                std::cout << "DEBUG: Final closest location for puck " << puckIndex << " is spot " << pucks[puckIndex].getSpot() << 
                                " at location " << parkingSpots[pucks[puckIndex].getSpot()].x << ", " << parkingSpots[pucks[puckIndex].getSpot()].y << std::endl;
            }
        }


        /*
            Initializes a random number, 1-9, of pucks, then gives them all random locations on the map.
        */
        void Initialize() {

            srand(time(NULL));

            // This will be the number of pucks generated
            int randSize = rand() % maxPucks + 1;

            for (int i = 0; i < randSize; i++) {

                // Generate a new puck with id = i and random positions between 0-480
                Puck newPuck(i);

                int xPosition = rand() % 480;
                int yPosition = rand() % 480;
                newPuck.x = xPosition;
                newPuck.y = yPosition;

                pucks.push_back(newPuck);
            }
            numPucks = randSize;

        }

        /*
            Prints all puck's location and id for debugging purposes.
        */
        void debugPrint() {
            for (int i = 0; i < numPucks; i++) {
                std::cout << "DEBUG: Puck " << i << ": x " << pucks[i].x << " y " << pucks[i].y << std::endl;
            }
        }


        /*
            This function is intended to print the pucks and track in an easily understandable way.
        */
        void printPucks() {

            // This array will store the puck[] array indeces sorted by their spot positions for easier printing.
            int arr[maxPucks];

            // Initialize arr with -1, so we can tell when a spot is empty.
            for (int i = 0; i < maxPucks; i++) {
                arr[i] = -1;
            }

            // Populate the arr[] with pucks, in their proper locations
            for (int i = 0; i < numPucks; i++) {
                if (pucks[i].spot == -1) {
                    std::cout << "ERROR: Pucks must first be assigned to their spots before printing." << std::endl;
                    return;
                }

                // Here, you can see I am storing the index to the puck array into the arr array respective of its spot.
                arr[pucks[i].spot] = i;
            }

            std::cout << "START ->";
            for (int i = 0; i < maxPucks; i++) {
                if (arr[i] == -1) {
                    std::cout << " EMPTY ->";
                    continue;
                }

                std::cout << " FILLED (Puck: " << pucks[arr[i]].getId() << ") ->";
            }
            
            std::cout << " END" << std::endl;

        }

        
};