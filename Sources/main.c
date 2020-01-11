#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define TIMER_INTERVAL (20)
#define TIMER_INTERVAL3 (50)
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
#include "../Headers/textures.h"

typedef struct Pair{
    
    int i;
    int j;
    
}pair;

int skupljeno = 0;

//Dimenzije prozora
static int window_width, window_height;

int animation_ongoing = 0;
double animation_parameter = 0;

int animation_ongoing2 = 0;
double animation_parameter2 = 0;

int animation_ongoing3 = 0;
double animation_parameter3 = 0;

int** matrix;

int num;
int found_num = 0;

int found_column = 0;
pair column_array[3];

int found_row = 0;
pair row_array[3];

//x i y u wcs
GLdouble x_1, y_1, x_2, y_2;
//prodnadjeni x i y u odnosu na niz
GLdouble xp1, yp1, xp2, yp2;
//int pom_t, pom_i, pom_j;

int i_1, i_2, j_1, j_2;

int proba, proba1, proba2;

//Deklaracije callback funkcija
static void on_display(void);
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse(int button, int state, int x, int y);

static void on_timer(int value);
static void on_timer2(int value);

//fja koja iscrtava objekte na veliku tablu nakon svake animacije
//tj. nakon swapovanja, prikupljanja objekata i iscrtavanja novih nakon sto prethodni
//sa tog mesta nestanu prikupljanjem
void draw_objects(/*int animation_ongoing, double animation_parameter*/);
void draw_on_side();

//pronalazi koordinate objekata u nizu tj. njihov centar u odnosu na onu tacku
//na koju klikne korisnik, jer nemoguce je da korisnik klikne tacnu tacku
//u odnosu na koju se iscrtava objekat
void find_objects(GLdouble x_1, GLdouble y_1, GLdouble x_2, GLdouble y_2);

void move_on_x(double animation_parameter, int p);
void move_on_y(double animation_parameter, int p);

//menja i u matrici i nizu vrednosti
void change_values();

//pronalazi da li ima uklopljenih vrednosti
void find_a_match();

//pronalazi ima li u koloni tri
void find_column();
void find_row();

void column_detected();
void row_detected();

void collect_objects(pair array[]);

void drawString(float x, float y, float z, char* string);
void tekst();
void tekst2();

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
    /*column_array[0].i = -1;
    column_array[0].j = -1;
    column_array[1].i = -1;
    column_array[1].j = -1;
    column_array[2].i = -1;
    column_array[2].j = -1;*/
    matrix = make_matrix();
    //Matrica intova u odnosu na koju se iscrtavaju elementi
    draw(matrix);
    init_objects(matrix);
    num = (int)(rand()%5);
    printf("num: %d\n", num);

    initialize();
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
            
            //da li ovde????
            find_a_match();
            
            animation_ongoing = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            
            //animation_ongoing2 = 1;
            //glutTimerFunc(TIMER_INTERVAL, on_timer2, TIMER_ID);
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
    
    set_lighting();
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
    //set_lighting();
    //Pomocne koor. linije
    //draw_coordinate_lines();
    textures();

    //Iscrtavaju se dve table na kojima se nalaze elementi za igru
    draw_tables();
    //ispisuje tekst
    tekst();
    //Iscrtavanje elemenata sa kojima se igra
    draw_objects(/*animation_ongoing, animation_parameter*/);
    //Iscrtavanje elementa koji se prikuplja
    draw_on_side();
    
    
    if(skupljeno >= 12){
        tekst2();
        //exit(0);
    }
    
    //Nova slika se salje na ekran
    glutSwapBuffers();
    
}
//za animaciju swapovanja
static void on_timer(int value){
    
    if(value!=TIMER_ID){
        return;
    }
    
    animation_parameter += 0.2;
    
   if(animation_parameter > 1){
       
        change_values();
        find_a_match();
        animation_ongoing = 0;
        animation_parameter = 0;
        
        return;
        
    }
    
    
    glutPostRedisplay();
    
    if (animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    
}
//za animaciju postavljanja novih elemenata
static void on_timer2(int value){
    
    if(value!=TIMER_ID){
        return;
    }
    
    animation_parameter2 += 0.2;
    
   if(animation_parameter2 > 1){
       
        find_a_match();
        animation_ongoing2 = 0;
        animation_parameter2 = 0;
        
        return;
        
    }
    
    
    glutPostRedisplay();
    
    if (animation_ongoing2)
        glutTimerFunc(TIMER_INTERVAL, on_timer2, TIMER_ID);
    
}
//za animaciju malog iskakanja elementa koji se prikuplja 
//kako bi se skrenula paznja na uspeh
static void on_timer3(int value){
    
    if(value!=TIMER_ID){
        return;
    }
    
    animation_parameter3 += 0.2;
    
   if(animation_parameter3 > 1){
       
        animation_ongoing3 = 0;
        animation_parameter3 = 0;
        
        return;
        
    }
    
    
    glutPostRedisplay();
    
    if (animation_ongoing3)
        glutTimerFunc(TIMER_INTERVAL3, on_timer3, TIMER_ID);
    
}
//swapuje elemente u istom redu
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
                if(animation_parameter == 1.0){
                    glTranslatef(0.0, 0.0, -0.5);
                }
                //printf("x2: %f, x2 + ap: %f, z: %f\n",x, x + animation_parameter*(0.5*p), z);
                glTranslatef(x + animation_parameter*(0.5*p), y + 0.0, z + 0.5);
                draw_one(type);
            glPopMatrix();
                //objects[k].x += p*0.5;
        }
        else if(x == xp2 && y == yp2){
            glPushMatrix();
                glTranslatef(x + animation_parameter*(-0.5*p), y + 0.0, z + 0.0);
                draw_one(type);
            glPopMatrix();
                //objects[k].x -= p*0.5;
        }
        else{
            glPushMatrix();
                glTranslatef(x, y, z);
                draw_one(type);
            glPopMatrix();
        }
    }
    
}
//swapuje elemente u istoj koloni
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
                    glTranslatef(0.0, 0.0, -0.5);
                glTranslatef(x + 0.0, y + animation_parameter*(-0.5*p), z + 0.5);
                draw_one(type);
            glPopMatrix();
                //objects[k].y -= p*0.5;
        }
        else if(y == yp2 && x == xp2){
            glPushMatrix();
                glTranslatef(x + 0.0, y + animation_parameter*(0.5*p), z + 0.0);
                draw_one(type);
            glPopMatrix();
            //objects[k].y += p*0.5;
        }
        else{
            glPushMatrix();
                glTranslatef(x, y, z);
                draw_one(type);
            glPopMatrix();
        }
    }
}
//iscrtava iznova objekte na tablu u zavisnosti od dela igrice koji se trenutno desava
void draw_objects(){
    
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
    else if(animation_ongoing2 == 1){
        if(found_column){
            found_column = 0;
            column_detected();
            //find_a_match();
        }//ovde ti je bilo found_column
        else if(found_row){
            found_row = 0;
            row_detected();
            //find_a_match();
        }
        else if(found_row && found_column){
            found_row = 0;
            found_column = 0;
            row_detected();
            column_detected();
        }
        else{
            /*animation_ongoing = 0;
            animation_ongoing2 = 0;
            animation_ongoing3 = 0;*/
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
            find_a_match();
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
//nakon klika misa na objekte koji se swapuju odredjuje im vrednosti u matrici(nizu)
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
    
    for(int k = 0; k <= 35; k+=7){
        if(y_1 >= objects[k].y - h  && y_1 <= objects[k].y + h){
            yp1 = objects[k].y;
            i_1 = k/7;
        }
        if(y_2 >= objects[k].y - h  && y_2 <= objects[k].y + h){
            yp2 = objects[k].y;
            i_2 = k/7;
        }
    }
    
    //printf("xp1: %f, yp1: %f, xp2: %f, yp2: %f\n", xp1, yp1, xp2, yp2);
    //printf("Matrica: i1: %d, j1: %d, i2: %d, j2: %d\n", i_1, j_1, i_2, j_2);
}
//menja vrednosti u nizu i matrici nakon swapovanja objekata
void change_values(){
    /*pom_t = objects[0].type;
    printf("***** %f, %f *****\n", pom_x, objects[1].x);
    objects[0].type = objects[1].type;
    objects[1].type = pom_t;
    for(int k = 0; k < 5;k++)
        printf("novi type: %d %f %f\n", objects[k].type, objects[k].x, objects[k].y);*/
    int type;
    GLdouble x, y;
    int k1, type1;
    int k2, type2;
    
    for(int k = 0; k < OBJECTS_MAX; k++){
            
        type = objects[k].type;
        x = objects[k].x;
        y = objects[k].y;
            
        if(x == xp1 && y == yp1){
            k1 = k;
            type1 = type;
        }
        else if(x == xp2 && y == yp2){
            k2 = k;
            type2 = type;
            
        }
    }
    
    int pom_t = type1;
    objects[k1].type = type2;
    objects[k2].type = pom_t;
    
    /*for(int k = 0; k < 5;k++)
        printf("novi type: %d %f %f\n", objects[k].type, objects[k].x, objects[k].y);*/
    
    int matrica_pom  = matrix[i_1][j_1];
    matrix[i_1][j_1] = matrix[i_2][j_2];
    matrix[i_2][j_2] = matrica_pom;
    
    draw(matrix);
    
}
//trazi match u redu ili koloni, ovo je fja omotac oko naredne dve fje
void find_a_match(){
    
    /*find_column();
    find_row();*/
     int num_of_columns = 0;
        column_array[0].i = -1;
        column_array[0].j = -1;
        column_array[1].i = -1;
        column_array[1].j = -1;
        column_array[2].i = -1;
        column_array[2].j = -1;
    
    int num_of_rows = 0;
        row_array[0].i = -1;
        row_array[0].j = -1;
        row_array[1].i = -1;
        row_array[1].j = -1;
        row_array[2].i = -1;
        row_array[2].j = -1;
        
   
    for (int i = 0; i < 6; i++){
            for (int j = 0; j < 7; j++){
                if(i < 4 && matrix[i][j] == matrix[i+1][j] && matrix[i][j] == matrix[i+2][j]){
                    num_of_columns++;
                    found_column = 1;
                    column_array[0].i = i;
                    column_array[0].j = j;
                    column_array[1].i = i+1;
                    column_array[1].j = j;
                    column_array[2].i = i+2;
                    column_array[2].j = j;
                    //ovde ide animacija nestajanja objekata i postavljanja novih na njihovo mesto
                    collect_objects(column_array);
                   // break;
                }
                else if(j < 5 && matrix[i][j] == matrix[i][j+1] && matrix[i][j] == matrix[i][j+2]){
                    num_of_rows++;
                    found_row = 1;
                    row_array[0].i = i;
                    row_array[0].j = j;
                    row_array[1].i = i;
                    row_array[1].j = j+1;
                    row_array[2].i = i;
                    row_array[2].j = j+2;
                    //ovde ide animacija nestajanja objekata i postavljanja novih na njihovo mesto
                    collect_objects(row_array);
                    //break;
                }
            }
        }
}
//trazi match u koloni i ukoliko ga nadje poziva fju koja
//azurira vrednosti i poziva animaciju
/*void find_column(){
    
        int num_of_columns = 0;
        column_array[0].i = -1;
        column_array[0].j = -1;
        column_array[1].i = -1;
        column_array[1].j = -1;
        column_array[2].i = -1;
        column_array[2].j = -1;
        
		for (int j = 0; j < 7; j++){
            for (int i = 0; i <= 3; i++){
                if(matrix[i][j] == matrix[i+1][j] && matrix[i][j] == matrix[i+2][j]){
                    num_of_columns++;
                    found_column = 1;
                    column_array[0].i = i;
                    column_array[0].j = j;
                    column_array[1].i = i+1;
                    column_array[1].j = j;
                    column_array[2].i = i+2;
                    column_array[2].j = j;
                    //ovde ide animacija nestajanja objekata i postavljanja novih na njihovo mesto
                    collect_objects(column_array);
                   // break;
                }
            }
        }
        //collect_objects(column_array);
}*/
//trazi match u redu i ukoliko ga nadje poziva fju koja
//azurira vrednosti i poziva animaciju
/*void find_row(){
    
        int num_of_rows = 0;
       //pair column_array[3];
        row_array[0].i = -1;
        row_array[0].j = -1;
        row_array[1].i = -1;
        row_array[1].j = -1;
        row_array[2].i = -1;
        row_array[2].j = -1;
        
		for (int i = 0; i < 6; i++){
            for (int j = 0; j <= 4; j++){
                if(matrix[i][j] == matrix[i][j+1] && matrix[i][j] == matrix[i][j+2]){
                    num_of_rows++;
                    found_row = 1;
                    row_array[0].i = i;
                    row_array[0].j = j;
                    row_array[1].i = i;
                    row_array[1].j = j+1;
                    row_array[2].i = i;
                    row_array[2].j = j+2;
                    //ovde ide animacija nestajanja objekata i postavljanja novih na njihovo mesto
                    collect_objects(row_array);
                    //break;
                }
            }
        }
        //collect_objects(row_array);
       // found_row = 0;

}*/
// //fja koja azurira vrednosti nakon matchovanja i poziva animaciju
void collect_objects(pair array[]){
    /*printf("[%d][%d], [%d][%d], [%d][%d]\n\n", column_array[0].i, column_array[0].j, column_array[1].i, column_array[1].j, column_array[2].i, column_array[2].j);*/
    int i = array[0].i;
    int j = array[0].j;
    int i_1 = array[1].i;
    int j_1 = array[1].j;
    int i_2 = array[2].i;
    int j_2 = array[2].j;
    
    
    int k = array[0].i * 7 + array[0].j;
    int type = objects[k].type;
    
    int k1 = array[1].i * 7 + array[1].j;
    int type1 = objects[k1].type;
    
    int k2 = array[2].i * 7 + array[2].j;
    int type2 = objects[k2].type;
    
    /*if(type == num)
        found_num = 1;
    if(found_num)
        collect_side_element();*/
    
    //mozda ovaj kod moze da prosiri tako da gleda celu matricu da nema match nakon jednog swap
    do{
        proba = (int)rand()%5;
    }
    while(proba == type);
    
    do{
        proba1 = (int)rand()%5;
    }
    while(proba1 == type1 || proba1 == proba);
    do{
        proba2 = (int)rand()%5;
    }
    while(proba2 == type2 || proba2 == proba1 || proba2 == proba);
    
    matrix[i][j] = proba;
    matrix[i_1][j_1] = proba1;
    matrix[i_2][j_2] = proba2;
    
    /*int color_type;
    
    for (i = 0; i < 6; i++){
		for (j = 0; j < 7; j++){
            do{
                color_type = (int)(rand()%5);
            }
            while (      
                         //da nemam tri u redu
                         (j >= 2 &&
                         matrix[i][j-1] == color_type &&
                         matrix[i][j-2] == color_type)
                    ||
                         //da nemam tri u koloni
                         (i >= 2 &&
                         matrix[i-1][j] == color_type &&
                         matrix[i-2][j] == color_type));
            
            //if(j>=2 || i>=2)
			matrix[i][j] = color_type;
		}
	}
	
	init_objects(matrix);*/
    
   // printf("*****");
    //draw(matrix);
    
    animation_ongoing2 = 1;
    glutTimerFunc(TIMER_INTERVAL, on_timer2, TIMER_ID);
    
    
}
//iscrtava element na levoj(manjoj) tabli uz pomoc funkcije iz draw.c
//i vodeci racuna o 
void draw_on_side(){
    
        if(found_num == 1){
            found_num = 0;
            glPushMatrix();
                glTranslatef(0.0, 0.0, 0.2);
                glTranslatef(0.0, 0.0, -0.2*animation_parameter3);
                draw_side_one(num);
            glPopMatrix();
        }
        else{
            draw_side_one(num);
        }
}
//vrsi animaciju dolaska novih elemenata na mesto onih koji su prikupljeni(matchovani)
//u slucaju da su matchovana tri elementa u koloni  i vodi racuna o tome je li matchovan onaj 
//koji se prikupljama i u slucaju da jeste uvecava njegov brojac
void column_detected(){
    int type;
    GLdouble x, y, z;
    int r = column_array[0].i * 7 + column_array[0].j;
        int typer = objects[r].type;
        if(typer == num){
            found_num = 1;
            skupljeno += 3;
            //if(skupljeno >= 12)
                //exit(0);
            animation_ongoing3 = 1;
            glutTimerFunc(TIMER_INTERVAL3, on_timer3, TIMER_ID);
        }
        for(int k = 0; k < OBJECTS_MAX; k++){
                
            type = objects[k].type;
            x = objects[k].x;
            y = objects[k].y;
            z = objects[k].z;
            
            if(k == column_array[0].i * 7 + column_array[0].j){
                objects[k].type = proba;
                glPushMatrix();
                    glTranslatef(x, y, z/*-1.0 + animation_parameter2*/);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
            }
            else if(k == column_array[1].i * 7 + column_array[1].j){
                objects[k].type = proba1;
                glPushMatrix();
                    glTranslatef(x, y, z/*-1.0 + animation_parameter2*/);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
            }
            else if(k == column_array[2].i * 7 + column_array[2].j){
                objects[k].type = proba2;
                glPushMatrix();
                    glTranslatef(x, y, z/*-1.0 + animation_parameter2*/);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
                
            }
            else{
            glPushMatrix();
                glTranslatef(x, y, z);
                draw_one(type);
            glPopMatrix();
            }
        }
}
//vrsi animaciju dolaska novih elemenata na mesto onih koji su prikupljeni(matchovani)
//u slucaju da su matchovana tri elementa u redu i vodi racuna o tome je li matchovan onaj 
//koji se prikupljama i u slucaju da jeste uvecava njegov brojac
void row_detected(){
    int type;
    GLdouble x, y, z;
    int r = row_array[0].i * 7 + row_array[0].j;
        int typer = objects[r].type;
        if(typer == num){
            found_num = 1;
            skupljeno += 3;
            //if(skupljeno >= 12)
                //exit(0);
            animation_ongoing3 = 1;
            glutTimerFunc(TIMER_INTERVAL3, on_timer3, TIMER_ID);
        }
        for(int k = 0; k < OBJECTS_MAX; k++){
                
            type = objects[k].type;
            x = objects[k].x;
            y = objects[k].y;
            z = objects[k].z;
            
            if(k == row_array[0].i * 7 + row_array[0].j){
                objects[k].type = proba;
                glPushMatrix();
                    glTranslatef(x, y, z/*-1.0 + animation_parameter2*/);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
            }
            else if(k == row_array[1].i * 7 + row_array[1].j){
                objects[k].type = proba1;
                glPushMatrix();
                    glTranslatef(x, y, z/*-1.0 + animation_parameter2*/);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
            }
            else if(k == row_array[2].i * 7 + row_array[2].j){
                objects[k].type = proba2;
                glPushMatrix();
                    glTranslatef(x, y, z/*-1.0 + animation_parameter2*/);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
                
            }
            else{
            glPushMatrix();
                glTranslatef(x, y, z);
                draw_one(type);
            glPopMatrix();
            }
        }
}
//ispisuje tekst na ekran o tome koliko je elemenata ostalo za prikupiti do kraja
void drawString(float x, float y, float z, char *string) {
    
    int n = strlen(string);
    glPushMatrix();
        glNormal3f(1.0,0.0,0.0);
        glColor3f(0.6,0.2,0.4);
        glRasterPos3f(x, y, z);

        for (int i = 0; i < n; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
        }
    glPopMatrix();
}
void tekst(){
    char prikupljeno[20];
    
    sprintf(prikupljeno, "%d / 12", skupljeno);
    
    drawString(-2.75, 0.2, 0.5, prikupljeno);
}
void tekst2(){
    char prikupljeno[20];
    
    sprintf(prikupljeno, "Congrats: %d / 12!", skupljeno);
    
   // drawString(-0.5, 2.0, 0.5, prikupljeno);
    drawString(-2.95, -0.3, 0.5, prikupljeno);
}
