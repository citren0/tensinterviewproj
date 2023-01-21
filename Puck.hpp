#include <iostream>
#include <vector>
#include <tuple>
#include <ctime>
#include <math.h>


/*
    TODO:
        - Move implementation out of the header file.
*/


// The geometric distance formula, put into a macro for easy use.
#define distFormula(x1, y1, x2, y2) (sqrt(pow(x2-x1, 2)) + sqrt(pow(y2-y1, 2)))


// Chose to make a location object to hold x and y values instead of using tuples.
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
        int arr[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
        

        PuckLibrary() {}


        /*
            Asynchronous work function, called when pucks reach the end of the track.
        */
        void Work(int puckNumber) {
            std::cout << "Puck " << puckNumber << " working...\n";
            pucks[puckNumber].hasWorked = true;
        }

        
        /*
            ELABORATE FURTHER -------------------------
            This function either moves the pucks as far forward as possible or to the ending/starting locations.
            Will return a false if more work needs to be done, or true if all work is finished.
        */
        bool Move(int puckNumber) {

            if (pucks[puckNumber].spot == numSpots) { // Puck is at the end, start working. Then, move the puck out of the parking spots.
                pucks[puckNumber].spot = -1;
                Work(puckNumber);

            } else if (pucks[puckNumber].spot == -1) { // Puck has finished working, move it back to the beginning.
                pucks[puckNumber].spot = 1;

            } else { // Move the puck as far forward as it will go without hitting another puck.
                // Since all pucks must traverse the spots and work in single file, if the puck at the front has finished working, then all of them must have.
                if(pucks[puckNumber].hasWorked == true) {
                    std::cout << "All pucks have finished working." << std::endl;
                    return true;
                }

                for(int i = pucks[puckNumber].spot+1; i < numSpots; i++) {
                    if(isSpotTaken(i) == true) {
                        std::cout << "Moving puck " << puckNumber << " to (" << parkingSpots[i-1].x << ", " << parkingSpots[i-1].y << ")" << std::endl;
                        pucks[puckNumber].spot = i-1;
                    }
                }
            }
            return false;
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

                // Create a variable to hold the pucks minimum distance to any given point, initialized with 1000, because no pucks can be that far away from any given spot.
                int minDistToPoint = 1000;

                // Loop through all parking spots.
                for (int spotIndex = 0; spotIndex < numSpots; spotIndex++) {
                    
                    // Calculate the distance from the puck and spot in question, then make sure its not taken.
                    // If the distance is less than the previous minimum, set the minimum and spot variables accordingly to reflect the new value.
                    int currDist = distFormula(pucks[puckIndex].x, pucks[puckIndex].y, parkingSpots[spotIndex].x, parkingSpots[spotIndex].y);

                    if (currDist < minDistToPoint && !isSpotTaken(spotIndex)) {
                        minDistToPoint = currDist;
                        //std::cout << "DEBUG: New min found for puck " << puckIndex << ", dist: " << minDistToPoint << ", spotindex: " << spotIndex << std::endl;
                        pucks[puckIndex].setSpot(spotIndex);
                    }

                }
                std::cout << "DEBUG: Final closest location for puck " << puckIndex << " is spot " << pucks[puckIndex].getSpot() << 
                                " at location " << parkingSpots[pucks[puckIndex].getSpot()].x << ", " << parkingSpots[pucks[puckIndex].getSpot()].y << std::endl;
                
                // Need to know the ordering of the pucks for printing purposes.
                arr[pucks[puckIndex].getSpot()] = puckIndex;
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

        void start() {
            int limit = 0;
            while(limit < 20) {
                for(int i = numPucks-1; i >= 0; i--) {
                    if(Move(i)) {
                        return;
                    }
                }
                limit++;
            }
        }

        
};