#include "offline.h"
#include <stdio.h>

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
    Player **list;
    int length;
    pthread_mutex_t *l_mut;
    pthread_mutex_t *p_mut[ENEMY_NUM + 1];
    struct timeval start_from;
} FieldPlayers;
FieldPlayers players;

int cp_init(int gameID, int *time) {
    if (gameID != 0) {
        fprintf(stderr, "something went wrong: cp_init\n");
        exit(1);
    }
    elapsed = time;
    return 0;
}

int cp_close(int gameID) {
    for (int i=0; i<players.length; i++) {
        free(players.list[i]);
    }
    return 0;
}

pid_t cp_createPlayer(Player *p, int *id) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        pthread_mutex_lock(players.l_mut);
        *id = players.length;
        players.list[players.length++] = p;
        if (players.length > ENEMY_NUM + 1) {
            fprintf(stderr, "WARNING: too many players registered in game\n");
            return -1;
        } else if (players.length == ENEMY_NUM + 1) {
            gettimeofday(&players.start_from, NULL);
            players.start_from.tv_sec += 2;
        }
        pthread_mutex_unlock(players.l_mut);
        printf("DEBUG: created player\n");
        return 0;
    } else {
        return pid;
    }
}

pid_t cp_sendPlayer(Player *p) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        pthread_mutex_lock(players.p_mut[p->id]);
        players.list[p->id] = p;
        pthread_mutex_unlock(players.p_mut[p->id]);
        return 0;
    } else {
        return pid;
    }
}

Player *cp_getEnemyInfo(int id) {
    return players.list[id];
}

Player **cp_getAllEnemyInfo(int player_id) {
    printf("戻り値には自分を指すものも返されます。\nidを参照して省いてください。\n");
    return players.list;
}

typedef struct _bullet_node{
    Bullet *b;
    int created_at;
    struct _bullet_node *next;
} BulletNode;
typedef struct {
    BulletNode *first, *last;
    pthread_mutex_t *mut;
} BulletList;
BulletList bulletlist[ENEMY_NUM + 1];

void cp_bullet_append(BulletList *list, Bullet *b) {
    pthread_mutex_lock(list->mut);
    BulletNode *new = (BulletNode *)malloc(sizeof(BulletNode));
    new->b = b;
    new->created_at = *elapsed;
    new->next = NULL;
    list->last->next = new;
    list->last = new;
    if (list->first == NULL) list->first = new;
    pthread_mutex_unlock(list->mut);
}
Bullet *cp_bullet_pop_front(BulletList *list) {
    if (list->first == NULL) {
        return NULL;
    }
    pthread_mutex_lock(list->mut);
    BulletNode *new = list->first;
    list->first = new->next;
    if (new->next == NULL) list->last = NULL;
    pthread_mutex_unlock(list->mut);
    Bullet *b = new->b;
    b->location.x = b->velocity.x * (*elapsed - new->created_at);
    b->location.y = b->velocity.y * (*elapsed - new->created_at);
    b->location.z = b->velocity.z * (*elapsed - new->created_at);
    free(new);
    return b;
}

pid_t cp_sendNewBullet(int player_id, Bullet *b) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        for (int i=0; i<players.length; i++) {
            if (i != player_id) {
                cp_bullet_append(&bulletlist[i], b);
            }
        }
        return 0;
    } else {
        return pid;
    }
}

Bullet *cp_getNewBullet(int player_id) {
    return cp_bullet_pop_front(&bulletlist[player_id]);
}

typedef struct _wall_node{
    Wall *w;
    int created_at;
    struct _wall_node *next;
} WallNode;
typedef struct {
    WallNode *first, *last;
    pthread_mutex_t *mut;
} WallList;
WallList walllist[ENEMY_NUM + 1];

void cp_wall_append(WallList *list, Wall *w) {
    pthread_mutex_lock(list->mut);
    WallNode *new = (WallNode *)malloc(sizeof(WallNode));
    new->w = w;
    new->created_at = *elapsed;
    new->next = NULL;
    list->last->next = new;
    list->last = new;
    if (list->first == NULL) list->first = new;
    pthread_mutex_unlock(list->mut);
}
Wall *cp_wall_pop_front(WallList *list) {
    if (list->first == NULL) {
        return NULL;
    }
    pthread_mutex_lock(list->mut);
    WallNode *new = list->first;
    list->first = new->next;
    if (new->next == NULL) list->last = NULL;
    pthread_mutex_unlock(list->mut);
    Wall *w = new->w;
    w->remain -= *elapsed - new->created_at;
    free(new);
    return w;
}

pid_t cp_sendNewWall(int player_id, Wall *w) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        for (int i=0; i<players.length; i++) {
            if (i != player_id) {
                cp_wall_append(&walllist[i], w);
            }
        }
        return 0;
    } else {
        return pid;
    }
}

Wall *cp_getNewWall(int player_id) {
    return cp_wall_pop_front(&walllist[player_id]);
}

#define MICRO 1000000
pid_t cp_loadEnemies(int player_id, Player *e[]) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        while (1) {
            if (players.length >= ENEMY_NUM + 1) {
                break;
            }
            sleep(1);
        }
        for (int i=0; i<ENEMY_NUM; i++) {
            e[i] = players.list[i + (i >= player_id)];
        }
        struct timeval current_time;
        gettimeofday(&current_time, NULL);
        current_time.tv_sec -= players.start_from.tv_sec;
        current_time.tv_usec -= players.start_from.tv_usec;
        if (current_time.tv_sec * MICRO + MICRO - current_time.tv_usec < 0) {
            fprintf(stderr, "not good connection with the server\n");
            return -1;
        } else {
            usleep(current_time.tv_sec * MICRO + MICRO - current_time.tv_usec);
        }
        return 0;
    } else {
        return pid;
    }
}
