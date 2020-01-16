#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>

#include "../Headers/coordinates.h"

void draw_coordinate_lines(void){
 
//iscrtavanje x ose
glPushMatrix();
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glNormal3f(1.0,0.0,0.0);
        glVertex3f(100,0,0);
        glVertex3f(-100,0,0);
    glEnd();
glPopMatrix();

//iscrtavanje y ose
glPushMatrix();
    glColor3f(0,1,0); 
    glBegin(GL_LINES);
        glNormal3f(1.0,0.0,0.0);
        glVertex3f(0,-100,0);
        glVertex3f(0,100,0);
    glEnd();
glPopMatrix();

//iscrtavanje z ose
glPushMatrix();
    glColor3f(0,0,1); 
    glBegin(GL_LINES);
        glNormal3f(1.0,0.0,0.0);
        glVertex3f(0,0,100);
        glVertex3f(0,0,-100);
    glEnd();
glPopMatrix();
}
