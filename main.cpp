#include "Game.h"
#include "Resources.h"


int main()
{
    Resources::loadTextures({
        "source/textures/wood.png",
        "source/textures/bluestone.png",
        "source/textures/colorstone.png",
        "source/textures/greystone.png", 
        "source/textures/purplestone.png", 
        "source/textures/redbrick.png" });

    Resources::loadFloorTexture("source/textures/redbrick.png");

    Game game;

    game.run();

    return 0;
}