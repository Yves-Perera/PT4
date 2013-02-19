#ifndef MODEL_H 
#define MODEL_H 

#define NB_MATRICE 400
#define NB_THREADS 4
#define multiple 2
#define vitesse 0.2f

extern short int first;
extern short int next;
extern bool *** matrice;
extern short int *** voisins;
void init();
void deleteMatrice();

#endif 
