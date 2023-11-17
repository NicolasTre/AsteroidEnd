#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>  
#include <time.h>
#include "player.h"
#include <stdbool.h>

#ifndef ASTEROID_H
#define ASTEROID_H
#define ASTEROID_RADIUS 32 // size of the radius of asteroid
#define MAX_ASTEROID 20 // number of max asteroid in game 

typedef struct Asteroid
{
	float x;
	float y;
	float speed;
	int size;
	int hitCount;
	float r;
	int isAlive;
	float radius;
} Asteroid;

Asteroid asteroidInit();
sfSprite* asteroidSpriteInit(sfRenderWindow* window, Asteroid* asteroid);
void handleAsteroidMovementsCollisions(sfRenderWindow* window, Asteroid* asteroid, Player* player, Bullet* bullet, sfSprite* playerSprite, sfSprite* bulletSprite, sfSprite* asteroidSprite);
bool checkCollision(Player* player, Asteroid* asteroid);
#endif 