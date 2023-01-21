#include <iostream>
#include <vector>
#include <tuple>

class Puck {
    public:
        int id;
        bool hasWorked;

        /* Will contain the randomly initialized position values, before the parkingSpot is assigned. */
        int x, y;

        /* The intention with this spot variable is to store a puck's current parking spot in it's object. I will use the corresponding index of parkingSpot. */
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

        // Position setters and getters

        int getX() {
            return x;
        }

        void setX(int x) {
            this->x = x;
        }

        int getY() {
            return y;
        }

        void setY(int y) {
            this->y = y;
        }

        

};

class PuckLibrary {
    public:
        std::vector<std::tuple<int, int>> parkingSpots {
                                                std::make_tuple(180, 60), // Start
                                                std::make_tuple(300, 60),
                                                std::make_tuple(420, 60),
                                                std::make_tuple(420, 180),
                                                std::make_tuple(300, 180),
                                                std::make_tuple(180, 180),
                                                std::make_tuple(180, 300),
                                                std::make_tuple(300, 300),
                                                std::make_tuple(420, 300)  // End
                                                };

        std::vector<Puck> pucks;

        int numPucks = -1;
        int maxPucks = 9;
        
        PuckLibrary() {}

        void Initialize() {

            srand(time(NULL));

            int randSize = rand() % maxPucks + 1;
            for(int i = 0; i < randSize; i++) {
                Puck newPuck(i+1);
                pucks.push_back(newPuck);
            }
            numPucks = randSize;

        }

        void printPucks() {

            // This array will store the order of the pucks for printing.
            int arr[maxPucks];

            // Initialize arr with -1, so we can tell when a spot is empty.
            for(int i = 0; i < maxPucks; i++) {
                arr[i] = -1;
            }

            for(int i = 0; i < numPucks; i++) {
                if(pucks[i].spot == -1) {
                    std::cout << "Pucks must first be moved to their spots before printing." << std::endl;
                    return;
                }

                arr[pucks[i].spot] = i;
            }

            std::cout << "START ->";
            for(int i = 0; i < maxPucks; i++) {
                if(arr[i] == -1) {
                    std::cout << " EMPTY ->";
                    continue;
                }

                std::cout << " FILLED (Puck: " << pucks[arr[i]].getId() << ") ->";
            }
            
            std::cout << " END"

        }

        
};