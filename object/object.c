#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

enum COLOR { WHITE, BROWN, ORANGE, GRAY, BLACK };
GLfloat color[][4] = {
	{ 1.0, 1.0, 1.0, 1.0 },
	{ 0.5, 0.4, 0.35, 1.0 },
	{ 0.99, 0.6, 0.4, 1.0 },
	{ 0.8, 0.8, 0.8, 1.0 },
	{ 0.0, 0.0, 0.0, 1.0 }
};

void calcNormal(GLdouble v0[3], GLdouble v1[3], GLdouble v2[3], GLdouble n[3])
{
	GLdouble v2v[2][3];
	GLdouble vt[3];
	double abs;
	int i;
	for (i = 0; i < 3; i++)
	{
		v2v[0][i] = v1[i] - v0[i];
		v2v[1][i] = v2[i] - v0[i];
	}
	vt[0] = v2v[0][1] * v2v[1][2] - v2v[0][2] * v2v[1][1];
	vt[1] = v2v[0][2] * v2v[1][0] - v2v[0][0] * v2v[1][2];
	vt[2] = v2v[0][0] * v2v[1][1] - v2v[0][1] * v2v[1][0];
	abs = sqrt(vt[0] * vt[0] + vt[1] * vt[1] + vt[2] * vt[2]);
	for (i = 0; i < 3; i++)
		n[i] = vt[i] / abs;
}

void draw_ground(void){
    int i, j;
	glPushMatrix();
	// glScalef(FIELD_MAX_X, FIELD_MAX_Y, FIELD_MAX_Z);
	glBegin(GL_QUADS);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[WHITE]);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	//glTranslatef(FIELD_MAX_X/2,0,FIELD_MAX_Z/2);
	glVertex3d(0,0,0);
	glVertex3d(0,0,FIELD_MAX_Z);
	glVertex3d(FIELD_MAX_X,0,FIELD_MAX_Z);
	glVertex3d(FIELD_MAX_X,0,0);
	glEnd();
	glPopMatrix();

}
void draw_snowman(Coordinate *location){
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[WHITE]);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
    glPushMatrix();
    glTranslatef(location->x,location->y,location->z);
	glRotatef(location->rotate * 180 / M_PI,0,1,0);
	glTranslatef(0,1,0);
    glutSolidSphere(1.0,30,30);
    glTranslatef(0,1.5,0);
    glutSolidSphere(0.7,30,30);
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color[BLACK]);
	glTranslatef(0.7*sin(70*M_PI/180)*sin(30*M_PI/180),0.7*cos(70*M_PI/180),-0.7*sin(70*M_PI/180)*cos(30*M_PI/180));
    glutSolidSphere(0.1,30,30);
	glTranslatef(-1.4*sin(70*M_PI/180)*sin(30*M_PI/180),0,0);
    glutSolidSphere(0.1,30,30);
	glTranslatef(0.7*sin(70*M_PI/180)*sin(30*M_PI/180),-0.7*cos(70*M_PI/180),0.7*sin(70*M_PI/180)*cos(30*M_PI/180));
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color[ORANGE]);
    glTranslatef(0,-1.5,0);
    glRotatef(180,1,0,0);
    glTranslatef(0,-1.7,0.5);
    glutSolidCone(0.25,0.5,30,30);
    glTranslatef(0,1.7,-0.5);
    glRotatef(-180,1,0,0);
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color[BROWN]);
    glTranslatef(0,0.2,0);
    glRotatef(45,0,0,1);
    for(int i=0;i<15;i++){
        glTranslatef(0,0.1,0);
        glutSolidCube(0.1);
    }
    glTranslatef(0,-1.5,0);
    glRotatef(90,0,0,-1);
    for(int j=0;j<15;j++){
        glTranslatef(0,0.1,0);
        glutSolidCube(0.1);
    }
    glPopMatrix();
}

void draw_bullet(Coordinate *location){
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[WHITE]);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
    glPushMatrix();
    glTranslatef(location->x,2,location->z);
    glutSolidSphere(BULLET_RADIUS,30,15);
    glPopMatrix();
}


void draw_wall(Coordinate *location){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[GRAY]);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
    glPushMatrix();
	glTranslatef(location->x,location->y,location->z);
	glRotatef(location->rotate * 180 / M_PI,0,1,0);
	glTranslatef(-2, 2, 0);
	for(int i=0;i<WALL_HEIGHT/4;i++){
		for(int j=0;j<2;j++){
			glutSolidCube(4);
			glTranslatef(4,0,0);
		}
		glTranslatef(-8, 4, 0);
	}
	glPopMatrix();
}


void put_character(Character c,Coordinate *location){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[WHITE]);
    glMaterialf(GL_FRONT, GL_SHININESS, 80);
    glPushMatrix();
	glTranslatef(location->x,location->y,location->z);
	glRotatef(location->rotate * 180 / M_PI,0,1,0);
	glScalef(PLAYER_HEIGHT, PLAYER_HEIGHT, PLAYER_HEIGHT);
	draw_character(c);

	glPopMatrix();
}

void draw_string(const char string[], Coordinate *location) {
	glRasterPos3f(location->x-strlen(string)/4.0, location->y+PLAYER_HEIGHT*4, location->z);
	while (*string != '\0') {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
	}
}