#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592

enum COLOR { WHITE, BROWN, ORANGE, GRAY, BLACK };
GLfloat color[][4] = {
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 0.5, 0.4, 0.35, 1.0 },
		{ 0.99, 0.6, 0.4, 1.0 },
		{ 0.8, 0.8, 0.8, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 } };


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
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
		{
		GLdouble v[5][3];
		v[0][0] = (j + 0)*0.5 - 2.5;
		v[0][1] = (i + 0)*0.5 - 2.5;
		v[0][2] = 0;
		v[1][0] = (j + 1)*0.5 - 2.5;
		v[1][1] = (i + 0)*0.5 - 2.5;
		v[1][2] = 0;
		v[2][0] = (j + 1)*0.5 - 2.5;
		v[2][1] = (i + 1)*0.5 - 2.5;
		v[2][2] = 0;
		v[3][0] = (j + 0)*0.5 - 2.5;
		v[3][1] = (i + 1)*0.5 - 2.5;
		v[3][2] = 0;
		if ((i^j) & 1)
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[WHITE]);
			glMaterialfv(GL_FRONT, GL_AMBIENT, color[BLACK]);
			glMaterialfv(GL_FRONT, GL_SPECULAR, color[WHITE]);
			glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
		}
		else
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[WHITE]);
			glMaterialfv(GL_FRONT, GL_AMBIENT, color[BLACK]);
			glMaterialfv(GL_FRONT, GL_SPECULAR, color[WHITE]);
			glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
		}
		calcNormal(v[0], v[1], v[2], v[4]);
		glNormal3dv(v[4]);
		glBegin(GL_QUADS);
		glVertex3dv(v[0]);
		glVertex3dv(v[1]);
		glVertex3dv(v[2]);
		glVertex3dv(v[3]);
		glEnd();
		}
	glPopMatrix();

}
void draw_snowman(Coordinate *location){
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[WHITE]);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
    glPushMatrix();
    glTranslatef(location->x,location->y,location->z);
	glRotatef(location->rotate,0,1,0);
	glTranslatef(0,1,0);
    glutSolidSphere(1.0,30,30);
    glTranslatef(0,1.5,0);
    glutSolidSphere(0.7,30,30);
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color[BLACK]);
    glTranslatef(0.7*sin(70*PI/180)*sin(30*PI/180),0.7*cos(70*PI/180),-0.7*sin(70*PI/180)*cos(30*PI/180));
    glutSolidSphere(0.1,30,30);
    glTranslatef(-1.4*sin(70*PI/180)*sin(30*PI/180),0,0);
    glutSolidSphere(0.1,30,30);
    glTranslatef(0.7*sin(70*PI/180)*sin(30*PI/180),-0.7*cos(70*PI/180),0.7*sin(70*PI/180)*cos(30*PI/180));
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
    glTranslatef(location->x,location->y,location->z);
    glutSolidSphere(0.30,30,15);
    glPopMatrix(); 
}


void draw_wall(Coordinate *location){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[GRAY]);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
    glPushMatrix();
	glTranslatef(location->x,location->y,location->z);
	glRotatef(location->rotate,0,1,0);
	glTranslatef(-1.5,0,0);
	for(int i=1;i<30;i++){
		for(int j=1;j<=40;j++){
			glutSolidCube(0.1);
			glTranslatef(0,0.1,0);
		}
		glTranslatef(0.1,-4,0);
	}
	glPopMatrix();
}


void put_character(Character c,Coordinate *location){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[WHITE]);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
    glPushMatrix();
	glTranslatef(location->x,location->y,location->z);
	glRotatef(location->rotate,0,1,0);
	if(c==DORAEMON){
		glRotatef(-90,1,0,0);
		glRotatef(-90,0,0,1);
		glTranslatef(0,0,1.5);
		glScalef(0.8,0.8,0.8);
	}else if(c==LOTSO){
		glTranslatef(0,2.2,0);
		glScalef(0.55,0.55,0.55);
	}else if(c==KATANA){
		glTranslatef(0,0.5,0);
	}else if(c==RILAKKUMA){
		glTranslatef(0,1.8,1);
		glScalef(0.3,0.3,0.3);
	}
	draw_character(c);

	glPopMatrix();
}
