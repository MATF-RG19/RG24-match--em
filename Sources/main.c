#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define TIMER_INTERVAL (20)
#define TIMER_ID (0)

#define check_error(expr, Usermsg)\
    do{\
        if(!(expr)){\
            fprintf(stderr, "%s\n", Usermsg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
    
#include "../Headers/coordinates.h"
#include "../Headers/draw.h"
#include "../Headers/lighting.h"

//Dimenzije prozora
static int window_width, window_height;

int animation_ongoing = 0;
double animation_parameter = 0;

int** matrix;
int broj;
//x i y u wcs
GLdouble x_1, y_1, x_2, y_2;
//prodnadjeni x i y u odnosu na niz
GLdouble xp1, yp1, xp2, yp2;

int i_1, i_2, j_1, j_2;

//Deklaracije callback funkcija
static void on_display(void);
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse(int button, int state, int x, int y);
static void on_timer(int value);

//fja koja iscrtava objekte na veliku tablu nakon svake animacije
//tj. nakon swapovanja, prikupljanja objekata i iscrtavanja novih nakon sto prethodni
//sa tog mesta nestanu prikupljanjem
void draw_objects(int animation_ongoing, double animation_parameter);
//pronalazi koordinate objekata u nizu tj. njihov centar u odnosu na onu tacku
//na koju klikne korisnik, jer nemoguce je da korisnik klikne tacnu tacku
//u odnosu na koju se iscrtava objekat
void find_objects(GLdouble x_1, GLdouble y_1, GLdouble x_2, GLdouble y_2);
void move_on_x(double animation_parameter, int p);
void move_on_y(double animation_parameter, int p);

int main(int argc, char **argv){
    
    
    //Inicijalizuje se GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    //Kreira se prozor
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    //Registruju se callback funkcije
    glutKeyboardFunc(on_keyboard);
    glutMouseFunc(on_mouse);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    //Obavlja se OpenGL inicijalizacija
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2);
    
    srand(time(NULL));
    matrix = make_matrix();
    //Matrica intova u odnosu na koju se iscrtavaju elementi
    draw(matrix);
    init_objects(matrix);
    broj = (int)(rand()%5);

    //Program ulazi u glavnu petlju
    glutMainLoop();

    return 0;
}

static void on_mouse(int button, int state, int x, int y){
    
   GLint viewport[4];
   GLdouble mvmatrix[16], projmatrix[16];
   GLint realy;
   GLdouble wx, wy, wz;  //world x, y, z coords

    //odredjujem poziciju objekta koji je "prvi" oznacen tj. onaj na koji smo pritisli mis
    if ((button == GLUT_LEFT_BUTTON ) && (state == GLUT_DOWN)){
            glGetIntegerv (GL_VIEWPORT, viewport);
            glPushMatrix();
                glLoadIdentity();
                gluLookAt(0, 0, 5,0,0,0,0,1,0);
                glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
            glPopMatrix();
            glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
            
            realy = viewport[3] - (GLint) y - 1;
            gluUnProject ((GLdouble) x, (GLdouble) realy, 1.0,
                           mvmatrix, projmatrix, viewport,
                           &wx, &wy, &wz);
            //delim sa cetiri jer tako bas dobijam pozicije koje sam i postavila objektima
            //nisam sigurna zasto mi gluUnProject vrati bas 4x vece vrednosti
            //printf ("World coords1 at z=1.0 are (%f, %f, %f)\n", wx/4.0, wy/4.0, wz);
            x_1 = wx/4.0;
            y_1 = wy/4.0;
    }
    
    //odredjujem poziciju objekta koji je "drugi" oznacen tj. onaj na koji smo otpustili mis nakon prevlacenja
    if ((button == GLUT_LEFT_BUTTON ) && (state == GLUT_UP)){
        
            glGetIntegerv (GL_VIEWPORT, viewport);
            glPushMatrix();
                glLoadIdentity();
                gluLookAt(0, 0, 5,0,0,0,0,1,0);
                glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
            glPopMatrix();
            glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
            
            realy = viewport[3] - (GLint) y - 1;
            gluUnProject ((GLdouble) x, (GLdouble) realy, 1.0,
                           mvmatrix, projmatrix, viewport, 
                           &wx, &wy, &wz);
            //delim sa cetiri jer tako bas dobijam pozicije koje sam i postavila objektima
            //nisam sigurna zasto mi gluUnProject vrati bas 4x vece vrednosti
            //printf ("World coords2 at z=1.0 are (%f, %f, %f)\n", wx/4.0, wy/4.0, wz);
            x_2 = wx/4.0;
            y_2 = wy/4.0;
            
            find_objects(x_1, y_1, x_2, y_2);
            
            animation_ongoing = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
    case 27:
        //Zavrsava se program
        exit(0);
        break;
    case 'r':
    case 'R':
        animation_parameter = 0;
        glutPostRedisplay();
        break;
    }
}

static void on_reshape(int width, int height){
    //Pamte se sirina i visina prozora
    window_width = width;
    window_height = height;
}

static void on_display(void){
    
    
    //Brise se prethodni sadrzaj prozora
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    //Podesava se viewport
    glViewport(0, 0, window_width, window_height);

    //Podesava se projekcija
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20);
    //gluPerspective (45.0, window_width/window_height, 1.5, 20.0);

    //Podesava se tacka pogleda
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 5.0, 
               0.0, 0.0, 0.0, 
               0.0, 1.0, 0.0);
    
    //Podesava se osvetljenje
    set_lighting();
    //Pomocne koor. linije
    draw_coordinate_lines();

    //Iscrtavaju se dve table na kojima se nalaze elementi za igru
    draw_tables();
    //Iscrtavanje elemenata sa kojima se igra
    draw_objects(animation_ongoing, animation_parameter);
    //Iscrtavanje elementa koji se prikuplja
    draw_on_side(broj);
    
    //Nova slika se salje na ekran
    glutSwapBuffers();
    
}

static void on_timer(int value){
    
    if(value!=TIMER_ID){
        return;
    }
    
    animation_parameter += 0.2;
    
   if(animation_parameter > 1){
       
        animation_ongoing = 0;
        animation_parameter = 0;
        
        return;
        
    }
    
    
    glutPostRedisplay();
    
    if (animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    
}

void move_on_x(double animation_parameter, int p){
    int type;
    GLdouble x, y, z;
    
    for(int k = 0; k < OBJECTS_MAX; k++){
            
        type = objects[k].type;
        x = objects[k].x;
        y = objects[k].y;
        z = objects[k].z;
            
        if(x == xp1 && y == yp1){
            glPushMatrix();
            if(animation_parameter == 1.0)
                glTranslatef(0.0, 0.0, -0.2);
            printf("x2: %f, x2 + ap: %f, z: %f\n",x, x + animation_parameter*(0.5*p), z);
            glTranslatef(x + animation_parameter*(0.5*p), y + 0.0, z + 0.2);
            draw_one(type);
                objects[k].x += p*0.5;
        glPopMatrix();
        }
        else if(x == xp2 && y == yp2){
            glPushMatrix();
                glTranslatef(x + animation_parameter*(-0.5*p), y + 0.0, z + 0.0);
                draw_one(type);
            glPopMatrix();
                objects[k].x -= p*0.5;
        }
        else{
            glPushMatrix();
                glTranslatef(x, y, z);
                draw_one(type);
            glPopMatrix();
        }
    }
    
}
void move_on_y(double animation_parameter, int p){
    
    int type;
    GLdouble x, y, z;
    
    for(int k = 0; k < OBJECTS_MAX; k++){
            
        type = objects[k].type;
        x = objects[k].x;
        y = objects[k].y;
        z = objects[k].z;
            
        if(y == yp1 && x == xp1){
            glPushMatrix();
                if(animation_parameter == 1.0)
                    glTranslatef(0.0, 0.0, -0.2);
                glTranslatef(x + 0.0, y + animation_parameter*(-0.5*p), z + 0.2);
                draw_one(type);
                objects[k].y -= p*0.5;
            glPopMatrix();
        }
        else if(y == yp2 && x == xp2){
            glPushMatrix();
                glTranslatef(x + 0.0, y + animation_parameter*(0.5*p), z + 0.0);
                draw_one(type);
            glPopMatrix();
            objects[k].y += p*0.5;
        }
        else{
            glPushMatrix();
                glTranslatef(x, y, z);
                draw_one(type);
            glPopMatrix();
        }
    }
}

void draw_objects(int animation_ongoing, double animation_parameter){
    
    int type;
    GLdouble x, y, z;
    
    if(animation_ongoing == 1){
            
            //pomeramo po x
            if(yp1 == yp2){
                if(xp1 < xp2)
                    move_on_x(animation_parameter, 1);
                else
                    move_on_x(animation_parameter, -1);
                    
            }
            //pomeramo po y
            else if(xp1 == xp2){
                if(yp1 > yp2)
                    move_on_y(animation_parameter, 1);
                else
                    move_on_y(animation_parameter, -1);
            }
        }
    else{
        for(int k = 0; k < OBJECTS_MAX; k++){
                
            type = objects[k].type;
            x = objects[k].x;
            y = objects[k].y;
            z = objects[k].z;
            
            glPushMatrix();
                glTranslatef(x, y, z);
                draw_one(type);
            glPopMatrix();
        }
        
    }
}

void find_objects(GLdouble x_1, GLdouble y_1, GLdouble x_2, GLdouble y_2){
    
    GLdouble h = 0.25;
    
    for(int k = 0; k < 7; k++){
        if(x_1 >= objects[k].x - h  && x_1 <= objects[k].x + h){
            xp1 = objects[k].x;
            j_1 = k;
        }
        if(x_2 >= objects[k].x - h  && x_2 <= objects[k].x + h){
            xp2 = objects[k].x;
            j_2 = k;
        }
    }
    
    for(int k = 0; k < OBJECTS_MAX; k+=7){
        if(y_1 >= objects[k].y - h  && y_1 <= objects[k].y + h){
            yp1 = objects[k].y;
            i_1 = k;
        }
        if(y_2 >= objects[k].y - h  && y_2 <= objects[k].y + h){
            yp2 = objects[k].y;
            i_2 = k;
        }
    }
    
    //printf("xp1: %f, yp1: %f, xp2: %f, yp2: %f\n", xp1, yp1, xp2, yp2);
}
