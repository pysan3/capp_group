#include "websocket.h"
#include "online.h"
#include "offline.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int isOnline;

int ws_createNewGameID(void) {
    if (isOnline) return multi_createNewGameID();
    else {
        fprintf(stderr, "do not create new game ID on offline gamemode\n");
        exit(1);
    }
}

int ws_init(int gameID) {
    isOnline = gameID;
    return isOnline
    ? multi_init(isOnline)
    : cp_init(isOnline);
}

pid_t ws_createPlayer(Player *p, int *id) {
    return isOnline
    ? multi_createPlayer(p, id)
    : cp_createPlayer(p, id);
}

pid_t ws_sendPlayer(Player *p) {
    return isOnline
    ? multi_sendPlayer(p)
    : cp_sendPlayer(p);
}

pid_t ws_sendNewBullet(Bullet *b) {
    return isOnline
    ? multi_sendNewBullet(b)
    : cp_sendNewBullet(b);
}

pid_t ws_sendNewWall(Wall *w) {
    return isOnline
    ? multi_sendNewWall(w)
    : cp_sendNewWall(w);
}

Player *ws_getEnemyInfo(int id) {
    return isOnline
    ? multi_getEnemyInfo(id)
    : cp_getEnemyInfo(id);
}

Bullet *ws_getNewBullet(void) {
    return isOnline
    ? multi_getNewBullet()
    : cp_getNewBullet();
}

Wall *ws_getNewWall(void) {
    return isOnline
    ? multi_getNewWall()
    : cp_getNewWall();
}

pid_t ws_loadEnemies(Player *e) {
    return isOnline
    ? multi_loadEnemies(e)
    : cp_loadEnemies(e);
}
