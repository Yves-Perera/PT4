#include <unistd.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <semaphore.h>
#include "Constantes.h"
#include "Model.h"

int nbCelluleVoisine(int x, int y){
	int cpt=0,i,j;
	for(i=x-1; i<=x+1; i++)
		for(j=y-1; j<=y+1; j++)
			if(!(i==x && j==y))
				if(matrice[i][j][first].cellule == 1)
					cpt++;
	return cpt;
}

bool calculeCellule(int i,int j){ //retourne vrai si il y a un changement
	int nbVoisins = nbCelluleVoisine(i,j);
	if(nbVoisins == 3)
		matrice[i][j][next].cellule = 1;
	else if( nbVoisins == 2)
		matrice[i][j][next].cellule = matrice[i][j][first].cellule;
		
	else
		matrice[i][j][next].cellule = 0;
		
	return(matrice[i][j][next].cellule != matrice[i][j][first].cellule);
}

void nextStep(int quartier){
	int x_min,x_max,i,j,cpt=0;
	x_min = (NB_MATRICE/NB_THREADS)*quartier;
	x_max = (NB_MATRICE/NB_THREADS)*(quartier+1);
	if(quartier == 0)
		x_min +=1;
	if(quartier == NB_THREADS-1)
		x_max =NB_MATRICE-1;
		
		
	for(i=x_min; i<x_max; ++i)
	{
		for(j=1; j<NB_MATRICE-1; ++j)
			calculeCellule(i,j);
	}
}

void nextStepStagnation(int quartier, bool &actif){
	int x_min,x_max,i,j,cpt=0;
	bool changement = false;
	x_min = (NB_MATRICE/NB_THREADS)*quartier;
	x_max = (NB_MATRICE/NB_THREADS)*(quartier+1);
	if(quartier == 0)
		x_min +=1;
	if(quartier == NB_THREADS-1)
		x_max =NB_MATRICE-1;
		
	//Calcule des bords
	for(i=x_min+1; i<x_max-1; ++i) //-1 et +1 pour éviter de passer deux fois sur les coins
	{
		if(calculeCellule(i,1))
			changement = true;
		if(calculeCellule(i,NB_MATRICE-2))
			changement = true;
	}
	for(j=1; j<NB_MATRICE-1; ++j)
	{
		if(calculeCellule(x_min,j))
			changement = true;
		if(calculeCellule(x_max-1,j))
			changement = true;
	}
	
	//si zone active, calcule de l interieur
	if(actif)
	{
		for(i=x_min+1; i<x_max-1; ++i)
		{
			for(j=2; j<NB_MATRICE-2; ++j){
				if(calculeCellule(i,j))
					changement = true;
			}
		}
	}
	actif = changement;
}

void *f_thread(void *arg)
{
	int rc;
	int quartier = (int&)arg;
	
	for(int i =0 ; i < LOOP;i++)
	{
		nextStep(quartier);	
		rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Impossible d'attendre la barrier\n");
			exit(-1);
		}
	}
	pthread_exit(NULL);
}

void *f_threadStagnation(void *arg)
{
	int rc;
	int quartier = (int&)arg;
	bool actif=true;
	
	for(int i =0 ; i < LOOP;i++)
	{
		rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD) {printf("Impossible d'attendre la barrier\n"); exit(-1);}
		
		nextStepStagnation(quartier,actif);
	}
	
	pthread_exit(NULL);
}

void tore(){

	for(int i=1;i<NB_MATRICE-1;i++){
		matrice[i][0][first].cellule = matrice[i][NB_MATRICE-2][first].cellule;
		matrice[i][NB_MATRICE -1][first].cellule = matrice[i][1][first].cellule;

		matrice[0][i][first].cellule = matrice[NB_MATRICE-2][i][first].cellule;
		matrice[NB_MATRICE-1][i][first].cellule = matrice[1][i][first].cellule;
	}
	
	matrice[0][0][first].cellule = matrice[NB_MATRICE-2][NB_MATRICE-2][first].cellule;
	matrice[0][NB_MATRICE-1][first].cellule = matrice[NB_MATRICE-2][1][first].cellule;
	matrice[NB_MATRICE-1][0][first].cellule = matrice[1][NB_MATRICE-2][first].cellule;
	matrice[NB_MATRICE-1][NB_MATRICE-1][first].cellule = matrice[1][1][first].cellule;
}





