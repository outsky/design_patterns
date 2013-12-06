#include "game.h"

int main() {
    game::instance()->prepare();
    game::instance()->run();

    return 0;
}
