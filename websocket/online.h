#pragma once

#include "websocket.h"

int multi_createNewGameID(void);

int multi_init(int gameID, int *time);

int multi_close(int gameID);

pid_t multi_createPlayer(Player *p, int *id);

pid_t multi_sendPlayer(Player *p);

pid_t multi_sendNewBullet(int player_id, Bullet *b);

pid_t multi_sendNewWall(int player_id, Wall *w);

Player *multi_getEnemyInfo(int id);

Player **multi_getAllEnemyInfo(int player_id);

Bullet *multi_getNewBullet(int player_id);

Wall *multi_getNewWall(int player_id);

pid_t multi_loadEnemies(int player_id, Player *e[]);
