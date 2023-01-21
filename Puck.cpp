#include "Puck.hpp"

using namespace std;

int main() {
    PuckLibrary pucks;
    pucks.Initialize();
    pucks.debugPrint();
    pucks.movePucksToSpots();
    pucks.start();
}