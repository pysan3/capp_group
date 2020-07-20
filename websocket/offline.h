#pragma once

#include "websocket.h"

int cp_init(int gameID, int *time);

int cp_close(int gameID);

pthread_t cp_createPlayer(Player *p, int *id);

pthread_t cp_sendPlayer(Player *p);

pthread_t cp_sendNewBullet(int player_id, Bullet *b);

pthread_t cp_sendNewWall(int player_id, Wall *w);

Player *cp_getEnemyInfo(int id);

Player **cp_getAllEnemyInfo(int player_id);

Bullet *cp_getNewBullet(int player_id);

Wall *cp_getNewWall(int player_id);

pthread_t cp_loadEnemies(int player_id, Player *e[]);
