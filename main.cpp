#include <iostream>
#include "game.hpp"

int main() {
    Game *game = new Game(1920, 1080);
    for (int i = 0; i < 100; ++i) {
        game->GetFrameBuffer();
    }
    return 0;
}