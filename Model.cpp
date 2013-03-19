#include "Constantes.h"
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include "Model.h"

bool first =false;
bool next =true;

//La matrice de cellule
Data *** matrice;

//Les bords en ordonnée des sous zones de stagnation des threads
int **bordSecY;
//Les bords en abcisse des threads
int **bordThreadX;

pthread_barrier_t barrier;


void init(){
	matrice = new Data**[NB_MATRICE];
	bordSecY = new int*[NB_SEC];
	bordThreadX = new int*[NB_THREADS];
	for(int i=0;i<NB_SEC;i++)
		bordSecY[i] = new int[2];
	for(int i=0;i<NB_THREADS;i++)
		bordThreadX[i] = new int[2];
		
	for(int i=0;i<NB_SEC;i++)
	{
		bordSecY[i][0] = (NB_MATRICE/NB_SEC)*i+1;
		bordThreadX[i][1] = ((NB_MATRICE/NB_SEC)*(i+1));
	}
	for(int i=0;i<NB_THREADS;i++)
	{
		bordThreadX[i][0] = (NB_MATRICE/NB_THREADS)*i+1;
		bordThreadX[i][1] = ((NB_MATRICE/NB_THREADS)*(i+1));
	}
	bordSecY[0][0] = 1;
	bordSecY[NB_SEC-1][1] = NB_MATRICE-2;
	
	bordThreadX[0][0] = 1;
	bordThreadX[NB_THREADS-1][1] = NB_MATRICE-2;
	

    for(int i=0;i<NB_MATRICE;i++){
        matrice[i] = new Data*[NB_MATRICE];
        
        for(int j=0;j<NB_MATRICE;j++)
            matrice[i][j] = new Data[2];
    }
    //*
    for(int i=0;i<NB_MATRICE;i++)
		for(int j=0;j<NB_MATRICE;j++)
			for(int k=0;k<2;k++)
				matrice[i][j][k].cellule = rand()%2;
	//*/
	/*
	for(int i=0;i<NB_MATRICE;i++)
		for(int j=0;j<NB_MATRICE;j++)
			for(int k=0;k<2;k++)
				matrice[i][j][k].cellule = 0;
	//*/		
	if(pthread_barrier_init(&barrier, NULL, NB_THREADS+1))
        printf("Impossible de creer la barrier\n");


}

void deleteMatrice()
{	
    for(int i=0;i<NB_MATRICE;i++)
    {
		for(int j=0;j<NB_MATRICE;j++)
             delete[] matrice[i][j];
        delete[] matrice[i];
    }
 
	for(int i=0;i<NB_SEC;i++)
		delete bordSecY[i];

	for(int i=0;i<NB_THREADS;i++)
		delete bordThreadX[i];
    delete[] bordSecY;
    delete[] bordThreadX;
    delete[] matrice;
}
