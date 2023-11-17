#include <SFML/Graphics.h>
#include "player.h"
#include "asteroid.h"

#define BULLET_SPEED 10.0f // define the speed of bullet


Player playerInit()
{
    Player p;
    p.IsAlive = 1;
    p.x = 960;
    p.y = 540;
    p.rotation = 0;
    p.speed = 0;
    p.score = 0;

    return p;
    
}

Bullet bulletInit()
{
    Bullet b;
    b.IsShot = 0;
    b.x = 0;
    b.y = 0;
    b.rotation = 0;
    return b;
}

sfSprite* playerSpriteInit(sfRenderWindow* window, Player* player)
{
    sfTexture* pTexture = sfTexture_createFromFile("sprites/player.png", NULL);
    sfSprite* pSprite = sfSprite_create();
    sfSprite_setTexture(pSprite, pTexture, sfFalse);
    sfVector2f pOrigin = { 32.0f, 32.0f };
    sfSprite_setOrigin(pSprite, pOrigin);
    sfVector2f playerCord = { player->x, player->y };
    sfSprite_setPosition(pSprite, playerCord);
    return pSprite;
}

void handlePlayerInput(sfRenderWindow* window, Player* player, sfSprite* playerSprite)
{
    // calculation of the rotation of player
    sfVector2i mPosition = sfMouse_getPosition(window);
    sfVector2f pPosition = sfSprite_getPosition(playerSprite);
    sfVector2f d = { mPosition.x - pPosition.x, mPosition.y - pPosition.y };
    float angle = atan2f(d.y, d.x) * 180 / 3.14159276f;

    if (player->IsAlive == 1)
    {
        if (sfKeyboard_isKeyPressed(sfKeyZ))
        {
            player->speed = 4;
        }
        else if (sfKeyboard_isKeyPressed(sfKeyS))
        {
            player->speed = -4;
        }
        else
        {
            player->speed = player->speed * 0.99;
        }

        // allows the asteroid to cross the map from top to bottom
        if (pPosition.y < -30)
        {
            pPosition.y = 1070;
        }
        if (pPosition.y > 1120)
        {
            pPosition.y = 0;
        }
        if (pPosition.x < -30)
        {
            pPosition.x = 1910;
        }
        if (pPosition.x > 1960)
        {
            pPosition.x = 0;
        }

        sfSprite_setPosition(playerSprite, pPosition);
    }
}

//creation the base of the sprite
sfSprite* cursorSpriteInit(sfRenderWindow* window)
{
    sfTexture* cursorBlueTexture = sfTexture_createFromFile("sprites/cursorBlue.png", NULL);
    sfSprite* cursorBlueSprite = sfSprite_create();
    sfSprite_setTexture(cursorBlueSprite, cursorBlueTexture, sfFalse);
    sfVector2f cOrigin = { 32.0f, 32.0f };
    sfSprite_setOrigin(cursorBlueSprite, cOrigin);
    sfSprite_setScale(cursorBlueSprite, (sfVector2f) { 0.6f, 0.6f });
    return cursorBlueSprite;
}


//creation the base for the bullet
sfSprite* bulletSpriteInit(sfRenderWindow* window, Bullet* bullet)
{
    sfTexture* bulletTexture = sfTexture_createFromFile("sprites/bullet.png", NULL);
    sfSprite* bulletSprite = sfSprite_create();
    sfSprite_setTexture(bulletSprite, bulletTexture, sfTrue);
    sfVector2f aOrigin = { 32.0f, 32.0f };
    sfSprite_setOrigin(bulletSprite, aOrigin);
    sfSprite_setScale(bulletSprite, (sfVector2f) { 0.6f, 0.6f });
    return bulletSprite;
}


void handleShootingBullet(sfRenderWindow* window, Player* player, Bullet* bullet, sfSprite* playerSprite, sfSprite* bulletSprite, sfSprite* cursorSprite)
{
    if (player->IsAlive == 1)
    {
        if (bullet->IsShot == 0)
        {
           
           
                bullet->IsShot = 1;
                sfVector2i mPosition = sfMouse_getPositionRenderWindow(window);
                sfVector2f pPosition = sfSprite_getPosition(playerSprite);
                sfVector2f d = { mPosition.x - pPosition.x, mPosition.y - pPosition.y };
                float length = sqrt(d.x * d.x + d.y * d.y);
                sfVector2f normalizedDirection = { d.x / length, d.y / length };
                sfVector2f bulletVelocity = { normalizedDirection.x * BULLET_SPEED, normalizedDirection.y * BULLET_SPEED };
                bullet->x = pPosition.x;
                bullet->y = pPosition.y;
                sfVector2f bulletPosition = { bullet->x, bullet->y };
                sfSprite_setPosition(bulletSprite, bulletPosition);
            
        }

    }
// colllision of bullet with the cursor
    sfFloatRect bulletBounds = sfSprite_getGlobalBounds(bulletSprite);
    sfFloatRect cursorBounds = sfSprite_getGlobalBounds(cursorSprite);
    if (sfFloatRect_intersects(&bulletBounds, &cursorBounds, NULL))
    {
        bullet->IsShot = 0;
    }


}



     