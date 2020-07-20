#include "../header.h"
#include "characters.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

GLfloat pos[]={3.0,4.0,5.0,1.0};
GLfloat blackam[] = {0.05375, 0.05,    0.06625, 1.0};
GLfloat blackdiff[] = {0.18275, 0.17,    0.22525, 1.0};
GLfloat blackspe[]={0.332741,0.328634,0.346435,1.0};
GLfloat blacksh=34.8;
GLfloat blackam1[]={0.02,   0.02,    0.02, 1.0};
GLfloat blackdiff1[] = {0.01,   0.01,    0.01, 1.0};
GLfloat blackspe1[]={0.5,  0.5,     0.0,  1.0};
GLfloat blacksh1=32.0;
GLfloat silveram[]={0.19225,  0.19225,  0.19225, 1.0};
GLfloat silverdiff[]={0.50754,  0.50754,  0.50754, 1.0};
GLfloat silverspe[]={0.508273, 0.508273, 0.508273,1.0};
GLfloat silversh = 51.2;
GLfloat whiteam[]={0.06,   0.06,    0.06, 1.0};
GLfloat whitediff[]={0.7,    0.7,     0.7,  1.0};
GLfloat whitespe[]={0.8,    0.8,     0.8,  1.0};
GLfloat whitesh = 10.0;
GLfloat orangeam[]={0.09,  0.04,    0.0,  1.0};
GLfloat orangediff[]={0.9,   0.2,     0.4,  1.0};
GLfloat orangespe[]={0.9,   0.4,     0.04, 1.0};
GLfloat orangesh = 10.0;


GLdouble vertex[][3] = {//鞘用
	{ -2.0, 0.0, 0.0 },//1
	{ 1.0, 0.0, 0.0 },//2
	{ 1.0, 0.13, 0.0 },//3
	{ -2.0, 0.13, 0.0 },//4
	{ -2.0, 0.0, 0.17},//5
	{ 1.0, 0.0, 0.17},//6
	{ 1.0, 0.13, 0.17},//7
	{ -2.0, 0.13, 0.17 }//8
};

GLdouble vertex1[][3]={//柄用
	{1.02,0.0,0.0},
	{1.62,0.0,0.0},
	{1.62,0.13,0.0},
	{1.02,0.13,0.0},
	{1.02,0.0,0.17},
	{1.62,0.0,0.17},
	{1.62,0.13,0.17},
	{1.02,0.13,0.17}
};

GLdouble vertex2[][3]={//兜金
	{1.62,0.0,0.0},
	{1.72,0.0,0.0},
	{1.72,0.13,0.0},
	{1.62,0.13,0.0},
	{1.62,0.0,0.17},
	{1.72,0.0,0.17},
	{1.72,0.13,0.17},
	{1.62,0.13,0.17}
};


int face1[][4] = {
		{ 0, 3, 2, 1 },
		{ 1, 2, 6, 5 },
		{ 4, 5, 6, 7 },
		{ 0, 4, 7, 3 },
		{ 0, 1, 4, 5 },
		{ 3, 7, 6, 2 }
};

GLdouble norm1[][3] = {
		{ 0.0, 0.0, -1.0 },
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ -1.0, 0.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, 1.0, 0.0 }
};

GLfloat ctrlpoints[4][3]={
	{-1.6,2.0,-0.05},
	{-1.3,0.5,0.0},
	{1.0,0.2,0.0},
	{0.8,-1.0,0.1}
};


void saya(void)
{
	int i;
	int j;

	glBegin(GL_QUADS);
	for (j = 0; j < 6; j++) {
		glNormal3dv(norm1[j]);
		for (i = 0; i < 4; i++) {
			glVertex3dv(vertex[face1[j][i]]);
		}
	}
	glEnd();
}

void tuba(void){
	double pi = 3.1415;
	//上の円
	glNormal3d(1.0,0.0,0.0);
	glBegin(GL_POLYGON);
	for(double i=10;i>=0;i--){
		double t=pi*2/10*(double)i;
		glVertex3d(0.02,0.2 * sin(t),0.2*cos(t));
	}
	glEnd();
	//側面
	glBegin(GL_QUAD_STRIP);
	for(double i=0;i<14;i++){
		double t = i*2*pi/10;
		glNormal3f(0.0,(GLfloat)sin(t),(GLfloat)cos(t));
		glVertex3f(-0.02,(GLfloat)(0.2*sin(t)),(GLfloat)(0.2*cos(t)));
		glVertex3f(0.02,(GLfloat)(0.2*sin(t)),(GLfloat)(0.2*cos(t)));
	}
	glEnd();
	//下の円
	glNormal3d(-1.0,0.0,0.0);
	glBegin(GL_POLYGON);
	for(double i=0;i<10;i++){
		double t=pi*2/10*(double)i;
		glVertex3d(-0.02,0.2 * sin(t),0.2*cos(t));
	}
	glEnd();
}

void tuka(void)
{
	int i;
	int j;

	glBegin(GL_QUADS);
	for (j = 0; j < 6; j++) {
		glNormal3dv(norm1[j]);
		for (i = 0; i < 4; i++) {
			glVertex3dv(vertex1[face1[j][i]]);
		}
	}
	glEnd();
}
void gara(void)
{
	int i;
	glNormal3d(0.0,1.0,0.0);
	for(i=0;i<6;i++){
		glBegin(GL_POLYGON);
		glVertex3d(1.07+i*0.1,0.13,0.17);
		glVertex3d(1.07+i*0.1,0.13,0);
		glVertex3d(1.02+i*0.1,0.13,0.085);
		glVertex3d(1.12+i*0.1,0.13,0.085);
		glEnd();
	}
}

void kabutogane(void)
{
	int i;
	int j;

	glBegin(GL_QUADS);
	for (j = 0; j < 6; j++) {
		glNormal3dv(norm1[j]);
		for (i = 0; i < 4; i++) {
			glVertex3dv(vertex2[face1[j][i]]);
		}
	}
	glEnd();
}

void draw_KATANA(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION,pos);
	glMaterialfv(GL_FRONT, GL_AMBIENT, blackam);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blackdiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blackspe);
	glMaterialfv(GL_FRONT, GL_SHININESS, &blacksh);
	saya();//鞘の部分
	glMaterialfv(GL_FRONT, GL_AMBIENT, silveram);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, silverdiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, silverspe);
	glMaterialfv(GL_FRONT, GL_SHININESS, &silversh);
	glTranslatef(1.02,0.06,0.08);
	tuba();//鍔の部分
	glTranslatef(-1.02,-0.06,-0.08);
	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteam);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whitediff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whitespe);
	glMaterialfv(GL_FRONT, GL_SHININESS, &whitesh);
	gara();//柄のみため
	glMaterialfv(GL_FRONT, GL_AMBIENT, blackam1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blackdiff1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blackspe1);
	glMaterialfv(GL_FRONT, GL_SHININESS, &blacksh1);
	tuka();//柄の部分
	glMaterialfv(GL_FRONT, GL_AMBIENT, blackam);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blackdiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blackspe);
	glMaterialfv(GL_FRONT, GL_SHININESS, &blacksh);
	kabutogane();//兜金の部分
}




