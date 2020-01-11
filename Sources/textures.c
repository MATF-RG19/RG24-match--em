#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "../Headers/image.h"
#include "../Headers/textures.h"



//imena fajlova sa teksturama
#define FILENAME0 "../Textures/1.bmp"
#define FILENAME1 "../Textures/7.bmp"

//identifikatori tekstura
static GLuint names[2];

void initialize(void)
{
    //objekat koji predstavlja teskturu ucitanu iz fajla
    Image * image;

    //postavlja se boja pozadine
    glClearColor(0, 0, 0, 0);

    //ukljucuje se testiranje z-koordinate piksela
    glEnable(GL_DEPTH_TEST);

    //ukljucuju se teksture
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

     //inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
    //fajla
    image = image_init(0, 0);

    //kreira se prva tekstura
    image_read(image, FILENAME0);

    //generisu se identifikatori tekstura
    glGenTextures(2, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    //kreira se druga tekstura
    image_read(image, FILENAME1);

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    //iskljucujemo aktivnu teksturu
    glBindTexture(GL_TEXTURE_2D, 0);

    //unistava se objekat za citanje tekstura iz fajla
    image_done(image);
}

void textures(){
    
   //iscrtava se pozadina
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-1.0*3.45, -1.0*3.45 , -0.02);

        glTexCoord2f(1, 0);
        glVertex3f(1*3.45, -1.0*3.45, -0.02);

        glTexCoord2f(1, 1);
        glVertex3f(1*3.45,1*3.45, -0.02);

        glTexCoord2f(0, 1);
        glVertex3f(-1*3.45, 1*3.45, -0.02);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    //crta se pozadina velikog pravougaonika
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-1.8, -1.8, 0.0);

        glTexCoord2f(1, 0);
        glVertex3f(1.8, -1.8, 0.0);

        glTexCoord2f(1, 1);
        glVertex3f(1.8, 1.8, 0.0);

        glTexCoord2f(0, 1);
        glVertex3f(-1.8, 1.8, 0.0);
    glEnd();

    //iskljucujemo aktivnu teksturu
    glBindTexture(GL_TEXTURE_2D, 0);
}
