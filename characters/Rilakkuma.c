#include "../header.h"
#include "characters.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI2 (3.1415*2)
#define N 50

GLfloat ktface[] = { 199.0 / 255.0, 130.0 / 255.0, 18.0 / 255.0, 1.0 };
GLfloat ktblack[] = { 0, 0, 0, 1 };
GLfloat ktwhite[] = { 1, 1, 1, 1 };
GLfloat ktyellow[] = { 253.0 / 255.0, 208.0 / 255.0, 0.0 / 255.0, 1.0 };
GLfloat ktgreen[] = { 0.3, 1.0, 0.0 };

#define W 30
#define H 8
char mouthshape[H][W] = {
		{ "                             " },
		{ "              **             " },
		{ "             ****            " },
		{ "           ***  ***          " },
		{ "         ****    ****        " },
		{ "      ****          ****     " },
		{ "  ****                  **** " },
		{ "                             " },
};

void ktmouth(void){
	int i, j;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktblack);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktblack);

	glPushMatrix();
	glScalef(1.0, 0.6, 0.6);
	glTranslatef(0, -0.5, -0.2);
	glColor3d(0.0, 0.0, 0.0);
	for (i = 0; i < H; i++)
		for (j = 0; j < W - 1; j++)
		{
			if (mouthshape[H - i - 1][j] == ' ')continue;
			glBegin(GL_POLYGON);
			glVertex3d(j / (double)W - 0.5, i / (double)H - 0.5, -(1+0.03*fabsf(j-W/2)));
			glVertex3d((j + 1) / (double)W - 0.5, i / (double)H - 0.5, -(1+0.03 * fabsf(j - W / 2)));
			glVertex3d((j + 1) / (double)W - 0.5, (i + 1) / (double)H - 0.5, -(1+0.03 * fabsf(j - W / 2)));
			glVertex3d(j / (double)W - 0.5, (i + 1) / (double)H - 0.5, -(1+0.03 * fabsf(j - W / 2)));
			glEnd();
		}
	glPopMatrix();
}

void ktnose(void)
{   
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktblack);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktwhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);

	glColor3d(0.0, 0.0, 0.0);
	glTranslatef(0, 0, -0.8);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();
}

void mouth_around(void) {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktwhite);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktwhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);

	glScalef(1.5, 1.2, 0.7);
	glTranslatef(0, 0, -1.7);
	glutSolidSphere(0.5, 30, 30);
	glPopMatrix();
}

void kteye(void) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktblack);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktwhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);

	glPushMatrix();
	glTranslatef(-1.2, 0.2, -1.3);
	glutSolidSphere(0.3, 30, 30); 
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.2, 0.2, -1.3);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();
}

void kthead(void) {
	glPushMatrix();
	ktmouth();
	ktnose();
	mouth_around();
	kteye();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktface);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktwhite);

	glScalef(1.5, 1.2, 1);
	glTranslatef(0, 0.7, -3);
	glutSolidSphere(2.0, 30, 30);
	glPopMatrix();
}

void ktCylinder(double x)
{
	int i;

	glNormal3d(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	for (i = 0; i < N; i++) {
		glVertex3d(cos(PI2 * i / N), sin(PI2 * i / N), -1);
	}
	glEnd();

	glNormal3d(0.0, 0.0, -1.0);
	glBegin(GL_POLYGON);
	for (i = N; i > 0; i--) {
		glVertex3d(cos(PI2 * i / N), sin(PI2 * i / N), -(1 + x));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < N; i++) {
		glNormal3d(cos(PI2 * 2*i / N), sin(PI2 * 2*i / N), 0.0);
		glVertex3f(cos(PI2 * 2*i / N), sin(PI2 * 2*i / N), -1.0);
		glVertex3f(cos(PI2 * 2*i / N), sin(PI2 * 2*i / N), -(1.0 + x));
	}
	glEnd();
}

void ear_inside(void) {
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktyellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktyellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktyellow);

	glTranslatef(0, 0, 0.01);
	glScalef(0.75, 0.75, 1.0);
	ktCylinder(0.3);
}

void ear_outside(void) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktface);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktface);
	glPushMatrix();
	glScalef(0.8, 0.8, 1.0);
	ktCylinder(0.5);
	ear_inside();
	glPopMatrix();
}

void ktear(void) {
	glPushMatrix();
	glTranslatef(1.8, 2.0, -0.6);
	ear_outside();
	glTranslatef(-3.6, 0.0, 0.0);
	ear_outside();
	glPopMatrix();
}

void body_inside(void) {
	glTranslatef(0.0, -2.75, -2.0);
	glScalef(1.1, 1.25, 0.5);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktwhite);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktwhite);
	glutSolidSphere(1.0, 30, 30);
}

void ktbody(void) {
	glPushMatrix();
	glScalef(2.2, 2.2, 1.4);
	glTranslatef(0.0, -3.0, -2.3);
	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktface);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktface);
	ktCylinder(2.0);
	glPopMatrix();
	glPushMatrix();
	body_inside();
	glPopMatrix();
}

void leg_inside(void) {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktyellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktyellow);
	glTranslatef(0.0, 0.0, 0.01);
	glScalef(0.8, 0.8, 1.0);
	ktCylinder(0.5);
	glPopMatrix();
}

void ktleg(void) {
	glPushMatrix();
	glScalef(0.7, 1.0, 0.7);
	glTranslatef(1.7, -7.0, -4.5);
	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktface);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktwhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);
	ktCylinder(2.0);
	leg_inside();
	glTranslatef(-3.4, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktface);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktwhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);
	ktCylinder(2.0);
	leg_inside();
	glPopMatrix();
}

void ktarm(void) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ktface);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ktblack);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ktface);

	GLfloat i = 3.8;
	glPushMatrix();

	glScalef(0.55, 0.55, 1.0);
	glTranslatef(i, -2.8, -0.5);

	ktCylinder(1.5);
	glTranslatef(-2*i, 0.0, 0.0);
	ktCylinder(1.5);
	glPopMatrix();
}


void draw_Rilakkuma(void)
{
	kthead();
	ktear();
	ktbody();
	ktleg();
	ktarm();
}

