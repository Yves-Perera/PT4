#ifndef MODEL_H 
#define MODEL_H 

extern short int first;
extern short int next;
extern bool *** matrice;
extern short int *** voisins;
extern pthread_barrier_t barrier;
extern bool open;
void init();
void deleteMatrice();

#endif 
