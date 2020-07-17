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
    pthread_t tid;
    threatPlayer *tp = (threatPlayer *)malloc(sizeof(threatPlayer));
    tp->p = p;
    tp->id = id;
    pthread_create(&tid, NULL, isOnline ? multi_createPlayer_th : cp_createPlayer_th, tp);
    return tid;
}

pthread_t ws_sendPlayer(Player *p) {
    pthread_t tid;
    pthread_create(&tid, NULL, isOnline ? multi_sendPlayer_th : cp_sendPlayer_th, p);
    return tid;
}

pthread_t ws_sendNewBullet(int player_id, Bullet *b) {
    pthread_t tid;
    playersBullet *pb = (playersBullet *)malloc(sizeof(playersBullet));
    pb->player_id = player_id;
    pb->b = b;
    pthread_create(&tid, NULL, isOnline ? multi_sendNewBullet_th : cp_sendNewBullet_th, &pb);
    return tid;
}

pthread_t ws_sendNewWall(int player_id, Wall *w) {
    pthread_t tid;
    playersWall *pw = (playersWall *)malloc(sizeof(playersWall));
    pw->player_id = player_id;
    pw->w = w;
    pthread_create(&tid, NULL, isOnline ? multi_sendNewWall_th : cp_sendNewWall_th, pw);
    return tid;
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

pthread_t ws_loadEnemies(int player_id, Player **e) {
    pthread_t tid;
    threadLoadEnemy *le = (threadLoadEnemy *)malloc(sizeof(threadLoadEnemy));
    le->player_id = player_id;
    le->e = e;
    pthread_create(&tid, NULL, isOnline ? multi_loadEnemies_th : cp_loadEnemies_th, le);
    return tid;
}
