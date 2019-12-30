#ifndef _DRAW_H_
#define _DRAW_H

#define OBJECTS_MAX (42)

typedef struct Object{
    
    int type;
    GLdouble x, y, z;
    
}object;

object objects[OBJECTS_MAX];

//crtanje pravougaonika(tabli) na kojima su objekti
extern void draw_tables();

//pravljenje i ispisivanje matrice u terminal
//matrica samo sadrzi intove na osnovu kojih crtam objekat odredjene boje i oblika
//i kasnije za pracenje elemenata koje treba docrtati kada korisnik pokupi tri ili vise istih
extern int** make_matrix();
extern void draw(int** matrix);

//niz elemenata tipa object, svaki element niza predstavlja jednu celiju iz matrice
//za svaki element cuvam koji objekat treba iscratati i na kojoj poziciji
extern void init_objects(int** matrix);
//pomocna fja za iscrtavanje objekta odredjenog oblika i boje
//koristi je fja draw objects koja se poziva iz maina i sluzi da iscrta sve objekte na tablu
extern void draw_one(int type);
//samo iscrtava element na maloj(levoj) tabli
extern void draw_on_side(int broj);

#endif
