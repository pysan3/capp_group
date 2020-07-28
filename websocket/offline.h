#pragma once

#include "websocket.h"

int cp_init(int gameID, int *time);

int cp_close(int gameID);

void cp_createPlayer_th(threatPlayer *tp);

void cp_sendPlayer_th(Player *p);

void cp_sendNewBullet_th(playersBullet *pb);

void cp_sendNewWall_th(playersWall *pw);

Player *cp_getEnemyInfo(int id);

Player **cp_getAllEnemyInfo(int player_id);

Bullet *cp_getNewBullet(int player_id);

Wall *cp_getNewWall(int player_id);

void cp_loadEnemies_th(threadLoadEnemy *le);

void cp_dead_th(threatPlayer *tp);
