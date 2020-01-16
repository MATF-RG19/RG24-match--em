#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define TIMER_INTERVAL (30)
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
/*struktura u kojoj cuvam parove indeksa objekata koji predstavljaju match 
dakle ako se nadju tri ista u koloni ili tri ista u redu 
pamte se njihove pozicije u matrici na kojima su nadjeni
kako bi mogle nove vrednosti da se postave u matricu i niz */
typedef struct Pair{
    
    int i;
    int j;
    
}pair;
//koliko je trenutno skupljeno objekata trazenog tipa(onog koji je iscrtan na levoj tabli)
int skupljeno = 0;
//flag koji sluzi kao pomoc sta treba iscrtati u on_display nakon sto se igrica ponovo pokrene na s ili S
int flag = 0;
//dimenzije prozora
static int window_width, window_height;

/*za animaciju swapovanja objekata
 * animacija je da objekti polako prelaze jedan preko drugog
 * bez medjusobnog dodirivanja
 * animacija je odradjena transliranjem po x ili y osi
 * u zavisnosti je li red ili kolona*/
int animation_ongoing = 0;
double animation_parameter = 0;

/*za animaciju dolazenja novih elemenata na mesto onih koji su prikupljeni
 * animacija je takva da umesto objekata koji su cinili match
 * dolaze novi objekti naizgled iz daljine i jako mali
 * a zatim sve blizi korisniku i samim tim veci
 * animacija je odradjena skaliranjem*/
int animation_ongoing2 = 0;
double animation_parameter2 = 0;

/*za animaciju objekta na levoj tabli koji se prikuplja, 
 * a koja ima za cilj da korisniku privuce paznju da je prikupio tri ista
 * animacija se zasniva na blagom iskakanju elementa
 * animacija je odradjena transliranjem po za osi*/
int animation_ongoing3 = 0;
double animation_parameter3 = 0;

//matrica tipova objekata(intova)
int** matrix;

//objekat koji prikupljamo
int num;
//identifikator da li je objekat prikupljen
int found_num = 0;

//identifikator da li su pronadjena tri ista elementa u koloni
int found_column = 0;
//pozicije objekata koji cine match u koloni
pair column_array[3];

//identifikator da li su pronadjena tri ista elementa u redu
int found_row = 0;
//pozicije objekata koji cine match u redu
pair row_array[3];

//x i y u wcs
GLdouble x_1, y_1, x_2, y_2;
//pronadjeni x i y u odnosu na niz
GLdouble xp1, yp1, xp2, yp2;

//pomocne promenljive za fju change_values
int i_1, i_2, j_1, j_2;

//koriste se prilikom pravljenja nove matrice nakon spajanja tri ista elementa
//kako nova izgenerisana matrica ne bi inicijalno imala tri ista el. u redu/koloni
int proba, proba1, proba2;

//Deklaracije callback funkcija
static void on_display(void);
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse(int button, int state, int x, int y);

//za animaciju swapovanja
static void on_timer(int value);
//za animaciju dolazenja objekata
static void on_timer2(int value);
//za iskakanje elementa sa leve strane
static void on_timer3(int value);

//fja koja iscrtava objekte na veliku tablu nakon svake animacije
//tj. nakon swapovanja, prikupljanja objekata i iscrtavanja novih nakon sto prethodni
//sa tog mesta nestanu prikupljanjem
void draw_objects();
//iscrtava element na levoj tabli
void draw_on_side();

//pronalazi koordinate objekata u nizu tj. njihov centar u odnosu na onu tacku
//na koju klikne korisnik, jer skoro je nemoguce da korisnik klikne na tacnu tacku
//u odnosu na koju se iscrtava objekat
void find_objects(GLdouble x_1, GLdouble y_1, GLdouble x_2, GLdouble y_2);

//za swapovanje u redu
void move_on_x(double animation_parameter, int p);
//za swapovanje u koloni
void move_on_y(double animation_parameter, int p);

//menja i u matrici i nizu vrednosti
void change_values();

//pronalazi da li ima tri iste vrednosti u redu ili koloni
void find_a_match();

//umesto na tri ista pronadjena u koloni postavlja nove elemente u niz koji su vec izgenerisani u matrici
void column_detected();
//umesto na tri ista pronadjena u redu postavlja nove elemente u niz koji su vec izgenerisani u matrici
void row_detected();

//generise nove objekte koji dolaze umesto starih i menja vrednost u matrici
void collect_objects(pair array[]);

//pomocna fja za ispis teksta
void drawString(float x, float y, float z, char* string);
//ispisuje tekst o broju trenutno prikupljenih
void tekst();
//ispisuje tekst o pobedi
void tekst2();
//ispisuje tekst o tome kako se ponovo moze odigrati nova partija
void tekst3();

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
    //draw(matrix);
    //popunjava se niz u odnosu na matricu
    init_objects(matrix);
    //randoom objekat koji se skuplja
    num = (int)(rand()%5);
    
    //za teksture
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
          
           if(x_1 <= -1.8 || x_2 >= 1.8 || y_1 >= 1.8 || y_2 <= -1.8){
               printf("Izvan table ste!\n");
           }
           else if(fabs(x_1 - x_2) >= 0.95 || (fabs(x_1 - x_2) <= 0.4 && fabs(y_1 - y_2) <= 0.1)){
               printf("Pokusavate u istom redu nesusedne!\n");
            }
            else if(fabs(y_1 - y_2) >= 0.95 || (fabs(y_1 - y_2) <= 0.4 && fabs(x_1 - x_2) <= 0.1) ){
                printf("Pokusavate u istoj koloni nesusedne!\n");
            }
           else if((x_2 - x_1 > 0.4 && y_2 - y_1 > 0.1) 
                || (x_2 - x_1 > 0.4 && y_1 - y_2 > 0.1)
                || (x_1 - x_2 > 0.4 && y_2 - y_1 > 0.1)
                || (x_1 - x_2 > 0.4 && y_1 - y_2 > 0.1))
           {
                printf("Pokusavate dijagonalno!\n");
           }
           //potez je regularan i moze se preci na pronalazenje objekata na koje je korisnik kliknuo
            else{
                find_objects(x_1, y_1, x_2, y_2);
                animation_ongoing = 1;
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            }
                
           
            
    }
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
    case 27:
        //Zavrsava se program
        exit(0);
        break;
    //za novu partiju
    case 's':
    case 'S':
        flag = 0;
        skupljeno = 0;
        num = (int)(rand()%5);
        matrix = make_matrix();
        init_objects(matrix);
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
    
    //postavlja je osvetljenje
    set_lighting();
    
    //Brise se prethodni sadrzaj prozora
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    //Podesava se viewport
    glViewport(0, 0, window_width, window_height);

    //Podesava se projekcija
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20);

    //Podesava se tacka pogleda
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 5.0, 
               0.0, 0.0, 0.0, 
               0.0, 1.0, 0.0);
    
    //Pomocne koor. linije
    //draw_coordinate_lines();
    
    //postavljaju se teksture
    textures();

    //Iscrtavaju se dve table na kojima se nalaze elementi za igru
    draw_tables();
    //ispisuje tekst o kolicini skupljenih
    if(flag == 0)
    tekst();
    //Iscrtavanje elemenata sa kojima se igra
    if(flag == 0)
    draw_objects();
    //Iscrtavanje elementa koji se prikuplja
    if(flag == 0)
    draw_on_side();
    
    //info o pobedi
    if(skupljeno >= 12){
        flag = 1;
        tekst2();
        tekst3();
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
    
   //nakon swapovanja izmeniti vrednosti u nizu/matrici i potraziti postoje li 3 iste u redu/koloni
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
       
        animation_ongoing2 = 0;
        animation_parameter2 = 0;
        //potraziti da li postavljanjem novih nastaju 3 iste jedna do druge u redu/koloni
        find_a_match();
        
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
        find_a_match();
        
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
    
    //prolazim kroz niz koji sadrzi sve objekte i pitam
    //koji ima trazene koordinate na koje je korisnik kliknuo
    //parametar p samo govori u kom smeru ide swap
    //u zavisnosti na koji element je korisnik prvo kliknuo
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
                glTranslatef(x + animation_parameter*(0.5*p), y + 0.0, z + 0.5);
                draw_one(type);
            glPopMatrix();
        }
        else if(x == xp2 && y == yp2){
            glPushMatrix();
                glTranslatef(x + animation_parameter*(-0.5*p), y + 0.0, z + 0.0);
                draw_one(type);
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
//swapuje elemente u istoj koloni
void move_on_y(double animation_parameter, int p){
    
    int type;
    GLdouble x, y, z;
    
    //prolazim kroz niz koji sadrzi sve objekte i pitam
    //koji ima trazene koordinate na koje je korisnik kliknuo
    //parametar p samo govori u kom smeru ide swap
    //u zavisnosti na koji element je korisnik prvo kliknuo
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
        }
        else if(y == yp2 && x == xp2){
            glPushMatrix();
                glTranslatef(x + 0.0, y + animation_parameter*(0.5*p), z + 0.0);
                draw_one(type);
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
//iscrtava iznova objekte na tablu u zavisnosti od dela igrice koji se trenutno desava
void draw_objects(){
    int type;
    GLdouble x, y, z;
    
    //swap
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
    //postavljanje novih objekata
    else if(animation_ongoing2 == 1){
        if(found_column){
            found_column = 0;
            column_detected();
        }
        else if(found_row){
            found_row = 0;
            row_detected();
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
    //nema promene
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
    
    //odredjuje x koor kliknutih objekata
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
    
    //odredjuje y koor kliknutih objekata
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
}
//menja vrednosti u nizu i matrici nakon swapovanja objekata
void change_values(){
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

    
    int matrica_pom  = matrix[i_1][j_1];
    matrix[i_1][j_1] = matrix[i_2][j_2];
    matrix[i_2][j_2] = matrica_pom;
    
    //draw(matrix);
    
}
//trazi match u redu ili koloni, ovo je fja omotac oko naredne dve fje
void find_a_match(){
    
    //cuva koja tri objekta cine kolonu istih
     int num_of_columns = 0;
        column_array[0].i = -1;
        column_array[0].j = -1;
        column_array[1].i = -1;
        column_array[1].j = -1;
        column_array[2].i = -1;
        column_array[2].j = -1;
    //cuva koja tri objekta cine red istih
    int num_of_rows = 0;
        row_array[0].i = -1;
        row_array[0].j = -1;
        row_array[1].i = -1;
        row_array[1].j = -1;
        row_array[2].i = -1;
        row_array[2].j = -1;
        
   
    for (int i = 0; i < 6; i++){
            for (int j = 0; j < 7; j++){
                //provera za kolonu
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
                    return;
                }
                //provera za red
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
                    return;
                }
            }
        }
}
// //fja koja azurira vrednosti nakon matchovanja i poziva animaciju
void collect_objects(pair array[]){
    //da ne brinemo jesmo li prosledili kolonu ili red posto se analogno radi
    int i = array[0].i;
    int j = array[0].j;
    int i_1 = array[1].i;
    int j_1 = array[1].j;
    int i_2 = array[2].i;
    int j_2 = array[2].j;
    
    //na osnovu pozicija u matrici trazim u nizu
    int k = array[0].i * 7 + array[0].j;
    int type = objects[k].type;
    
    int k1 = array[1].i * 7 + array[1].j;
    int type1 = objects[k1].type;
    
    int k2 = array[2].i * 7 + array[2].j;
    int type2 = objects[k2].type;
    
    //generisem tri razlicita nova objekta
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
    
    //menjam vrednosti u matrici
    matrix[i][j] = proba;
    matrix[i_1][j_1] = proba1;
    matrix[i_2][j_2] = proba2;
    
    animation_ongoing2 = 1;
    glutTimerFunc(TIMER_INTERVAL, on_timer2, TIMER_ID);
    
    
}
//iscrtava element na levoj(manjoj) tabli uz pomoc funkcije iz draw.c
//i vodeci racuna o 
void draw_on_side(){
    
        //ukoliko je pronadjen obavesti korisnika iskakanjem
        if(found_num == 1){
            found_num = 0;
            glPushMatrix();
                glTranslatef(0.0, 0.0, 0.2);
                glTranslatef(0.0, 0.0, -0.2*animation_parameter3);
                draw_side_one(num);
            glPopMatrix();
        }
        //inace samo ga iscrtaj
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
                    glTranslatef(x, y, z);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
            }
            else if(k == column_array[1].i * 7 + column_array[1].j){
                objects[k].type = proba1;
                glPushMatrix();
                    glTranslatef(x, y, z);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
            }
            else if(k == column_array[2].i * 7 + column_array[2].j){
                objects[k].type = proba2;
                glPushMatrix();
                    glTranslatef(x, y, z);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
                
            }
            //nema promena
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
                    glTranslatef(x, y, z);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
            }
            else if(k == row_array[1].i * 7 + row_array[1].j){
                objects[k].type = proba1;
                glPushMatrix();
                    glTranslatef(x, y, z);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
            }
            else if(k == row_array[2].i * 7 + row_array[2].j){
                objects[k].type = proba2;
                glPushMatrix();
                    glTranslatef(x, y, z);
                    glScalef(animation_parameter2, animation_parameter2, animation_parameter2);
                    draw_one(objects[k].type);
                glPopMatrix();
                
            }
            //nema promena
            else{
            glPushMatrix();
                glTranslatef(x, y, z);
                draw_one(type);
            glPopMatrix();
            }
        }
}
//pomocna fja za ispis teksta
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
//ispisuje tekst na ekran o tome koliko je elemenata ostalo za prikupiti do kraja
void tekst(){
    char prikupljeno[20];
    
    sprintf(prikupljeno, "%d / 12", skupljeno);
    
    drawString(-2.75, 0.2, 0.5, prikupljeno);
}
//ispisuje tekst na ekran o pobedi
void tekst2(){
    char prikupljeno[70];
    
    sprintf(prikupljeno, "Congrats: %d / 12! ~(^-^)~", skupljeno);
    drawString(-0.5, 0.0, 0.1, prikupljeno);
}
//ispisuje tekst na ekran o tome kako odigrati novu partiju
void tekst3(){
    char ispis[70];
    
    sprintf(ispis, "To play again press 's' or 'S'");
    
   // drawString(-0.5, 2.0, 0.5, prikupljeno);
    //drawString(-2.95, -0.3, 0.5, prikupljeno);
    drawString(-0.5, -0.2, 0.1, ispis);
}
