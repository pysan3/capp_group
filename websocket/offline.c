#include "offline.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdatomic.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

int *elapsed;

typedef struct {
    Player *list[ENEMY_NUM + 1];
    int length;
    pthread_mutex_t l_mut;
    pthread_mutex_t p_mut[ENEMY_NUM + 1];
    struct timeval start_from;
} FieldPlayers;
FieldPlayers players;

typedef struct _bullet_node{
    Bullet *b;
    int created_at;
    struct _bullet_node *next;
} BulletNode;
typedef struct {
    BulletNode *first, *last;
    pthread_mutex_t mut;
} BulletList;
BulletList bulletlist[ENEMY_NUM + 1];

typedef struct _wall_node{
    Wall *w;
    int created_at;
    struct _wall_node *next;
} WallNode;
typedef struct {
    WallNode *first, *last;
    pthread_mutex_t mut;
} WallList;
WallList walllist[ENEMY_NUM + 1];

int cp_init(int gameID, int *time) {
    if (gameID != 0) {
        fprintf(stderr, "something went wrong: cp_init\n");
        exit(1);
    }
    elapsed = time;
    pthread_mutex_init(&players.l_mut, NULL);
    for (int i=0; i<ENEMY_NUM + 1; i++) {
        pthread_mutex_init(&players.p_mut[i], NULL);
        pthread_mutex_init(&bulletlist[i].mut, NULL);
        pthread_mutex_init(&walllist[i].mut, NULL);
    }
    return 0;
}

int cp_close(int gameID) {
    return 0;
}

typedef struct {
    Player *p;
    int *id;
} threatPlayer;
void cp_createPlayer_th(threatPlayer *tp) {
    pthread_mutex_lock(&players.l_mut);
    *tp->id = players.length;
    players.list[players.length] = tp->p;
    if (players.length > ENEMY_NUM) {
        fprintf(stderr, "WARNING: too many players registered in game\n");
        exit(EXIT_FAILURE);
    } else if (players.length == ENEMY_NUM) {
        gettimeofday(&players.start_from, NULL);
        players.start_from.tv_sec += 2;
    }
    players.length += 1;
    pthread_mutex_unlock(&players.l_mut);
    printf("DEBUG: created player %s, id: %d\n", tp->p->name, *tp->id);
    free(tp);
}
pthread_t cp_createPlayer(Player *p, int *id) {
    pthread_t tid;
    threatPlayer *tp = (threatPlayer *)malloc(sizeof(threatPlayer));
    tp->p = p;
    tp->id = id;
    pthread_create(&tid, NULL, cp_createPlayer_th, tp);
    return tid;
}

void cp_sendPlayer_th(Player *p) {
    pthread_mutex_lock(&players.p_mut[p->id]);
    players.list[p->id] = p;
    pthread_mutex_unlock(&players.p_mut[p->id]);
}
pthread_t cp_sendPlayer(Player *p) {
    pthread_t tid;
    pthread_create(&tid, NULL, cp_sendPlayer_th, p);
    return tid;
}

Player *cp_getEnemyInfo(int id) {
    return players.list[id];
}

Player **cp_getAllEnemyInfo(int player_id) {
    printf("戻り値には自分を指すものも返されます。\nidを参照して省いてください。\n");
    return players.list;
}

void cp_bullet_append(BulletList *list, Bullet *b) {
    pthread_mutex_lock(&list->mut);
    BulletNode *new = (BulletNode *)malloc(sizeof(BulletNode));
    new->b = b;
    new->created_at = *elapsed;
    new->next = NULL;
    list->last->next = new;
    list->last = new;
    if (list->first == NULL) list->first = new;
    pthread_mutex_unlock(&list->mut);
}

Bullet *cp_bullet_pop_front(BulletList *list) {
    if (list->first == NULL) {
        return NULL;
    }
    pthread_mutex_lock(&list->mut);
    BulletNode *new = list->first;
    list->first = new->next;
    if (new->next == NULL) list->last = NULL;
    pthread_mutex_unlock(&list->mut);
    Bullet *b = new->b;
    b->location.x = b->velocity.x * (*elapsed - new->created_at);
    b->location.y = b->velocity.y * (*elapsed - new->created_at);
    b->location.z = b->velocity.z * (*elapsed - new->created_at);
    free(new);
    return b;
}

typedef struct {
    int player_id;
    Bullet *b;
} playersBullet;
void cp_sendNewBullet_th(playersBullet *pb) {
    for (int i=0; i<players.length; i++) {
        if (i != pb->player_id) {
            cp_bullet_append(&bulletlist[i], pb->b);
        }
    }
    free(pb);
}
pthread_t cp_sendNewBullet(int player_id, Bullet *b) {
    pthread_t tid;
    playersBullet *pb = (playersBullet *)malloc(sizeof(playersBullet));
    pb->player_id = player_id;
    pb->b = b;
    pthread_create(&tid, NULL, cp_sendNewBullet_th, &pb);
    return tid;
}

Bullet *cp_getNewBullet(int player_id) {
    return cp_bullet_pop_front(&bulletlist[player_id]);
}

void cp_wall_append(WallList *list, Wall *w) {
    pthread_mutex_lock(&list->mut);
    WallNode *new = (WallNode *)malloc(sizeof(WallNode));
    new->w = w;
    new->created_at = *elapsed;
    new->next = NULL;
    list->last->next = new;
    list->last = new;
    if (list->first == NULL) list->first = new;
    pthread_mutex_unlock(&list->mut);
}

Wall *cp_wall_pop_front(WallList *list) {
    if (list->first == NULL) {
        return NULL;
    }
    pthread_mutex_lock(&list->mut);
    WallNode *new = list->first;
    list->first = new->next;
    if (new->next == NULL) list->last = NULL;
    pthread_mutex_unlock(&list->mut);
    Wall *w = new->w;
    w->remain -= *elapsed - new->created_at;
    free(new);
    return w;
}

typedef struct {
    int player_id;
    Wall *w;
} playersWall;
void cp_sendNewWall_th(playersWall *pw) {
    for (int i=0; i<players.length; i++) {
        if (i != pw->player_id) {
            cp_wall_append(&walllist[i], pw->w);
        }
    }
    free(pw);
}
pthread_t cp_sendNewWall(int player_id, Wall *w) {
    pthread_t tid;
    playersWall *pw = (playersWall *)malloc(sizeof(playersWall));
    pw->player_id = player_id;
    pw->w = w;
    pthread_create(&tid, NULL, cp_sendNewWall_th, pw);
    return tid;
}

Wall *cp_getNewWall(int player_id) {
    return cp_wall_pop_front(&walllist[player_id]);
}

#define MICRO 1000000
typedef struct {
    int player_id;
    Player **e;
} threadLoadEnemy;
void cp_loadEnemies_th(threadLoadEnemy *le) {
    while (1) {
        if (players.length >= ENEMY_NUM + 1) {
            break;
        }
        sleep(1);
    }
    if (le->e != NULL) {
        for (int i=0; i<ENEMY_NUM; i++) {
            le->e[i] = players.list[i + (i >= le->player_id)];
        }
    }
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    current_time.tv_sec -= players.start_from.tv_sec;
    current_time.tv_usec -= players.start_from.tv_usec;
    if (current_time.tv_sec * MICRO + MICRO - current_time.tv_usec > 0) {
        fprintf(stderr, "not good connection with the server\n");
        exit(EXIT_FAILURE);
    } else {
        usleep(- current_time.tv_sec * MICRO - (MICRO - current_time.tv_usec));
    }
    free(le);
}
pthread_t cp_loadEnemies(int player_id, Player **e) {
    pthread_t tid;
    threadLoadEnemy *le = (threadLoadEnemy *)malloc(sizeof(threadLoadEnemy));
    le->player_id = player_id;
    le->e = e;
    pthread_create(&tid, NULL, cp_loadEnemies_th, le);
    return tid;
}
