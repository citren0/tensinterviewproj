#include "Puck.hpp"

using namespace std;

int main() {
    PuckLibrary pucks;
    pucks.Initialize();
    pucks.movePucksToSpots();
    pucks.closeGaps();
    pucks.start();
}