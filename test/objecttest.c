#include "header.h"
#include "object/object.h"
#include <stdlib.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

GLfloat red[] = {1, 0, 0, 1};
GLfloat green[] = {0, 1, 0, 1};
GLfloat blue[] = {0, 0, 1, 1};
GLfloat white[] = {1, 1, 1, 1};
GLfloat gray[] = {0.5, 0.5, 0.5, 1};
GLfloat black[] = {0, 0, 0, 1};

void print_axes(void) {
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glBegin(GL_LINES);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    glVertex3f(5, 0, 0);
    glVertex3f(0, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    glVertex3f(0, 5, 0);
    glVertex3f(0, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    glVertex3f(0, 0, 5);
    glVertex3f(0, 0, 0);
    glEnd();
    glPopMatrix();
}


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 回転するにはここのコメントを外す
    glRotatef(0.5, 0, 1, 0);

    // ここにdoraemonを追加した
    // draw_character(DORAEMON);
    Coordinate try = {0,0,0};
    put_character(LOTSO,&try);
    print_axes();
    // 描画盤面を入れ替える
    glutSwapBuffers();
}


void init(void) {
    srand(0);
    // 背景色
    // glClearColor(1.0, 1.0, 1.0, 0.0);
    glClearColor(0, 0, 0, 0.0);
    // 隠面除去
    glEnable(GL_DEPTH_TEST);
    // 背面除去
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    glCullFace(GL_BACK);

    // 光源設定
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat pos1[] = {100, 100, 300, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, gray);
    glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
    glLightfv(GL_LIGHT0, GL_SPECULAR, gray);
    glMaterialf(GL_FRONT, GL_SHININESS, 80);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(7, 3, 2, 0, 0, 0, 0, 1.0, 0);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Group A");
    glutDisplayFunc(display);
    glutIdleFunc(glutPostRedisplay);
    init();
    glutMainLoop();

    return 0;
}
