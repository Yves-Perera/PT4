#ifndef CONTROL_H
#define CONTROL_H 

int nbCelluleVoisine(int x, int y);
void calculeCellule(int i,int j);
void nextStep(int quartier);
void *f_thread(void *arg);
void tore();

#endif 
