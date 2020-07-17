#include "header.h"

#include "websocket/websocket.h"
#include "Bullet/bullet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int elapsed;

void update_offline_enemy(int *index) {
    int i = *index;
    char name[100];
    sprintf(name, "enemy%d", i);
    Player e = {0, {i, 0, i}, 100, "", 0, 0, DORAEMON};
    strncpy(e.name, name, strlen(name));
    pthread_t tid = ws_createPlayer(&e, &e.id);
    pthread_join(tid, NULL);
    printf("enemy %d, id: %d, hp: %d\n", i, e.id, e.hp);
    tid = ws_loadEnemies(e.id, NULL);
    pthread_join(tid, NULL);
    while (1) {
        Coordinate v = {
            (rand() + 1.0) / (RAND_MAX + 2.0),
            0,
            (rand() + 1.0) / (RAND_MAX + 2.0)
        };
        e.location.x += v.x;
        e.location.y += v.y;
        e.location.z += v.z;
        Coordinate hitSpace[4];
        double width = 0.5, depth = 0.7;
        for (int i=0; i<4; i++) {
            hitSpace[i].x = ((i < 2) * 2 - 1) * width;
            hitSpace[i].z = ((i & 1) * 2 - 1) * depth;
        }
        // e.hp -= bullet_hit(&hitSpace);
        e.hp -= 3;
        sleep(1);
        // printf("%d %d %s %d %d\n", e.id, e.hp, e.name, e.sleep_bullet, e.sleep_wall);
        ws_sendPlayer(&e);
        if (e.hp <= 0) {
            printf("%s (id: %d) is dead\n", e.name, e.id);
            break;
        }
    }
}

int main(void) {
    pthread_t tid;
    FieldInfo f_info;
    f_info.elapsed_time = 10;
    f_info.isOnline = 0;
    ws_init(f_info.isOnline, &f_info.elapsed_time);

    pthread_t enemy_threads[ENEMY_NUM];
    int enemy_index[ENEMY_NUM];
    for (int i=0; i<ENEMY_NUM; i++) {
        enemy_index[i] = i;
        pthread_create(&enemy_threads[i], NULL, update_offline_enemy, &enemy_index[i]);
    }

    Player me = {0, {0, 0, 0}, 150, "me", 0, 0, DORAEMON};
    f_info.me = &me;
    tid = ws_createPlayer(f_info.me, &f_info.me->id);
    pthread_join(tid, NULL);
    printf("my id: %d\n", f_info.me->id);

    tid = ws_loadEnemies(f_info.me->id, f_info.enemies);
    pthread_join(tid, NULL);

    int bullet_length = 0;
    while (1) {
        Bullet *newBullet;
        while ((newBullet = ws_getNewBullet(f_info.me->id)) != NULL) {
            f_info.bullets[bullet_length++] = newBullet;
        }
        for (int i=0; i<bullet_length; i++) {
            f_info.bullets[i]->location.x += f_info.bullets[i]->velocity.x;
            f_info.bullets[i]->location.y += f_info.bullets[i]->velocity.y;
            f_info.bullets[i]->location.z += f_info.bullets[i]->velocity.z;
        }
        Coordinate v = {
            (rand() + 1.0) / (RAND_MAX + 2.0),
            0,
            (rand() + 1.0) / (RAND_MAX + 2.0)
        };
        f_info.me->location.x += v.x;
        f_info.me->location.y += v.y;
        f_info.me->location.z += v.z;
        f_info.me->hp -= 5;
        tid = ws_sendPlayer(f_info.me);
        {
            int enemy_hp = 0;
            for (int i=0; i<ENEMY_NUM; i++) {
                enemy_hp += f_info.enemies[i]->hp > 0;
            }
            if (enemy_hp == 0) {
                printf("you won\n");
                break;
            }
        }
        if (f_info.me->hp <= 0) {
            printf("I'm dead\n");
            break;
        }
        f_info.elapsed_time += 1;
        pthread_join(tid, NULL);
        sleep(1);
    }
    for (int i=0; i<ENEMY_NUM; i++) {
        pthread_kill(enemy_threads[i], 0);
    }
    ws_close(0);

    return 0;
}