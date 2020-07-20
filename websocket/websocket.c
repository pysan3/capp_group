#include "websocket.h"
#include "online.h"
#include "offline.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int isOnline;

int ws_createNewGameID(void) {
    if (isOnline) return multi_createNewGameID();
    else {
        fprintf(stderr, "do not create new game ID on offline gamemode\n");
        exit(1);
    }
}

int ws_init(int gameID, int *time) {
    if (gameID != 0) {
        fprintf(stderr, "online functions are not created yet\n");
    }
    isOnline = gameID;
    return isOnline
    ? multi_init(isOnline, time)
    : cp_init(isOnline, time);
}

int ws_close(int gameID) {
    isOnline = gameID;
    return isOnline
    ? multi_close(isOnline)
    : cp_close(isOnline);
}

pthread_t ws_createPlayer(Player *p, int *id) {
    return isOnline
    ? multi_createPlayer(p, id)
    : cp_createPlayer(p, id);
}

pthread_t ws_sendPlayer(Player *p) {
    return isOnline
    ? multi_sendPlayer(p)
    : cp_sendPlayer(p);
}

pthread_t ws_sendNewBullet(int player_id, Bullet *b) {
    return isOnline
    ? multi_sendNewBullet(player_id, b)
    : cp_sendNewBullet(player_id, b);
}

pthread_t ws_sendNewWall(int player_id, Wall *w) {
    return isOnline
    ? multi_sendNewWall(player_id, w)
    : cp_sendNewWall(player_id, w);
}

Player *ws_getEnemyInfo(int id) {
    return isOnline
    ? multi_getEnemyInfo(id)
    : cp_getEnemyInfo(id);
}

Player **ws_getAllEnemyInfo(int player_id) {
    return isOnline
    ? multi_getAllEnemyInfo(player_id)
    : cp_getAllEnemyInfo(player_id);
}

Bullet *ws_getNewBullet(int player_id) {
    return isOnline
    ? multi_getNewBullet(player_id)
    : cp_getNewBullet(player_id);
}

Wall *ws_getNewWall(int player_id) {
    return isOnline
    ? multi_getNewWall(player_id)
    : cp_getNewWall(player_id);
}

pthread_t ws_loadEnemies(int player_id, Player *e[]) {
    return isOnline
    ? multi_loadEnemies(player_id, e)
    : cp_loadEnemies(player_id, e);
}
