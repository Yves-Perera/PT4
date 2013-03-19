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

/**
 * Renvoie le nombre de voisin d'une Cellule
 * */
int nbCelluleVoisine(int x, int y){
	int cpt=0,i,j;
	for(i=x-1; i<=x+1; i++)
		for(j=y-1; j<=y+1; j++)
			if(!(i==x && j==y))
				if(matrice[i][j][first].cellule == 1)
					cpt++;
	return cpt;
}

/**
 * Calcule l'état suivant d'une Cellule.
 * Retourne Vrai si la cellule a changé depuis l'état precedent.
 * */
bool calculeCellule(int i,int j){
	int nbVoisins = nbCelluleVoisine(i,j);
	if(nbVoisins == 3)
		matrice[i][j][next].cellule = 1;
	else if( nbVoisins == 2)
		matrice[i][j][next].cellule = matrice[i][j][first].cellule;
		
	else
		matrice[i][j][next].cellule = 0;
		
	return(matrice[i][j][next].cellule != matrice[i][j][first].cellule);
}

/**
 * Calcule les sous zones des thread suivant si elles sont active ou pas.
 * Peux eventuellement passer la zone en active ou inactive.
 * Calcule dans tous les cas les bords de la sous zone.
 * */
void calculeSection(int numThread, bool actif[], int numSection)
{
	int i,j;
	bool changement = false;
	//Calcule des bords
	for(i=bordSecY[numSection][0]+1; i<bordSecY[numSection][1]-1; i++) //-1 et +1 pour éviter de passer deux fois sur les coins
	{
		if(calculeCellule(i,bordThreadX[numThread][0]))
			changement = true;
		if(calculeCellule(i,bordThreadX[numThread][1]))
			changement = true;
	}
	for(j=bordThreadX[numThread][0]; j<bordThreadX[numThread][1]; j++)
	{
		if(calculeCellule(bordSecY[numSection][0],j))
			changement = true;
		if(calculeCellule(bordSecY[numSection][1],j))
			changement = true;
	}
	
	//si zone active, calcule de l interieur
	if(actif[numSection])
	{
		for(i=bordSecY[numSection][0]; i<bordSecY[numSection][1]; ++i)
		{
			for(j=bordThreadX[numThread][0]; j<bordThreadX[numThread][1]; ++j){
				if(calculeCellule(i,j))
					changement = true;
			}
		}
	}
	actif[numSection] = changement;
}

/**
 * appel la fonction de calcule de sous zone
 * */
void nextStepSection(int numThread, bool actif[])
{
	for(int i =0; i<NB_SEC; i++)
		calculeSection(numThread,actif,i);
}

/**
 * Fonction appelé par chaque thread
 * */
void *f_threadStagnation(void *arg)
{
	int rc;
	int numThread = (int&)arg;
	bool *actif= new bool[NB_SEC]; //tableau de booleen pour savoir si les sous zones sont active
	for(int i =0 ; i < NB_SEC;i++)
		actif[i] = true; //au commencement, elles sont toutes active par defaut
	
	for(int i =0 ; i < LOOP;i++)
	{
		rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD) {printf("Impossible d'attendre la barrier\n"); exit(-1);}
		nextStepSection(numThread,actif);
	}
	
	pthread_exit(NULL);
}

/**
 * Effectue le systeme de tore de la matrice, copiant les bords
 * exterieur entre eux, ainsi que les coins 2 par 2
**/
void tore(){

	//copie des bords
	for(int i=1;i<NB_MATRICE-1;i++){
		matrice[i][0][first].cellule = matrice[i][NB_MATRICE-2][first].cellule;
		matrice[i][NB_MATRICE -1][first].cellule = matrice[i][1][first].cellule;

		matrice[0][i][first].cellule = matrice[NB_MATRICE-2][i][first].cellule;
		matrice[NB_MATRICE-1][i][first].cellule = matrice[1][i][first].cellule;
	}
	
	//copie des coins
	matrice[0][0][first].cellule = matrice[NB_MATRICE-2][NB_MATRICE-2][first].cellule;
	matrice[0][NB_MATRICE-1][first].cellule = matrice[NB_MATRICE-2][1][first].cellule;
	matrice[NB_MATRICE-1][0][first].cellule = matrice[1][NB_MATRICE-2][first].cellule;
	matrice[NB_MATRICE-1][NB_MATRICE-1][first].cellule = matrice[1][1][first].cellule;
}





