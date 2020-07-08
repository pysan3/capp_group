#define GL_SILENCE_DEPRECATION 1
#include "characters.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


void pinkpart(){
    int i;
    double k=0.0,l=0.0,m=0.0,n=0.0,o=0.0;
    GLfloat pink[]={0.74,0.29,0.46,1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pink);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0);
    //glPushMatrix();
    glutSolidSphere(1.2, 30, 30);
    glTranslatef(0.0,0.65,0.0);
    glutSolidSphere(0.8,30,30);
    glTranslatef(0.8,0.3,0.0);
    glutSolidTorus(0.2, 0.3, 30,30);
    
    glTranslatef(-1.6,0,0);
    glutSolidTorus(0.2, 0.3, 30, 30);
    glTranslatef(0.8,-0.3,-0.1);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0.0, 0.0, 1.0);
    for(i=0;i<=10;i++){
        glTranslatef(0.0,0.0,0.1);
        glutSolidTorus(0.4, k, 30, 30);
        k=k+0.08;
    }
    for(i=0;i<5;i++){
        glTranslatef(0.0, 0.0, 0.1);
        glutSolidTorus(0.4, 0.8+l, 30, 30);
        l=l+0.04;
    }
    for(i=0;i<5;i++){
        glTranslatef(0.0, 0.0, 0.1);
        glutSolidTorus(0.4, 1.0+m, 30, 30);
        m=m-0.04;
    }
    glTranslatef(0.8,0.1,-1.6);
    glPushMatrix();
    glRotatef(35, 0, 1, 0);
    for(i=0;i<100;i++){
        glTranslatef(0,0,0.01);
        glutSolidSphere(0.3+n,30,30);
        n=n+0.001;
    }
    glRotatef(35, 0, -1, 0);
    for(i=0;i<10;i++){
        glTranslatef(0,0,0.01);
        glutSolidSphere(0.31-o,30,30);
        o=o+0.01;
      
    }
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.6,0,0);
    glRotatef(35, 0, -1, 0);
    n=0.0;
    for(i=0;i<100;i++){
        glTranslatef(0,0,0.01);
        glutSolidSphere(0.3+n,30,30);
        n=n+0.001;
    }
    o=0.0;
    glRotatef(35, 0, 1, 0);
    for(i=0;i<10;i++){
        glTranslatef(0,0,0.01);
        glutSolidSphere(0.31-o,30,30);
        o=o+0.01;
        
    }
    glPopMatrix();
    glTranslatef(-0.3, 0, 1.0);
    for(i=0;i<30;i++){
        glTranslatef(0,0,0.05);
        glutSolidSphere(0.7,30,30);
    }
    glTranslatef(-1.0,0,0);
    for(i=0;i<30;i++){
        glTranslatef(0,0,-0.05);
        glutSolidSphere(0.7, 30, 30);
    }
   
    
}

void whitepart(){
    GLfloat white[]={1.0,1.0,1.0,1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, 0.7);
    glutSolidSphere(0.7, 30, 30);
    glTranslatef(0.8,0.95,-0.7);
    glutSolidTorus(0.3,0.1,3,30);
    glTranslatef(-1.6,0,0);
    glutSolidTorus(0.3, 0.1, 3, 30);
    //glTranslatef(0.8,-0.95,0.8);
    glTranslatef(0.95,-0.25,0.8);
    glutSolidSphere(0.18,30,30);
    glTranslatef(-0.3, 0.0, 0);
    glutSolidSphere(0.18, 30, 30);
    glTranslatef(0.15,-2.5,-0.30);
    glutSolidSphere(0.9,30,30);
}

void darkpinkpart(){
    GLfloat darkpink[]={0.2,0.06,0.13,1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, darkpink);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,0.3,1.3);
    glRotatef(30, 0, 0, 1);
    glutSolidTorus(0.2,0.2,30,3);
    glRotatef(30,0,0,-1);
    glTranslatef(0.15,0.7,-0.65);
    glRotatef(10, -1, 1, 0);
    glutSolidCube(0.2);
    glTranslatef(0.2,0,0);
    glutSolidCube(0.2);
    glRotatef(10, 1, -1, 0);
    glTranslatef(-0.5,0,0);
    glRotatef(10,1,-1,0);
    glutSolidCube(0.2);
    glTranslatef(-0.2,0,0);
    glutSolidCube(0.2);
   
}
void blackpart(){
    GLfloat black[]={.0,.0,.0,1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.15,0.7,0.85);
    glutSolidSphere(0.15, 30, 30);
    glTranslatef(-0.3,0,0);
    glutSolidSphere(0.15,30,30);
    
}

void draw_LOTSO(void)
{
	
	
    glTranslatef(0,0.3,0);
	glPushMatrix();
    pinkpart();
    glPopMatrix();
    whitepart();
    darkpinkpart();
    blackpart();
    

	glPopMatrix();
	
}




