#pragma once

#include "characters/characters.h"

typedef struct {
    double x;
    double y;
    double z;
} Coordinate;

#define PLAYER_HEIGHT 10

typedef struct {
    int id;
    Coordinate location;
    int hp;
    char name[100];
    int sleep_bullet;
    int sleep_wall;
    Character c;
} Player;

#define BULLET_RADIUS 1

typedef struct {
    Coordinate location;
    Coordinate velocity;
    double damage;
} Bullet;

#define WALL_HEIGHT 10

typedef struct {
    Coordinate location;
    int remain;
} Wall;

#define FIELD_MAX_X 100
#define FIELD_MAX_Y 100
#define FIELD_MAX_Z 100

#define ENEMY_NUM 1

typedef struct {
<<<<<<< HEAD
    Player me;
    Player enemies[ENEMY_NUM];
    unsigned long long elapsed_time;
=======
    Player *me;
    Player *enemies[ENEMY_NUM];
    Player *cannon[100];
    Bullet *bullets[1000];
    Wall *walls[1000];
    int elapsed_time;
    int isOnline;
>>>>>>> 92a75cb56bf5a3bef501b0037be25b93d9ff3813
} FieldInfo;
