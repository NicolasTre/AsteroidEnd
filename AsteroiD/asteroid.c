#include <SFML/Graphics.h>
#include "player.h"
#include "asteroid.h"

float random(int min, int max)
{
	return (float)(rand() % (max + 1 - min)) + min;
}

Asteroid asteroidInit()
{
	Asteroid asteroid;
	asteroid.x = rand() % 1920 + 100;
	asteroid.y = rand() % 1080 + 100;
	asteroid.speed = 3;
	asteroid.size = 2;
	asteroid.hitCount = 2;
	asteroid.r = random(0, 360);
	return asteroid;
}

sfSprite* asteroidSpriteInit(sfRenderWindow* window, Asteroid* asteroid)
{
//Sprite Creation
	sfTexture* asteroidTexture = sfTexture_createFromFile("sprites/asteroid.png", NULL);
	sfSprite* asteroidSprite = sfSprite_create();
	sfSprite_setTexture(asteroidSprite, asteroidTexture, sfFalse);
	sfVector2f sOrigin = { 32.0f, 32.0f };
	sfSprite_setOrigin(asteroidSprite, sOrigin);
	sfVector2f asteroidCord = { asteroid->x, asteroid->y };
	sfSprite_setPosition(asteroidSprite, asteroidCord);

//set scale when asteroid be touch
	if (asteroid->size == 2)
	{
		sfSprite_setScale(asteroidSprite, (sfVector2f) { 2.2f, 2.2f });
		return asteroidSprite;
	}
	else if (asteroid->size == 1)
	{
		sfSprite_setScale(asteroidSprite, (sfVector2f) { 1.4f, 1.4f });
		return asteroidSprite;
	}
}

void handleAsteroidMovementsCollisions(sfRenderWindow* window, Asteroid* asteroid, Player* player, Bullet* arrow, sfSprite* playerSprite, sfSprite* bulletSprite, sfSprite* asteroidSprite)
{
	sfVector2f asteroidPosition = sfSprite_getPosition(asteroidSprite);

//collision of bullet


	sfFloatRect bulletBounds = sfSprite_getGlobalBounds(bulletSprite);
	sfFloatRect asteroidBounds = sfSprite_getGlobalBounds(asteroidSprite);

	if (asteroid->hitCount > 0)
	{
		if (sfFloatRect_intersects(&bulletBounds, &asteroidBounds, NULL))
		{
			arrow->IsShot = 0;
			asteroid->hitCount--;
			if (asteroid->hitCount == 1)
			{
				player->score = player->score + 100;
			}
			if (asteroid->hitCount == 0)
			{
				asteroid->size = 0;
				player->score = player->score + 200;
			}
			else
			{
				sfVector2f newScale;
				sfVector2f newPosition = sfSprite_getPosition(asteroidSprite);

				if (asteroid->hitCount == 2)
				{
					newScale = (sfVector2f){ 2.2f, 2.2f };
					asteroid->speed = 4;
				}
				else if (asteroid->hitCount == 1)
				{
					newScale = (sfVector2f){ 1.4f, 1.4f };
					asteroid->speed = 6;
				}
				sfSprite_setScale(asteroidSprite, newScale);
				sfSprite_setPosition(asteroidSprite, newPosition);
			}
		}
	}


	if (asteroid->size > 0)
	{
		if (rand() % 100 == 0)
		{
			asteroid->r = random(0, 360);
		}

		asteroidPosition.x += cosf(asteroid->r * 3.14 / 180) * asteroid->speed;
		asteroidPosition.y += sinf(asteroid->r * 3.14 / 180) * asteroid->speed;

// allows the asteroid to cross the map from top to bottom
		if (asteroidPosition.y < 0)
		{
			asteroidPosition.y = 1080;
		}
		if (asteroidPosition.y > 1080)
		{
			asteroidPosition.y = 0;
		}
		if (asteroidPosition.x < 0)
		{
			asteroidPosition.x = 1920;
		}
		if (asteroidPosition.x > 1920)
		{
			asteroidPosition.x = 0;
		}

		sfSprite_setPosition(asteroidSprite, asteroidPosition);

	}

}

bool checkCollision(Player* player, Asteroid* asteroid) {
	// Calculation of distance between player and asteroid
	bool returnvalue = false;
	for (int i = 0; i < 12; i++) 
	{
		float dist = sqrtf(powf((player->x - asteroid[i].x), 2) + powf((player->y - asteroid[i].y), 2));
		if ( dist<= ASTEROID_RADIUS)

			return true;
	}
	return false;
}