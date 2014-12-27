#define SPRITE_SIZE    32

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct game_sprites {
    int Sky;
    int Scenery;
    int Level;
    int Hud;
    int Player;
    bool loaded;
};

global_variable struct game_sprites Sprites;

global_variable game_rect spriteScenery[] = {
    { 0, 146, 128, 71 }, // cloud1
    { 0, 73 , 129, 71 }, // cloud2
    { 0, 0  , 129, 71 }  // cloud3
};

global_variable game_rect levelTiles[] = {
    { 648, 0, 70, 70 },  // grass
    { 504, 576, 70, 70}, // grassMid
    { 576, 864, 70, 70}, // grassCenter
};

global_variable game_rect hudLayout[] = {
    {230, 0  , 30 ,38}, // 0
    {196, 41 , 26 ,37}, // 1
    { 55, 98 , 32 ,38}, // 2
    {239, 80 , 28 ,38}, // 3
    {238, 122, 29 ,38}, // 4
    {238, 162, 28 ,38}, // 5
    {230, 40 , 30 ,38}, // 6
    {226, 206, 32 ,39}, // 7
    {192, 206, 32 ,40}, // 8
    {196, 0  , 32 ,39}, // 9
};

global_variable game_rect animations[] = {
    {365,  98, 69, 71}, // duck
    {  0, 196, 66, 92}, // front
    {438,   0, 69, 92}, // hurt
    {438,  93, 67, 94}, // jump
    { 67, 196, 66, 92}, // stand
};

global_variable game_rect walkAnimations[] = {
    {  0,  0, 72, 97 }, // walk01
    { 73,  0, 72, 97 }, // walk02
    {146,  0, 72, 97 }, // walk03
    {  0, 98, 72, 97 }, // walk04
    { 73, 98, 72, 97 }, // walk05
    {146, 98, 72, 97 }, // walk06
    {219,  0, 72, 97 }, // walk07
    {292,  0, 72, 97 }, // walk08
    {219, 98, 72, 97 }, // walk09
    {365,  0, 72, 97 }, // walk10
    {292, 98, 72, 97 }  // walk11
};

void LoadSprites() {
    if (!Sprites.loaded) {
        Sprites.Sky     = loadTextureFromFile("res/sky.png");
        Sprites.Scenery = loadTextureFromFile("res/scenery.png");
        Sprites.Level   = loadTextureFromFile("res/level.png");
        Sprites.Hud     = loadTextureFromFile("res/hud.png");
        Sprites.Player  = loadTextureFromFile("res/player.png");
        Sprites.loaded = true;
    }
}


void GameUpdateAndRender(game_offscreen_buffer *buffer) {
    LoadSprites();

    // the sky
    RenderSprite(Sprites.Sky, 0, 0);

    // a bunch of clouds
	game_rect cloud = spriteScenery[0];
    game_rect posCloud = cloud;
    posCloud.x = 100;
    posCloud.y = 100;
    RenderSprite(Sprites.Scenery, &cloud, &posCloud);

    // floor
    for (int i=0; i < 100; i++) {
        game_rect tilePosition = levelTiles[1];
        tilePosition.x = i*70;
        tilePosition.y = 670;
        RenderSprite(Sprites.Level, &levelTiles[1], &tilePosition); 

        // draw 4 lines below
        for (int j=1; j<4; j++) {
            tilePosition = levelTiles[2];
            tilePosition.x = i*70;
            tilePosition.y = 670 + (70*j);
            RenderSprite(Sprites.Level, &levelTiles[2], &tilePosition);
        }
    }

    // hud
    for (int i=0; i<3; i++) {
        game_rect hudPosition = hudLayout[i];
        // TODO: SCREEN_WIDHT is not constant since we're doing resizing of the window
        hudPosition.x = SCREEN_WIDTH - 50 - i*40;
        hudPosition.y = 10;
        RenderSprite(Sprites.Hud, &hudLayout[i], &hudPosition);
    }


    // player
    game_rect playerFrame = animations[4];
    game_rect playerPosition = animations[4];

    playerPosition.x = 200;
    playerPosition.y = SCREEN_HEIGHT - 190;
    // playerPosition.w = 64;
    // playerPosition.h = 64;

    RenderSprite( Sprites.Player, &playerFrame, &playerPosition);
}
