#include "offline.h"

int cp_init(int gameID) {
    return 0;
}

pid_t cp_createPlayer(Player *p, int *id) {
    return 0;
}

pid_t cp_sendPlayer(Player *p) {
    return 0;
}

pid_t cp_sendNewBullet(Bullet *b) {
    return 0;
}

pid_t cp_sendNewWall(Wall *w) {
    return 0;
}

Player *cp_getEnemyInfo(int id) {
    return NULL;
}

Bullet *cp_getNewBullet(void) {
    return NULL;
}

Wall *cp_getNewWall(void) {
    return NULL;
}

pid_t cp_loadEnemies(Player *e) {
    return 0;
}
