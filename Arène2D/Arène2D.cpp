#include "Game.h"
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Pour le random des NPC
    Game game;
    game.run();
    return 0;
}