#include "offline.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>

int *elapsed;
Player dead;

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

typedef struct _wall_node{
    Wall *w;
    int created_at;
    struct _wall_node *next;
} WallNode;
typedef struct {
    WallNode *first, *last;
    pthread_mutex_t mut;
} WallList;

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

BulletList bulletlist[ENEMY_NUM + 1];
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

void cp_sendPlayer_th(Player *p) {
    pthread_mutex_lock(&players.p_mut[p->id]);
    players.list[p->id] = p;
    pthread_mutex_unlock(&players.p_mut[p->id]);
}

Player *cp_getEnemyInfo(int id) {
    return players.list[id];
}

Player **cp_getAllEnemyInfo(int player_id) {
    printf("戻り値には自分を指すものも返されます。\nidを参照して省いてください。\n");
    return players.list;
}

void cp_sendNewBullet_th(playersBullet *pb) {
    for (int i=0; i<players.length; i++) {
        if (i != pb->player_id) {
            cp_bullet_append(&bulletlist[i], pb->b);
        }
    }
    free(pb);
}

Bullet *cp_getNewBullet(int player_id) {
    return cp_bullet_pop_front(&bulletlist[player_id]);
}

void cp_sendNewWall_th(playersWall *pw) {
    for (int i=0; i<players.length; i++) {
        if (i != pw->player_id) {
            cp_wall_append(&walllist[i], pw->w);
        }
    }
    free(pw);
}

Wall *cp_getNewWall(int player_id) {
    return cp_wall_pop_front(&walllist[player_id]);
}

void cp_loadEnemies_th(threadLoadEnemy *le) {
    while (players.length < ENEMY_NUM + 1) sleep(1);
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

void cp_dead_th(threatPlayer *tp) {
    cp_sendPlayer_th(tp->p);
    free(tp);
}
