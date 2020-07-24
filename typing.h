#pragma once

#include "characters/characters.h"

typedef struct {
    double x;
    double y;
    double z;
    double rotate;
} Coordinate;

#define PLAYER_HEIGHT 4

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
#define BULLET_SPEED 1.0
#define BULLET_DAMAGE 30

typedef struct {
    Coordinate location;
    Coordinate velocity;
    double damage;
    int player_id;
} Bullet;

#define WALL_HEIGHT 8

typedef struct {
    Coordinate location;
    int remain;
    int player_id;
} Wall;

#define FIELD_MAX_X 118
#define FIELD_MAX_Y 100
#define FIELD_MAX_Z 100

#define FPS 30.0

#define ENEMY_NUM 1

typedef struct {
    Player *me;
    Player *enemies[ENEMY_NUM];
    Player *cannon[100];
    Bullet *bullets[1000];
    Wall *walls[1000];
    int elapsed_time;
    int isUpdated;
    int isOnline;
} FieldInfo;
