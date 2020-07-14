#pragma once

#include "websocket.h"

int multi_createNewGameID(void);

int multi_init(int gameID);

pid_t multi_createPlayer(Player *p, int *id);

pid_t multi_sendPlayer(Player *p);

pid_t multi_sendNewBullet(Bullet *b);

pid_t multi_sendNewWall(Wall *w);

Player *multi_getEnemyInfo(int id);

Bullet *multi_getNewBullet(void);

Wall *multi_getNewWall(void);

pid_t multi_loadEnemies(Player *e);
