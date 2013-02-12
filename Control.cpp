#include <unistd.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Model.h"

int nbCelluleVoisine(int x, int y){
	int cpt=0,i,j;
	for(i=x-1; i<=x+1; i++)
		for(j=y-1; j<=y+1; j++)
			if(!(i==x && j==y))
				if(matrice[i][j][first] == 1)
					cpt++;
	return cpt;
}

void calculeCellule(int i,int j){
	int nbVoisins = nbCelluleVoisine(i,j);
	if( nbVoisins == 3)
		matrice[i][j][next] = 1;
	else if( nbVoisins == 2)
		matrice[i][j][next] = matrice[i][j][first];
	else if( nbVoisins < 2 || nbVoisins > 3)
		matrice[i][j][next] = 0;
}

void nextStep(int quartier){
	int x_min,x_max,i,j;
	x_min = (NB_MATRICE/NB_THREADS)*quartier;
	x_max = (NB_MATRICE/NB_THREADS)*(quartier+1);

	for(i=x_min; i<x_max; i++)
		for(j=1; j<NB_MATRICE-1; j++)
			calculeCellule(i,j);
}

void *f_thread(void *arg)
{
  int quartier = (int&)arg;
  //printf("Bonjour, je suis le thread %d\n", quartier);
  nextStep(quartier);
  //return (void*)quartier;
}
void tore(){
	int i;
	
	for(i=1;i<NB_MATRICE-1;i++){
		matrice[i][0][first] = matrice[i][NB_MATRICE-1][first];
		matrice[i][NB_MATRICE][first] = matrice[i][1][first];

		matrice[0][i][first] = matrice[NB_MATRICE-1][i][first];
		matrice[NB_MATRICE][i][first] = matrice[1][i][first];
	}
	
}





