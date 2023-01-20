#include <iostream>
#include <vector>
#include <tuple>

class Puck {
    public:
        int id;
        bool hasWorked;
        
        Puck(int id) {
            this->id = id;
            hasWorked = false;
        }

};

class PuckLibrary {
    public:
        std::vector<std::tuple<int, int>> parkingSpots {
                                                std::make_tuple(180, 60),
                                                std::make_tuple(300, 60),
                                                std::make_tuple(420, 60),
                                                std::make_tuple(420, 180),
                                                std::make_tuple(300, 180),
                                                std::make_tuple(180, 180),
                                                std::make_tuple(180, 300),
                                                std::make_tuple(300, 300),
                                                std::make_tuple(420, 300)
                                                };

        std::vector<Puck> pucks;
        
        PuckLibrary() {}

        void Initialize() {
            srand(time(NULL));

            int randSize = rand() % 9 + 1;
            for(int i = 0; i < randSize; i++) {
                Puck newPuck(i+1);
                pucks.push_back(newPuck);
            }

            for(int i = 0; i < randSize; i++) {
                std::cout << pucks[i].id << std::endl;
            }
        }

        
};