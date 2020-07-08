#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>

#define PI2 (3.1415*2)
#define N 50

GLfloat pos0[] = { 2.0, 2.0, 6.0, 0.0 };
//GLfloat pos1[] = { -2.0, 0.0, 2.0, 0.0 };
GLfloat face[] = { 199.0 / 255.0, 130.0 / 255.0, 18.0 / 255.0, 1.0 };
GLfloat black[] = { 0, 0, 0, 1 };
GLfloat white[] = { 1, 1, 1, 1 };
GLfloat yellow[] = { 253.0 / 255.0, 208.0 / 255.0, 0.0 / 255.0, 1.0 };
//GLfloat red[] = { 1, 0, 0, 1 };
//GLfloat blue[] = { 0, 0, 1, 1 };
GLfloat green[] = { 0.3, 1.0, 0.0 };

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

void mouth(void){
	int i, j;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);

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

void nose(void)
{   
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);

	glColor3d(0.0, 0.0, 0.0);
	glTranslatef(0, 0, -0.8);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();
}

void mouth_around(void) {
	glPushMatrix();
	//glColor3d(199.0/255.0, 130.0/255.0, 18.0/255.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);

	glScalef(1.5, 1.2, 0.7);
	glTranslatef(0, 0, -1.7);
	glutSolidSphere(0.5, 30, 30);
	glPopMatrix();
}

void eye(void) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
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

void head(void) {
	glPushMatrix();
	mouth();
	nose();
	mouth_around();
	eye();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, face);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	//glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);

	glScalef(1.5, 1.2, 1);
	glTranslatef(0, 0.7, -3);
	glutSolidSphere(2.0, 30, 30);
	glPopMatrix();
}

void Cylinder(double x)
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
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow);

	//glColor3d(253.0 / 255.0, 208.0 / 255.0, 0.0 / 255.0);
	glTranslatef(0, 0, 0.01);
	glScalef(0.75, 0.75, 1.0);
	Cylinder(0.3);
}

void ear_outside(void) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, face);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, face);
	glPushMatrix();
	glScalef(0.8, 0.8, 1.0);
	Cylinder(0.5);
	ear_inside();
	glPopMatrix();
}

void ear(void) {
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
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glutSolidSphere(1.0, 30, 30);
}

void body(void) {
	glPushMatrix();
	glScalef(2.2, 2.2, 1.4);
	glTranslatef(0.0, -3.0, -2.3);
	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, face);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, face);
	Cylinder(2.0);
	glPopMatrix();
	glPushMatrix();
	body_inside();
	glPopMatrix();
}

void leg_inside(void) {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow);
	glTranslatef(0.0, 0.0, 0.01);
	glScalef(0.8, 0.8, 1.0);
	Cylinder(0.5);
	glPopMatrix();
}

void leg(void) {
	glPushMatrix();
	glScalef(0.7, 0.7, 1.0);
	glTranslatef(1.7, -5.5, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, face);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);
	Cylinder(2.0);
	leg_inside();
	glTranslatef(-3.4, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, face);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000.0);
	Cylinder(2.0);
	leg_inside();
	glPopMatrix();
}

void arm(void) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, face);
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, face);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1000.0);

	GLfloat i = 3.8;
	glPushMatrix();

	glScalef(0.55, 0.55, 1.0);
	glTranslatef(i, -2.8, -0.5);
	//glRotatef(20, 1, 0, 0);

	Cylinder(1.5);
	glTranslatef(-2*i, 0.0, 0.0);
	Cylinder(1.5);
	glPopMatrix();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);

	head();
	ear();
	body();
	leg();
	arm();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_AMBIENT, green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, green);
	glBegin(GL_POLYGON);
	glVertex3d(-100,-5, 100);
	glVertex3d(100, -5, 100);
	glVertex3d(100, -5, -100);
	glVertex3d(-100, -5, -100);
	glEnd();

	glutSwapBuffers();
}

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0, 1, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0, 1.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(8.0, 0.5, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Rilakkuma");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
