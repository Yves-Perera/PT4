#ifndef MODEL_H 
#define MODEL_H 
struct Data
{
   unsigned cellule  : 1;
};
extern bool first;
extern bool next;
extern Data *** matrice;
extern pthread_barrier_t barrier;
void init();
void deleteMatrice();

#endif 
