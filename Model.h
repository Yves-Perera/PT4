#ifndef MODEL_H 
#define MODEL_H 

extern bool first;
extern bool next;
extern bool *** matrice;
//extern short int *** voisins;
extern pthread_barrier_t barrier;
void init();
void deleteMatrice();

#endif 
