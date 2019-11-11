#include <stdlib.h>
#include <GL/glut.h>

/* Dimenzije prozora */
static int window_width, window_height;

/* Deklaracije callback funkcija */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

int main(int argc, char **argv){
    /* Inicijalizuje se GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor */
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija */
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2);

    /* Program ulazi u glavnu petlju */
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
    case 27:
        /* Zavrsava se program */
        exit(0);
        break;
    }
}

static void on_reshape(int width, int height){
    /* Pamte se sirina i visina prozora */
    window_width = width;
    window_height = height;
}

static void on_display(void){
    /* Brise se prethodni sadrzaj prozora */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se viewport */
    glViewport(0, 0, window_width, window_height);

    /* Podesava se projekcija */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

    /* Podesava se tacka pogleda */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 5.0, 
               0.0, 0.0, 0.0, 
               0.0, 1.0, 0.0);
    
    /* Iscrtavanje x ose */
    glPushMatrix();
        glColor3f(1,0,0);
        glBegin(GL_LINES);
            glVertex3f(100,0,0);
            glVertex3f(-100,0,0);
        glEnd();
    glPopMatrix();

    /* Iscrtavanje y ose */
    glPushMatrix();
        glColor3f(0,1,0); 
        glBegin(GL_LINES); 
            glVertex3f(0,-100,0);
            glVertex3f(0,100,0);
        glEnd();
    glPopMatrix();

    /* Iscrtavanje z ose */
    glPushMatrix();
        glColor3f(0,0,1); 
        glBegin(GL_LINES);
            glVertex3f(0,0,100);
            glVertex3f(0,0,-100);
        glEnd();
    glPopMatrix();

   /* Podesava se prvi plavi pravougaonik */
    glColor3f(0.74902, 0.847059, 0.847059);
    
   
    glBegin(GL_POLYGON);
        glVertex3f (-1.8, -1.8, 0.0);
        glVertex3f (-1.8, 1.8, 0.0);
        glVertex3f (1.8, 1.8, 0.0);
        glVertex3f (1.8, -1.8, 0.0);
    glEnd();
    
    /* Podesava se drugi zeleni pravougaonik */
    glColor3f(0.560784, 0.737255, 0.560784);
    
     glBegin(GL_POLYGON);
        glVertex3f (-3.35, -1.2, 0.0);
        glVertex3f (-3.35, 1.2, 0.0);
        glVertex3f (-2.35, 1.2, 0.0);
        glVertex3f (-2.35, -1.2, 0.0);
    glEnd();
    
    /* Druga opcija  je da bude pored njega:
     * glBegin(GL_POLYGON);
        glVertex3f (-3.0, -1.2, 0.0);
        glVertex3f (-3.0, 1.2, 0.0);
        glVertex3f (-2.0, 1.2, 0.0);
        glVertex3f (-2.0, -1.2, 0.0);
    glEnd();
    */

    /* Nova slika se salje na ekran */
    glutSwapBuffers();
}
