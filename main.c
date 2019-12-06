#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>

#define check_error(expr, Usermsg)\
    do{\
        if(!(expr)){\
            fprintf(stderr, "%s\n", Usermsg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)
//Dimenzije prozora
static int window_width, window_height;

int** matrix;

//Deklaracije callback funkcija
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
int random_at_most(int max);
int** make_matrix();
void draw(int** matrix);
void draw_objects(int** matrix);
void draw_one();
void set_light();

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
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    //Obavlja se OpenGL inicijalizacija
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2);

    //Program ulazi u glavnu petlju
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
    case 27:
        //Zavrsava se program
        exit(0);
        break;
    }
}

static void on_reshape(int width, int height){
    //Pamte se sirina i visina prozora
    window_width = width;
    window_height = height;
}

static void on_display(void){
    
    set_light();
    //Brise se prethodni sadrzaj prozora
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Podesava se viewport
    glViewport(0, 0, window_width, window_height);

    //Podesava se projekcija
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

    //Podesava se tacka pogleda
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 5.0, 
               0.0, 0.0, 0.0, 
               0.0, 1.0, 0.0);
    
    //Iscrtavanje x ose
    glPushMatrix();
        glColor3f(1,0,0);
        glBegin(GL_LINES);
            glVertex3f(100,0,0);
            glVertex3f(-100,0,0);
        glEnd();
    glPopMatrix();

    //Iscrtavanje y ose
    glPushMatrix();
        glColor3f(0,1,0); 
        glBegin(GL_LINES); 
            glVertex3f(0,-100,0);
            glVertex3f(0,100,0);
        glEnd();
    glPopMatrix();

    //Iscrtavanje z ose
    glPushMatrix();
        glColor3f(0,0,1); 
        glBegin(GL_LINES);
            glVertex3f(0,0,100);
            glVertex3f(0,0,-100);
        glEnd();
    glPopMatrix();

   //Podesava se prvi plavi pravougaonik
    glColor3f(0.74902, 0.847059, 0.847059);
    
   
    glBegin(GL_POLYGON);
        glVertex3f (-1.8, -1.8, 0.0);
        glVertex3f (-1.8, 1.8, 0.0);
        glVertex3f (1.8, 1.8, 0.0);
        glVertex3f (1.8, -1.8, 0.0);
    glEnd();
    
    //Podesava se drugi zeleni pravougaonik
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

    matrix = make_matrix();
    draw(matrix);
    draw_objects(matrix);
    draw_one();
    
    //Nova slika se salje na ekran
    glutSwapBuffers();
    
}


// Pp 0 <= max <= RAND_MAX
// Vraca u intervalu [0, max]
int random_at_most(int max) {
  unsigned int
    // max <= RAND_MAX < ULONG_MAX
    num_bins = (unsigned int) max + 1,
    num_rand = (unsigned int) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  int x;
  do {
   x = rand();
  }
  
  while (num_rand - defect <= (unsigned int)x);

  return x/bin_size;
}

int** make_matrix(){
	
	int** matrix;
	
	matrix = malloc(6 * sizeof(int*));
	check_error(matrix != NULL, "Neuspesna alokacija memorije!");

	int i, j;
	for(i = 0; i < 6; i++){
		matrix[i] = malloc(7 * sizeof(int));
		check_error(matrix[i] != NULL, "Neuspesna alokacija memorije!");
	}

	for (i = 0; i < 6; i++){
		for (j = 0; j < 7; j++){
			matrix[i][j] = random_at_most(4);
		}
	}

	return matrix;
}

void draw(int** matrix){
int i, j;

    for (i = 0; i < 6; i++){
		for (j = 0; j < 7; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

void draw_objects(int** matrix){

    int i, j;

    glTranslatef(-0.3, 0.55, 0.0);
    for(i=0; i<6; i++){
        for(j=0; j<7; j++){
            //Zuta sfera
            if(matrix[i][j] == 0){
                glPushMatrix();
                    glColor3f(1, 1, 0);
                    glTranslatef(1.8 - 0.5*j, -1.8 + 0.5*i, 0.0);
                    //glScalef(1, 2, 1);
                    glutSolidSphere(0.2, 200, 200);
                glPopMatrix();
            }
            //Crvena kocka
            else if(matrix[i][j] == 1){
                 glPushMatrix();
                    glColor3f(1, 0, 0);
                    glTranslatef(1.8 - 0.5*j, -1.8 + 0.5*i, 0.0);
                    //glScalef(1, 2, 1);
                    glutSolidCube(0.3);
                 glPopMatrix();
            }
            //Zeleni konus
            else if(matrix[i][j] == 2){
                glPushMatrix();
                    glColor3f(0, 1, 0);
                    glTranslatef(1.8 - 0.5*j, -1.8 + 0.47*i, 0.0);
                    glRotatef(-90, 1, 0, 0);
                    //glScalef(1, 2, 1);
                    glutSolidCone(0.2, 0.25, 100, 100);
                glPopMatrix();
            }
            //Plavi cajnik
               else if(matrix[i][j] == 3){
                glPushMatrix();
                    glColor3f(0, 0, 1);
                    glTranslatef(1.8 - 0.5*j, -1.8 + 0.5*i, 0.0);
                    glScalef(1, 2, 1);
                    glutSolidTeapot(0.15);
                glPopMatrix();
            }
            //Ljubicast torus
           else{
                 glPushMatrix();
                    glColor3f(0.7, 0.1, 0.9);
                    glTranslatef(1.8 - 0.5*j, -1.8 + 0.5*i, 0.0);
                    glRotatef(180, 1, 0, 0);
                    //glScalef(1, 2, 1);
                    glutSolidTorus(0.06, 0.15, 100, 100);
                 glPopMatrix();
            }
        }
    }
}

void draw_one(){
    
    int broj = random_at_most(4);
    
    //Zuta sfera
    if(broj == 0){
        glPushMatrix();
            glColor3f(1, 1, 0);
            glTranslatef(-3.35 + 0.77, 1.2 - 1, 0.0);
            //glScalef(1, 2, 1);
            glutSolidSphere(0.15, 200, 200);
        glPopMatrix();
    }
    //Crvena kocka
    else if(broj == 1){
        glPushMatrix();
            glColor3f(1, 0, 0);
            glTranslatef(-3.35 + 0.77, 1.2 - 1, 0.0);
            //glScalef(1, 2, 1);
            glutSolidCube(0.3);
        glPopMatrix();
    }
    //Zeleni konus
    else if(broj == 2){
        glColor3f(0, 1, 0);
            glTranslatef(-3.35 + 0.77, 1.2 - 1, 0.0);
            glRotatef(-90, 1, 0, 0);
            glRotatef(-5, 0, 1, 1);
            glRotatef(3, 1, 0, 0);
            //glScalef(1, 2, 1);
            glutSolidCone(0.2, 0.25, 100, 100);
        glPopMatrix();
    }
    //Plavi cajnik
    else if(broj == 3){
        glPushMatrix();
            glColor3f(0, 0, 1);
            glTranslatef(-3.35 + 0.77, 1.2 - 1, 0.0);
            //glScalef(1, 2, 1);
            glutSolidTeapot(0.15);
        glPopMatrix();
    }
    //Ljubicast torus
    else{
        glPushMatrix();
            glColor3f(0.7, 0.1, 0.9);
            glTranslatef(-3.35 + 0.77, 1.2 - 1, 0.0);
            glRotatef(180, 1, 0, 0);
            //glScalef(1, 2, 1);
            glutSolidTorus(0.06, 0.15, 100, 100);
        glPopMatrix();
    }
}

void set_light(){
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

    // Ukljucuje se osvjetljenje i podesavaju parametri svetla
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
}
