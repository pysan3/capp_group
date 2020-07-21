#pragma once

#include "../typing.h"

#define MICRO 1000000

typedef struct {
    Player *p;
    int *id;
} threatPlayer;

typedef struct {
    int player_id;
    Bullet *b;
} playersBullet;

typedef struct {
    int player_id;
    Wall *w;
} playersWall;

typedef struct {
    int player_id;
    Player **e;
} threadLoadEnemy;
