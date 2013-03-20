#ifndef MODEL_H 
#define MODEL_H 
struct Data
{
   unsigned cellule  : 1;
   //unsigned numSec : 7; //7bits libre
};
extern bool first;
extern bool next;
extern Data *** matrice;
extern int **bordSecY;
extern int **bordThreadX;
extern pthread_barrier_t barrier;
void init();
void deleteMatrice();

#endif 
