#include "header.h"
#include "Bullet/bullet.h"
#include "characterchange/charcaterchange.h"
#include "characters/characters.h"
#include "object/object.h"
#include "Player/player.h"
#include "Wall/wall.h"
#include "websocket/websocket.h"
#include "fpsControl/fpsControl.h"
#include "texture20/texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

GLfloat red[] = {1, 0, 0, 1};
GLfloat green[] = {0, 1, 0, 1};
GLfloat blue[] = {0, 0, 1, 1};
GLfloat white[] = {1, 1, 1, 1};
GLfloat gray[] = {0.5, 0.5, 0.5, 1};
GLfloat black[] = {0, 0, 0, 1};
GLfloat pos1[] = {100, 100, 300, 1};
// static const GLfloat lightpos[] = { 0.0, 0.0, 1.0, 0.0 };

void print_axes(void) {
    glPushMatrix();
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    glVertex3f(FIELD_MAX_X, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(FIELD_MAX_X, 0, FIELD_MAX_Z);
    glVertex3f(0, 0, FIELD_MAX_Z);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    glVertex3f(0, FIELD_MAX_Y, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(FIELD_MAX_X, FIELD_MAX_Y, 0);
    glVertex3f(FIELD_MAX_X, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    glVertex3f(0, 0, FIELD_MAX_Z);
    glVertex3f(0, 0, 0);
    glVertex3f(FIELD_MAX_X, 0, FIELD_MAX_Z);
    glVertex3f(FIELD_MAX_X, 0, 0);
    glEnd();
    glPopMatrix();
}

// void scene(void) {
//     glPushMatrix();

//     /* 法線マップのマッピング開始 */
//     glEnable(GL_TEXTURE_2D);

//     /* 正規化マップのマッピング開始 */
//     glActiveTexture(GL_TEXTURE1);
//     glEnable(GL_TEXTURE_CUBE_MAP);

//     /* 矩形を描く */
//     rectangle(FIELD_MAX_X, FIELD_MAX_Z, pos1);

//     /* 正規化マップのマッピング終了 */
//     glDisable(GL_TEXTURE_CUBE_MAP);

//     /* 法線マップのマッピング終了 */
//     glActiveTexture(GL_TEXTURE0);
//     glDisable(GL_TEXTURE_2D);

//     glPopMatrix();
// }

FieldInfo *f_info;
typedef struct {
    pthread_t tid;
    int index;
} ENEMY_THREAD_INFO;
ENEMY_THREAD_INFO *enemy_thread;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    // 地面の凹凸計算
    // scene();

    update_bullets(f_info->me->id);
    update_player();
    if (update_player() == EXIT_FAILURE) {
        printf("you lose\n");
        exit(EXIT_SUCCESS);
    }
    int enemies_hp = 0;
    for (int i=0; i<ENEMY_NUM; i++) {
        put_character(f_info->enemies[i]->c, &f_info->enemies[i]->location);
        draw_string(f_info->enemies[i]->name, &f_info->enemies[i]->location);
        enemies_hp += f_info->enemies[i]->hp > 0;
    }
    if (!enemies_hp) {
        printf("you won\n");
        exit(0);
    } else if (f_info->elapsed_time > 60 * FPS) {
        printf("draw\n");
        exit(0);
    }
    updata_wall(f_info->me->id);

    print_axes();

    draw_introdutcion();

    glPopMatrix();

    // 描画盤面を入れ替える
    glutSwapBuffers();

    glFlush();
}

void update_offline_enemy(int *index) {
    int enemy_id = *index;
    pthread_t tid;
    char name[100];
    sprintf(name, "enemy%d", enemy_id);
    Player *e = (Player *)malloc(sizeof(Player));
    player_init(e, rand() % (FIELD_MAX_X / 2) + (FIELD_MAX_X / 4), rand() % (FIELD_MAX_Z / 2) + (FIELD_MAX_Z / 4), name, rand() % 4);
    tid = ws_createPlayer(e, &e->id);
    pthread_join(tid, NULL);
    printf("enemy %d, id: %d, hp: %d\n", enemy_id, e->id, e->hp);
    tid = ws_loadEnemies(e->id, NULL);
    pthread_join(tid, NULL);
    while (1) {
        if (update_enemy_offline(enemy_id) == EXIT_FAILURE) { // dead
            printf("enemy: %s is dead\n", e->name);
            // ws_dead(enemy_id);
            break;
        }
        usleep(MICRO / FPS / 2);
    }
}

void game_init(void) {
    pthread_t tid;

    int user_input;
    do {
        printf("You want to play [0: offline, 1: online]? ");
        // TODO: testing
        scanf("%d", &user_input);
        // user_input = 0;
    } while (user_input != 0 && user_input != 1);
    if (user_input == 1) {
        printf("Enter GameID: [-1 to create new] ");
        scanf("%d", &user_input);
        if (user_input == -1) {
            f_info->isOnline = ws_createNewGameID();
            printf("received a new gameID from server: %d\n", f_info->isOnline);
            printf("tell your friends the gameID so you can play together!\n");
        } else {
            f_info->isOnline = user_input;
            printf("you set the GameID: %d\n", f_info->isOnline);
        }
    } else {
        f_info->isOnline = 0;
        printf("Starting a game offline\n");
    }

    player_fieldget(f_info);
    bullet_init(f_info->bullets, &f_info->isUpdated);
    wall_init(f_info->walls, &f_info->isUpdated);
    ws_init(f_info->isOnline, &f_info->elapsed_time, &f_info->isUpdated);

    // オフラインの場合に敵AIを作成
    if (!f_info->isOnline) {
        enemy_thread = (ENEMY_THREAD_INFO *)malloc(sizeof(ENEMY_THREAD_INFO) * ENEMY_NUM);
        for (int i=0; i<ENEMY_NUM; i++) {
            enemy_thread[i].index = i;
            pthread_create(&enemy_thread[i].tid, NULL, update_offline_enemy, &enemy_thread[i].index);
        }
    }

    // キャラ選択
    cc_makeme(f_info);

    tid = ws_createPlayer(f_info->me, &f_info->me->id);
    pthread_join(tid, NULL);
    tid = ws_loadEnemies(f_info->me->id, f_info->enemies);
    pthread_join(tid, NULL);
}

void idle(void) {
    fps_elapsed();
    glutPostRedisplay();
    // if (fps_elapsed()) {
    //     glutPostRedisplay();
    // }
}

void init(void) {
    // srand(0);
    // 背景色
    // glClearColor(1.0, 1.0, 1.0, 0.0);
    glClearColor(0.8, 0.8, 0.8, 0.0);
    // 隠面除去
    glEnable(GL_DEPTH_TEST);
    // 背面除去
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    glCullFace(GL_BACK);

    // 光源設定
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 80);

    // texture_init();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 16.0 / 9, 1.0, 250.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(FIELD_MAX_X / 2, FIELD_MAX_Y, FIELD_MAX_Z * sqrt(3), FIELD_MAX_X / 2, 0, FIELD_MAX_Z / 2, 0, 1, 0);
    // gluLookAt(2, 3, 7, 0, 0, 0, 0, 1, 0);

    f_info = (FieldInfo *)malloc(sizeof(FieldInfo));
    memset(f_info, 0, sizeof(FieldInfo));
    game_init();
    fps_init(&f_info->elapsed_time, &f_info->isUpdated);

    glutKeyboardFunc(myKeyboardFunc);
	glutKeyboardUpFunc(myKeyboardUpFunc);
	glutMouseFunc(myMouseFunc);

    glutIdleFunc(idle);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    // glutInitWindowPosition(0, 0);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Group A");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
