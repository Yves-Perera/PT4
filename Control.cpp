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
	int y_min,y_max,i,j,cpt=0;
	y_min = (NB_MATRICE/NB_THREADS)*quartier;
	y_max = (NB_MATRICE/NB_THREADS)*(quartier+1);
	if(quartier == 0)
		y_min +=1;
	if(quartier == NB_THREADS-1)
		y_max =NB_MATRICE-1;
		
		
	for(i=y_min; i<y_max; ++i)
	{
		for(j=1; j<NB_MATRICE-1; ++j)
			calculeCellule(i,j);
	}
}

void nextStepStagnation(int quartier, bool actif[]){
	int y_min,y_max,i,j,cpt=0;
	bool changement = false;
	y_min = (NB_MATRICE/NB_THREADS)*quartier;
	y_max = (NB_MATRICE/NB_THREADS)*(quartier+1);
	if(quartier == 0)
		y_min +=1;
	if(quartier == NB_THREADS-1)
		y_max =NB_MATRICE-1;
		
	//Calcule des bords
	for(i=y_min+1; i<y_max-1; ++i) //-1 et +1 pour éviter de passer deux fois sur les coins
	{
		if(calculeCellule(i,1))
			changement = true;
		if(calculeCellule(i,NB_MATRICE-2))
			changement = true;
	}
	for(j=1; j<NB_MATRICE-1; ++j)
	{
		if(calculeCellule(y_min,j))
			changement = true;
		if(calculeCellule(y_max-1,j))
			changement = true;
	}
	
	//si zone active, calcule de l interieur
	if(actif[0])
	{
		for(i=y_min+1; i<y_max-1; ++i)
		{
			for(j=2; j<NB_MATRICE-2; ++j){
				if(calculeCellule(i,j))
					changement = true;
			}
		}
	}
	actif[0] = changement;
}

void calculeSection(int numThread, bool actif[], int numSection)
{
	int i,j;
	bool changement = false;
	//Calcule des bords
	for(i=bordX[numSection][0]+1; i<bordX[numSection][1]-1; ++i) //-1 et +1 pour éviter de passer deux fois sur les coins
	{
		if(calculeCellule(i,bordY[numThread][0]))
			changement = true;
		if(calculeCellule(i,bordY[numThread][1]))
			changement = true;
	}
	for(j=bordY[numThread][0]+1; j<bordY[numThread][1]-1; ++j)
	{
		if(calculeCellule(bordX[numSection][0],j))
			changement = true;
		if(calculeCellule(bordX[numSection][1],j))
			changement = true;
	}
	
	//si zone active, calcule de l interieur
	if(actif[numSection])
	{
		for(i=bordX[numSection][0]; i<bordX[numSection][1]; ++i)
		{
			for(j=bordY[numThread][0]; j<bordY[numThread][1]; ++j){
				if(calculeCellule(i,j))
					changement = true;
			}
		}
	}
	actif[numSection] = changement;
}
void nextStepSection(int numThread, bool actif[])
{
	for(int i =0; i<NB_SEC; i++)
		calculeSection(numThread,actif,i);
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
	int numThread = (int&)arg;
	bool *actif= new bool[NB_SEC];
	for(int i =0 ; i < NB_SEC;i++)
		actif[i] = true; 
	
	for(int i =0 ; i < LOOP;i++)
	{
		rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD) {printf("Impossible d'attendre la barrier\n"); exit(-1);}
		
		//nextStepStagnation(quartier,actif);
		nextStepSection(numThread,actif);
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





