#include <iostream>
#include <vector>
#include <tuple>
#include <ctime>
#include <math.h>


/*
    Most of the algorithms in this file use the assumption that for a queue to move forward, the person in front must move first, then the person behind them, etc.
    These algorithms use the same assumption to move pucks forward, that the puck in front must first move, followed by the one behind it, etc.
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
        

        PuckLibrary() {}


        /*
            Asynchronous work function, called when pucks reach the end of the track.
        */
        void Work(int puckNumber) {
            std::cout << "Puck " << puckNumber << " working...\n";
            pucks[puckNumber].hasWorked = true;
        }

        /*
            Returns the current parking spot of any given puck.
            O(n) time complexity.
        */
        int getPuckSpot(int puckNumber) {
            for(int i = 0; i < 10; i++) {
                if(track[i] == puckNumber) {
                    return i;
                }
            }
            return -1;
        }

        
        /*
            This function will move pucks, make them work, and reset them if they are done working.
            It will return false if there is more work to be done, and true if all pucks have finished working.
            O(n) time complexity.
        */
        bool Move(int puckNumber) {

            if(puckNumber == -1) {
                // Move may be called upon spots in the track which don't currently have a puck in them.
                return false;
            }

            if (track[8] == puckNumber) {
                // Puck is at the end, move it off the track, check if all pucks are finished, then start working if not.
                track[8] = -1;
                track[9] = puckNumber;

                // Since all pucks must traverse the spots and work in single file, if the puck at the front has finished working, then all of them must have.
                if(pucks[puckNumber].hasWorked == true) {
                    std::cout << "All pucks have finished working." << std::endl;
                    return true;
                }

                Work(puckNumber);
                printPucks();

            } else if (track[9] == puckNumber) {
                // Puck has finished working, move it back to the beginning.
                std::cout << "Puck " << puckNumber << " has been moved to the start." << std::endl;

                track[0] = puckNumber;
                track[9] = -1;
                printPucks();
                // This puck must be moved once more to free space for the next working puck.
                Move(puckNumber);
                printPucks();

            } else {
                // Puck is somewhere in the middle of the track, move it as far forward as it will go without hitting another puck.
                for(int i = getPuckSpot(puckNumber)+1; i <= numSpots; i++) {
                    // Start searching for its next spot right in front of its current location

                    if(track[i] != -1 || i == 9) {
                        // If the spot in question is filled, or past the end, the correct spot for this puck is the previous one.
                        track[getPuckSpot(puckNumber)] = -1;
                        std::cout << "Moving puck " << puckNumber << " to (" << parkingSpots[i-1].x << ", " << parkingSpots[i-1].y << ")" << std::endl;
                        track[i-1] = puckNumber;
                        return false;
                    }

                }

                printPucks();
            }
            return false;
        }


        /*
            Used to find if a parking spot is already occupied.
            O(1) time complexity.
        */
        bool isSpotTaken(int spotIndex) {
            if(track[spotIndex] != -1) {
                return true;
            }
            return false;
        }


        /*
            This function serves to close all gaps in the track.
            It does this by moving the first puck as far forward as possible, then the one behind it, etc. Until the end of the track.
            O(n^2) time complexity.
        */
        void closeGaps() {

            // The general algorithm here is to move from the front of the queue backwards, selecting a puck, then move forward to find the furthest the puck can go before colliding with another or the end.
            for (int i = 7; i >= 0; i--) {

                // If the spot is empty, we can ignore it, because there is no puck to move forward there.
                if(track[i] == -1) {
                    continue;
                }

                // We have found a puck. Now we will move forwards through the track, finding the furthest forward position to move the puck to without crossing another puck or going past the end of the track.
                for(int f = i+1; f <= 9; f++) {

                    if(track[i+1] != -1) {
                        break;
                    }

                    // If the spot isnt empty, then we can move the puck right up behind that spot
                    // Or if we reached the end without hitting another puck, we move there
                    if(track[f] != -1 || f == 9) {

                        // Swap locations then move to the next puck.
                        track[f-1] = track[i];
                        track[i] = -1;
                        break;
                    }
                
                }
            }
            std::cout << "Positions now that gaps are closed: " << std::endl;
            printPucks();
        }


        /*
            This function is only intended to be called once, after the pucks are initialized with their random position values.
            It will iterate through the pucks[] array and use the distance formula to find the closest parking spot for each puck in order, while avoiding parking spots already taken.
            O(n^2) time complexity.
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
                        pucks[puckIndex].setSpot(spotIndex);
                    }

                }
                
                // Need to know the ordering of the pucks for printing purposes.
                track[pucks[puckIndex].getSpot()] = puckIndex;
            }

            std::cout << "Position once pucks are moved to starting locations: " << std::endl;
            printPucks();
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

                std::cout << "Puck " << i << " initialized at position (" << newPuck.x << ", " << newPuck.y << ")" << std::endl;

                pucks.push_back(newPuck);
            }
            numPucks = randSize;

        }


        /*
            This function is intended to print the pucks and track in an easily understandable way.
        */
        void printPucks() {

            std::cout << "START ->";
            for (int i = 0; i < maxPucks; i++) {
                if (track[i] == -1) {
                    std::cout << " EMPTY ->";
                    continue;
                }

                std::cout << " FILLED (Puck: " << pucks[track[i]].getId() << ") ->";
            }
            
            std::cout << " END" << std::endl << std::endl;

        }


        /*
            This function starts the process of moving the pucks and making them work, and will automatically exit when they are all finished.
        */
        void start() {
            // Will loop until all pucks are finished.
            while(true) {
                // Pucks are moved from the front to the back.
                for(int i = 9; i >= 0; i--) {
                    // Call move, if it returns true, all pucks have finished working.
                    if(Move(track[i])) {
                        return;
                    }
                }
            }
        }

        
};