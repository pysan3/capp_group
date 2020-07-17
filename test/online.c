#include "../header.h"

#include "../websocket/websocket.h"
#include "../Bullet/bullet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int elapsed;
pid_t enemy_process[ENEMY_NUM];

void kill_all_process(int sig) {
    printf("got SIGINT\n");
    for (int i=0; i<ENEMY_NUM; i++) {
        kill(enemy_process[i], SIGINT);
        printf("killed process %d\n", i + 1);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    pthread_t tid;
    FieldInfo f_info;
    f_info.elapsed_time = 10;
    if (argc == 2) {
        f_info.isOnline = atoi(argv[1]);
        printf("gameID is set from command line args as %d\n", f_info.isOnline);
    } else {
        printf("you did not set gameID\n");
        f_info.isOnline = ws_createNewGameID();
        printf("received a new gameID from server: %d\n", f_info.isOnline);
        printf("tell your friends the gameID so you can play together!\n");
    }

    // テスト用ダミー敵のための新規プロセスを生成
    int child_id = 0;
    for (int i=0; i<ENEMY_NUM; i++) {
        if ((enemy_process[i] = fork()) == 0) {
            child_id = i + 1;
            break;
        }
    }
    // 親プロセスがkillされたときに子もすべて殺すよう設定
    if (child_id == 0) {
        signal(SIGINT, kill_all_process);
    }

    // ここからが実際の処理
    ws_init(f_info.isOnline, &f_info.elapsed_time);

    Player me = {0, {0, 0, 0}, 150, "", 0, 0, DORAEMON};
    {
        char player_name[100];
        sprintf(player_name, "player %c", 'A' + child_id);
        strncpy(me.name, player_name, strlen(player_name));
    }
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
    ws_close(f_info.isOnline);
    if (child_id == 0) {
        for (int i=0; i<ENEMY_NUM; i++) {
            waitpid(enemy_process[i], NULL, 0);
            printf("process %d ended\n", i + 1);
        }
    }

    return 0;
}