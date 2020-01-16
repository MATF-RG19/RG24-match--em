#ifndef _DRAW_H_
#define _DRAW_H

#define OBJECTS_MAX (42)

/* struktura kojom opisujem pojedinacne objekte na tabli
 * za svaki objekat se cuva njegova pozicija na tabli
 * i tip koji moze biti: 0 - zuta sfera
 *                       1 - crveni kvadar
 *                       2 -zeleni kvadar
 *                       3 - plavi cajnik
 *                       4 - ljubicasti torus*/
typedef struct Object{
    
    int type;
    GLdouble x, y, z;
    
}object;

/*niz u koji stavljam sve trenutne objekte na tabli
 * kako tabla ima 6 redova i 7 kolona, niz svaki put ima 42 elementa*/
object objects[OBJECTS_MAX];

//crtanje pravougaonika(tabli) na kojima su objekti
extern void draw_tables();

//pravljenje i ispisivanje matrice u terminal
//matrica samo sadrzi intove na osnovu kojih crtam objekat odredjene boje i oblika 
//(tj. svaki int predstavlja odredjeni tip definisan u strukturi object)
//i kasnije za pracenje elemenata koje treba docrtati kada korisnik pokupi tri ista
extern int** make_matrix();
//ispisivanje matrice u terminal radi lakseg rada
extern void draw(int** matrix);

//niz elemenata tipa object, svaki element niza predstavlja jednu celiju iz matrice
//za svaki element cuvam koji objekat treba iscratati i na kojoj poziciji
extern void init_objects(int** matrix);
//ispisivanje niza objects u terminal radi lakseg rada
extern void print_objects();

//pomocna fja za iscrtavanje objekta odredjenog oblika i boje
//koristi je fja draw objects koja se poziva iz maina i sluzi da iscrta sve objekte na tablu
extern void draw_one(int type);
//iscrtava element na maloj(levoj) tabli
extern void draw_side_one(int broj);

#endif
