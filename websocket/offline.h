#pragma once

#include "websocket.h"

int cp_init(int gameID);

pid_t cp_createPlayer(Player *p, int *id);

pid_t cp_sendPlayer(Player *p);

pid_t cp_sendNewBullet(Bullet *b);

pid_t cp_sendNewWall(Wall *w);

Player *cp_getEnemyInfo(int id);

Bullet *cp_getNewBullet(void);

Wall *cp_getNewWall(void);

pid_t cp_loadEnemies(Player *e);
