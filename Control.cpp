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

void calculeCellule(int i,int j){
	int nbVoisins = nbCelluleVoisine(i,j);
	if( nbVoisins == 3)
		matrice[i][j][next] = 1;
	else if( nbVoisins == 2)
		matrice[i][j][next] = matrice[i][j][first];
	else if( nbVoisins < 2 || nbVoisins > 3)
		matrice[i][j][next] = 0;
}
void ajoutVoisin(int x, int y)
{
	for(int i=x-1; i<=x+1; i++)
		for(int j=y-1; j<=y+1; j++)
			if(!(i==x && j==y))
				voisins[i][j][next] = voisins[i][j][first] +1;
	
}
void supprVoisin(int x, int y)
{
	for(int i=x-1; i<=x+1; i++)
		for(int j=y-1; j<=y+1; j++)
			if(!(i==x && j==y))
				voisins[i][j][next] = voisins[i][j][first] -1;
	
}
void calculeCelluleVoisin(int i,int j){
	if(voisins[i][j][first] == 3){
		if( matrice[i][j][first] == 0)
			ajoutVoisin(i,j);
		matrice[i][j][next] = 1;
	}
	else if(voisins[i][j][first] == 2){
		matrice[i][j][next] = matrice[i][j][first];
		//voisins[i][j][next] = voisins[i][j][first];
		
	}
	else if( voisins[i][j][first] < 2 || voisins[i][j][first] > 3)
	{
		if(matrice[i][j][first] == 1)
			supprVoisin(i,j);
		matrice[i][j][next] = 0;
	}	
}

void nextStep(int quartier){
	int x_min,x_max,i,j;
	x_min = (NB_MATRICE/NB_THREADS)*quartier;
	x_max = (NB_MATRICE/NB_THREADS)*(quartier+1);
	if(quartier == 0)
		x_min +=1;
	if(quartier == NB_THREADS-1)
		x_max -=1;
	for(i=x_min; i<x_max; i++)
		for(j=1; j<NB_MATRICE-1; j++)
			//calculeCelluleVoisin(i,j);
			calculeCellule(i,j);
}

void initVoisins()
{
	for(int i=1;i<NB_MATRICE-1;i++)
		for(int j=1;j<NB_MATRICE-1;j++)
			for(int k=0;k<2;k++)
				voisins[i][j][k] = nbCelluleVoisine(i,j);
	
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
	//printf("Thread termine\n");
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





