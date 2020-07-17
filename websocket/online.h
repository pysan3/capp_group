#pragma once

#include "websocket.h"

int multi_createNewGameID(void);

int multi_init(int gameID, int *time);

int multi_close(int gameID);

void multi_createPlayer_th(threatPlayer *tp);

void multi_sendPlayer_th(Player *p);

void multi_sendNewBullet_th(playersBullet *pb);

void multi_sendNewWall_th(playersWall *pw);

Player *multi_getEnemyInfo(int id);

Player **multi_getAllEnemyInfo(int player_id);

Bullet *multi_getNewBullet(int player_id);

Wall *multi_getNewWall(int player_id);

void multi_loadEnemies_th(threadLoadEnemy *le);
