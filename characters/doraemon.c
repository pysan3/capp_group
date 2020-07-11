#define _USE_MATH_DEFINES

#include "../header.h"
#include "characters.h"
#include <stdlib.h>
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

typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Coord3d;

double dot(Coord3d a, Coord3d b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Coord3d rotate(Coord3d a, double x, double y, double z) {
    Coord3d mat[] = {
        (Coord3d){cos(z)*cos(y), cos(z)*sin(y)*sin(x) - sin(z)*cos(x), cos(z)*sin(y)*cos(x) + sin(z)*sin(x)},
        (Coord3d){sin(z)*cos(y), sin(z)*sin(y)*sin(x) + cos(z)*cos(x), sin(z)*sin(y)*cos(x) - cos(z)*sin(x)},
        (Coord3d){-sin(y), cos(y)*sin(x), cos(y)*cos(x)}
    };
    return (Coord3d){
        dot(mat[0], a),
        dot(mat[1], a),
        dot(mat[2], a),
    };
}

void glRotate(double x, double y, double z) {
    glRotatef(x * 180 / M_PI, 1, 0, 0);
    glRotatef(y * 180 / M_PI, 0, 1, 0);
    glRotatef(z * 180 / M_PI, 0, 0, 1);
}

void oval(double a, double b) {
    glBegin(GL_POLYGON);
    for (int i=0; i<100; i++) {
        glVertex3d(0, a * cos(M_PI * 2 * i / 100), b * sin(M_PI * 2 * i / 100));
    }
    glEnd();
}

void eye(int side) {
    glPushMatrix();
    glRotate(0, -M_PI / 8, side * M_PI / 16);
    glTranslatef(1.005, 0, 0);
    glRotate(0, -0.08, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    oval(0.2, 0.3);
    glPushMatrix();
    glTranslatef(0.01, side * -0.13, -0.03);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
    oval(0.07, 0.1);
    glPopMatrix();
    glPopMatrix();
}

void beard(int side) {
    glPushMatrix();
    glTranslatef(1, side * 0.2, -0.2);
    glRotate(0, M_PI / 16, 0);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(0, side * 0.4, 0);
    glVertex3d(0, 0, 0.1);
    glVertex3d(0, side * 0.4, 0.2);
    glVertex3d(0, 0, -0.1);
    glVertex3d(0, side * 0.4, -0.2);
    glEnd();
    glPopMatrix();
}

void face(void) {
    // 青いところ
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    glutSolidSphere(1, 30, 30);

    glPushMatrix();
    glTranslatef(0.12, 0, -0.05);
    // 中の白いところ
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glutSolidSphere(0.92, 30, 30);
    // 人中
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
    glBegin(GL_LINE_STRIP);
    for (int i=0; i<=45; i++) {
        Coord3d new = rotate((Coord3d){0.93,0,0}, 0, i*M_PI/180.0, 0);
        glVertex3d(new.x, new.y, new.z);
    }
    glEnd();
    // 口
    glBegin(GL_LINE_STRIP);
    for (int i=-40; i<=40; i++) {
        Coord3d new = rotate((Coord3d){0.93,0,0}, 0, sqrt(45*45-i*i)*M_PI/180, i*M_PI/180);
        glVertex3d(new.x, new.y, new.z);
    }
    glEnd();
    glPopMatrix();

    // ひげ
    beard(1);
    beard(-1);

    // 目
    eye(1);
    eye(-1);

    // 鼻
    glPushMatrix();
    glTranslatef(1.1, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    glutSolidSphere(0.1, 30, 30);
    glPopMatrix();
}

void leg_rec(double from, double to, int steps, double length) {
    glPushMatrix();
    int cumul = steps * (steps + 1) / 2;
    double delta_r = (to - from) / cumul;
    double delta_z = length / steps;
    double radius = from;
    for (int i=steps; i>0; i--) {
        double next_r = radius + delta_r * i;
        glBegin(GL_QUAD_STRIP);
        for (int j=30; j>=0; j--) {
        glColor3fv(blue);
            glVertex3d(radius * cos(M_PI * j / 15), radius * sin(M_PI * j / 15), 0);
            glVertex3d(next_r * cos(M_PI * j / 15), next_r * sin(M_PI * j / 15), delta_z);
        }
        glEnd();
        glTranslatef(0, 0, delta_z);
        radius = next_r;
    }
    glPopMatrix();
}
void leg(int side, double length, double width) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    glPushMatrix();
    int vert_sepa = 30;
    double min_r = width - 0.05;
    double max_r = width + 0.05;
    glTranslatef(0, side * width, 0);
    glPushMatrix();
    glRotate(M_PI, 0, 0);
    leg_rec(min_r, max_r, vert_sepa, length/2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, -length);
    leg_rec(min_r, max_r, vert_sepa, length/2);
    glPopMatrix();
    glPopMatrix();
}

void front_back_rec(double from, double to, int steps, double length, double width) {
    glPushMatrix();
    int cumul = steps * (steps + 1) / 2;
    double delta_r = (to - from) / cumul;
    double delta_z = length / steps;
    double radius = from;
    double z = 0;
    glBegin(GL_QUAD_STRIP);
    for (int i=steps; i>0; i--) {
        glVertex3d(radius, width, z);
        glVertex3d(radius, -width, z);
        radius += delta_r * i;
        z += delta_z;
    }
    glEnd();
    glPopMatrix();
}
void front_back(int side, double length, double width) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    glPushMatrix();
    int vert_sepa = 30;
    double min_r = width - 0.05;
    double max_r = width + 0.05;
    glRotate(M_PI, 0, M_PI * (side > 0));
    front_back_rec(min_r, max_r, vert_sepa, length/2, width);
    glPopMatrix();
}

void pocket(double max_r) {
    glPushMatrix();

    glPushMatrix();
    glTranslatef(max_r, 0, -0.7);
    glRotate(0, -M_PI / 64, 0);
    GLfloat new[] = {0.3, 0.3, 0.3, 1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(0, 0, 0);
    double r = 0.5;
    for (int i=0; i<=30; i++) {
        glVertex3d(0, r * cos(M_PI * i / 15), r * sin(M_PI * i / 15));
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(max_r+0.03, 0, -0.7);
    glRotate(M_PI, -M_PI / 128, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(0, 0, 0);
    r = 0.45;
    for (int i=0; i<=30; i++) {
        glVertex3d(0, r * cos(M_PI * i / 30), r * sin(M_PI * i / 30));
    }
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

void neck(void) {
    glPushMatrix();
    glTranslatef(0, 0, -0.03);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    glutSolidTorus(0.1, 0.6, 30, 30);
    glPopMatrix();
}

void body(void) {
    double length = 1.7;
    double width = 0.35;
    // 脚
    leg(1, length, width);
    leg(-1, length, width);

    // お腹と背中
    front_back(1, length, width);
    front_back(-1, length, width);

    // お腹の白いところとポケット
    pocket(width + 0.05);
    // 首輪
    neck();

    // 左手
    glPushMatrix();
    glTranslatef(0, 1.4, 0.16);
    glRotate(M_PI * 2 / 3, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    leg_rec(0.15, 0.23, 30, 1);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glutSolidSphere(0.25, 30, 30);
    glPopMatrix();

    // 右手
    glPushMatrix();
    glTranslatef(0, -1.2, -1);
    glRotate(-M_PI * 1 / 4, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    leg_rec(0.15, 0.23, 30, 1);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glutSolidSphere(0.25, 30, 30);
    glPopMatrix();

    // 足
    glPushMatrix();
    glTranslatef(0, width, -length);
    glScalef(1.25, 1, 0.7);
    glutSolidSphere(0.35, 30, 30);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -width, -length);
    glScalef(1.25, 1, 0.7);
    glutSolidSphere(0.35, 30, 30);
    glPopMatrix();
}

void draw_doraemon(void) {
    glPushMatrix();

    // 顔
    glPushMatrix();
    glTranslatef(0, 0, 0.85);
    face();
    glPopMatrix();

    // 体
    glPushMatrix();
    body();
    glPopMatrix();

    glPopMatrix();
}