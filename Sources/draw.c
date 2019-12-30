#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "../Headers/draw.h"

#define check_error(expr, Usermsg)\
    do{\
        if(!(expr)){\
            fprintf(stderr, "%s\n", Usermsg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)

//iscrtava table tj. pravougaonike na kojima su smesteni objekti
void draw_tables(){
    //Podesava se prvi plavi pravougaonik, veci
    glColor3f(0.74902, 0.847059, 0.847059);
    
   
    glBegin(GL_POLYGON);
        glNormal3f(1.0,0.0,0.0);
        glVertex3f (-1.8, -1.8, 0.0);
        glVertex3f (-1.8, 1.8, 0.0);
        glVertex3f (1.8, 1.8, 0.0);
        glVertex3f (1.8, -1.8, 0.0);
    glEnd();
    
    //Podesava se drugi zeleni pravougaonik, manji
    glColor3f(0.560784, 0.737255, 0.560784);
    
     glBegin(GL_POLYGON);
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f (-3.35, -1.2, 0.0);
        glVertex3f (-3.35, 1.2, 0.0);
        glVertex3f (-2.35, 1.2, 0.0);
        glVertex3f (-2.35, -1.2, 0.0);
    glEnd();

}
//alociram matricu na osnovu koje imam randomizaciju
//i kasnije pracenje elemenata koji su uklonjeni sa table
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
			matrix[i][j] = (int)(rand()%5);
		}
	}

	return matrix;
}
//samo ispisivanje matrice u terminal radi provere
void draw(int** matrix){
int i, j;

    for (i = 0; i < 6; i++){
		for (j = 0; j < 7; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}
//inicijalno postavljeni objekti u niz u odnosu na matricu
void init_objects(int** matrix){
    
        int k = 0;
        while(k < OBJECTS_MAX){
            for(int i = 0; i < 6;i++){
                for(int j = 0;j < 7;j++){
                    objects[k].type = matrix[i][j];
                    objects[k].x = -1.5 + 0.5*j;
                    objects[k].y = 1.25 - 0.5*i;
                    k++;
                }
            }
        }
        
        for(int k = 0; k < OBJECTS_MAX; k++){
                printf("type: %d,x: %lf,y: %lf,z: %lf\n", objects[k].type, objects[k].x, objects[k].y, objects[k].z);
        }
        
    
}
//crta odredjeni oblik u odredjenoj boji u odnosu na prosledjenu vrednost
void draw_one(int type){
    
    switch(type){
        //zuta sfera
        case 0 :
            glColor3f(1, 1, 0);
            glutSolidSphere(0.2, 200, 200);
            break;
        //crvena kocka
        case 1:
            glColor3f(1, 0, 0);
            glutSolidCube(0.3);
            break;
        //zelena kocka
        case 2:
            glColor3f(0, 1, 0);
            glutSolidCube(0.3);
            break;
        //plavi cajnik
        case 3:
            glScalef(1, 2, 1);
            glColor3f(0, 0, 1);
            glutSolidTeapot(0.13);
            break;
        //Ljubicasti torus
        case 4:
            glColor3f(0.7, 0.1, 0.9);
            glutSolidTorus(0.06, 0.15, 100, 100);
            break;
        default:
            printf("Pogresna vrednost");
            break;
    }
}

//iscrtavanje elementa koji se skuplja
//on se nalazi na levoj(manjoj) tabli
void draw_on_side(int broj){
    
    //Zuta sfera
    if(broj == 0){
        glPushMatrix();
            glColor3f(1, 1, 0);
            glTranslatef(-2.90, 0.8, 0.0);
            glutSolidSphere(0.2, 200, 200);
        glPopMatrix();
    }
    //Crvena kocka
    else if(broj == 1){
        glPushMatrix();
            glColor3f(1, 0, 0);
            glTranslatef(-2.90, 0.8, 0.0);
            glutSolidCube(0.3);
        glPopMatrix();
    }
    //Zeleni konus
    else if(broj == 2){
        glPushMatrix();
            glColor3f(0, 1, 0);
            glTranslatef(-2.90, 0.8, 0.0);
            glutSolidCube(0.3);
        glPopMatrix();
    }
    //Plavi cajnik
    else if(broj == 3){
        glPushMatrix();
            glColor3f(0, 0, 1);
            glTranslatef(-2.90, 0.8, 0.0);
            glScalef(1, 2, 1);
            glutSolidTeapot(0.15);
        glPopMatrix();
    }
    //Ljubicast torus
    else{
        glPushMatrix();
            glColor3f(0.7, 0.1, 0.9);
            glTranslatef(-2.90, 0.8, 0.0);
            glutSolidTorus(0.06, 0.15, 100, 100);
        glPopMatrix();
    }
}

