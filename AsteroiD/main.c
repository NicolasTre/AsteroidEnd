#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include <time.h>
#include "player.h"
#include "asteroid.h"

#define BULLET_SPEED 10.0f
#define NUM_ASTEROID 12
#define ASTEROID_RADIUS 64

sfRenderWindow* window;
Player player;
Bullet bullet;
Asteroid asteroid[NUM_ASTEROID];
sfSprite* playerSprite;
sfSprite* cursorSprite;
sfSprite* bulletSprite;
sfSprite* asteroidSprite[NUM_ASTEROID];
sfText* scoreText;

void initializeAsteroid()
{
    for (int i = 0; i < NUM_ASTEROID; ++i)
    {
        asteroid[i] = asteroidInit();
        asteroidSprite[i] = asteroidSpriteInit(window, &asteroid[i]);
    }
}

void respawnAsteroid(int index)
{
    asteroid[index] = asteroidInit();
    asteroidSprite[index] = asteroidSpriteInit(window, &asteroid[index]);
}



int main()
{
    
    sfVideoMode VideoMode = { 1080, 860, 64 };
    window = sfRenderWindow_create(VideoMode, "Asteroid", sfClose, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);
    sfEvent event;

    //creation of a sprite for the game over 
    sfTexture* gameOverTexture = sfTexture_createFromFile("sprites/gameover.png", NULL);
    sfSprite* gameOverSprite = sfSprite_create();
    sfSprite_setTexture(gameOverSprite, gameOverTexture, sfFalse);
    sfVector2f gameoverCord = { 0, 0 };
    sfSprite_setPosition(gameOverSprite, gameoverCord);

    //creation of a sprite for score
    sfFont* font = sfFont_createFromFile("font/arial.ttf");
    scoreText = sfText_create();
    sfText_setFont(scoreText, font);
    sfText_setCharacterSize(scoreText, 40);
    sfVector2f textPosition = { 10.0f, 10.0f };
    sfText_setPosition(scoreText, textPosition);

    //init
    player = playerInit();
    bullet = bulletInit();
    playerSprite = playerSpriteInit(window, &player);
    bulletSprite = bulletSpriteInit(window, &bullet);
    cursorSprite = cursorSpriteInit(window);
    sfVector2f cursorPosition = { 0.0f, 0.0f };

    while (sfRenderWindow_isOpen(window))
    {
        sfRenderWindow_clear(window, sfBlack);
        if (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfVector2i mPosition = sfMouse_getPositionRenderWindow(window);

//calcul of the rotation of player
        sfVector2f pPosition = sfSprite_getPosition(playerSprite);
        sfVector2f d = { mPosition.x - pPosition.x, mPosition.y - pPosition.y };
        float angle = atan2f(d.y, d.x) * 180 / 3.14f;
        sfVector2f pVelocity = { cos(angle * 3.14f / 180) * player.speed, sin(angle * 3.14f / 180) * player.speed };
        player.x += pVelocity.x;
        player.y += pVelocity.y;
        sfSprite_setRotation(playerSprite, angle);


//calculation of the pos of cursor 
        cursorPosition.x = (float)mPosition.x;
        cursorPosition.y = (float)mPosition.y;
        sfSprite_setPosition(cursorSprite, cursorPosition);


//calculation of the bullet pos
        if (bullet.IsShot)
        {
            sfVector2f bulletPosition = sfSprite_getPosition(bulletSprite);
            sfVector2f bulletDirection = { mPosition.x - bulletPosition.x, mPosition.y - bulletPosition.y };
            float length = sqrt(bulletDirection.x * bulletDirection.x + bulletDirection.y * bulletDirection.y);
            float bulletAngle = atan2f(bulletDirection.y, bulletDirection.x) * 180 / 3.14159276f;
            sfVector2f normalizedDirection = { bulletDirection.x / length, bulletDirection.y / length };
            sfVector2f bulletVelocity = { normalizedDirection.x * BULLET_SPEED, normalizedDirection.y * BULLET_SPEED };
            sfSprite_move(bulletSprite,bulletVelocity);
            sfSprite_setRotation(bulletSprite, bulletAngle);
            sfRenderWindow_drawSprite(window, bulletSprite, NULL);
        }


// respawn asteroid and initialisation of size for asteroid when he touch
        for (int i = 0; i < NUM_ASTEROID; ++i)
        {
            if (asteroid[i].size > 0)
            {
                sfRenderWindow_drawSprite(window, asteroidSprite[i], NULL);
                handleAsteroidMovementsCollisions(window, &asteroid[i], &player, &bullet, playerSprite, bulletSprite, asteroidSprite[i]);
            }
            else
            {
                respawnAsteroid(i);
            }
        }

        sfRenderWindow_drawSprite(window, playerSprite, NULL);
        handlePlayerInput(window, &player, playerSprite);
        sfSprite_move(playerSprite, pVelocity);
        sfRenderWindow_drawSprite(window, cursorSprite, NULL);
        handleShootingBullet(window, &player, &bullet, playerSprite, bulletSprite, cursorSprite);
        printScore(window, &player);
        sfRenderWindow_display(window);


        bool collision = checkCollision(&player, asteroid);

        if (collision) {
            // collision detected
            printf("Collision detected!\n");	
            sfRenderWindow_drawSprite(window, gameOverSprite, NULL);
            for (int i = 0; i < NUM_ASTEROID; ++i)
            {
                sfSprite_destroy(asteroidSprite[i]);
            }
        }
       else {
           //no collision detected
            printf("No collision detected.\n");
            printf("Distance entre le joueur et l'astéroïde : %.2f\n", sqrtf(powf((pPosition.x - asteroid->x), 2) + powf((pPosition.y - asteroid->y), 2)));
            printf("Rayon de l'asteroïde : %.2d\n", ASTEROID_RADIUS);
        }
    }

    return 0;

}

int printScore(sfRenderWindow* window, Player* player)
{
    char scoreString[50];
    snprintf(scoreString, sizeof(scoreString), "     Score: %d", player->score - 0);
    sfText_setString(scoreText, scoreString);
    sfRenderWindow_drawText(window, scoreText, NULL);
}


void cleanup()
{
    

    sfSprite_destroy(playerSprite);
    sfSprite_destroy(cursorSprite);
    sfSprite_destroy(bulletSprite);
    sfRenderWindow_destroy(window);
}