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
				if(matrice[i][j][first] == 1)
					cpt++;
	return cpt;
}

bool calculeCellule(int i,int j){ //retourne vrai si il y a un changement
	int nbVoisins = nbCelluleVoisine(i,j);
	if(nbVoisins == 3)
		matrice[i][j][next] = 1;
	else if( nbVoisins == 2)
		matrice[i][j][next] = matrice[i][j][first];
		
	else
		matrice[i][j][next] = 0;
		
	return(matrice[i][j][next] != matrice[i][j][first]);
}

void nextStep(int quartier){
	int x_min,x_max,i,j,cpt=0;
	bool changement = false;
	x_min = (NB_MATRICE/NB_THREADS)*quartier;
	x_max = (NB_MATRICE/NB_THREADS)*(quartier+1);
	if(quartier == 0)
		x_min +=1;
	if(quartier == NB_THREADS-1)
		x_max =NB_MATRICE-1;
		
		
	for(i=x_min; i<x_max; ++i)
	{
		for(j=1; j<NB_MATRICE-1; ++j){
			if(calculeCellule(i,j))
				changement = true;
		}
	}
}

void nextStepStagnation(int quartier){
	int x_min,x_max,i,j,cpt=0;
	bool changement = false;
	x_min = (NB_MATRICE/NB_THREADS)*quartier;
	x_max = (NB_MATRICE/NB_THREADS)*(quartier+1);
	if(quartier == 0)
		x_min +=1;
	if(quartier == NB_THREADS-1)
		x_max =NB_MATRICE-1;
		
		
	for(i=x_min; i<x_max; ++i)
	{
		for(j=1; j<NB_MATRICE-1; ++j){
			if(calculeCellule(i,j))
				changement = true;
		}
	}
}

void *f_thread(void *arg)
{
	int rc;
	int quartier = (int&)arg;
	
	for(int i =0 ; i < LOOP;i++)
	{
		rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Impossible d'attendre la barrier\n");
			exit(-1);
		}
		nextStep(quartier);	
	}
	pthread_exit(NULL);
}

void *f_threadStagnation(void *arg)
{
	int rc;
	int quartier = (int&)arg;
	bool actif;
	
	for(int i =0 ; i < LOOP;i++)
	{
		rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Impossible d'attendre la barrier\n");
			exit(-1);
		}
		nextStepStagnation(quartier);
	}
	pthread_exit(NULL);
}

void tore(){

	for(int i=1;i<NB_MATRICE-1;i++){
		matrice[i][0][first] = matrice[i][NB_MATRICE-2][first];
		matrice[i][NB_MATRICE -1][first] = matrice[i][1][first];

		matrice[0][i][first] = matrice[NB_MATRICE-2][i][first];
		matrice[NB_MATRICE-1][i][first] = matrice[1][i][first];
	}
	
	matrice[0][0][first] = matrice[NB_MATRICE-2][NB_MATRICE-2][first];
	matrice[0][NB_MATRICE-1][first] = matrice[NB_MATRICE-2][1][first];
	matrice[NB_MATRICE-1][0][first] = matrice[1][NB_MATRICE-2][first];
	matrice[NB_MATRICE-1][NB_MATRICE-1][first] = matrice[1][1][first];
}





