#include <SFML/Graphics.h>
#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player
{
    int IsAlive;
    float x;
    float y;
    float speed;
    float rotation;
    int score;

} Player;

typedef struct Bullet
{
    int IsShot;
    float x;
    float y;
    float rotation;
    sfVector2f velocity;
} Bullet;

Player playerInit();
Bullet bulletInit();
sfSprite* playerSpriteInit(sfRenderWindow* window, Player* player);
void handlePlayerInput(sfRenderWindow* window, Player* player, sfSprite* playerSprite);
sfSprite* cursorSpriteInit(sfRenderWindow* window);
sfSprite* bulletSpriteInit(sfRenderWindow* window, Bullet* bullet);
void handleShootingBullet(sfRenderWindow* window, Player* player, Bullet* bullet, sfSprite* playerSprite, sfSprite* bulletSprite, sfSprite* cursorSprite);

#endif
