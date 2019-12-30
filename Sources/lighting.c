#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>

#include "../Headers/lighting.h"

void set_lighting(){
    
        // Pozicija svetla
    GLfloat light_position[] = { 1, 1, 1, 0 };

    // Ambijentalna komponenta svetlosti
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1 };

    // Difuzna komponenta svetlosti
    GLfloat light_diffuse[] = { 0.95, 0.95, 0.95, 1 };
    
    // Spekularna komponenta svetlosti
    //GLfloat light_specular[] = { 0.8, 0.8, 0.2, 1 };
    GLfloat light_specular[] = {1, 1, 1, 1,};

    // Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.5, 0.5, 0.1, 1 };

    // Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 1, 1, 0.8, 1 };

    // Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 1, 1, 1, 0 };

    // Koeficijent glatkosti materijala
    GLfloat shininess = 20;

    // Brise se prethodni sadrzaj prozora
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Ukljucuje se osvetljenje i podesavaju parametri svetla
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Podesavaju se parametri materijala
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
    glShadeModel(GL_FLAT);
}
