#include "Constantes.h"
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include "Model.h"

bool first =false;
bool next =true;
Data *** matrice;
int **bordX;
int **bordY;
pthread_barrier_t barrier;


void init(){
	matrice = new Data**[NB_MATRICE];
	bordX = new int*[NB_SEC];
	bordY = new int*[NB_THREADS];
	for(int i=0;i<NB_SEC;i++)
		bordX[i] = new int[2];
	for(int i=0;i<NB_THREADS;i++)
		bordY[i] = new int[2];
		
	for(int i=0;i<NB_SEC;i++)
	{
		bordX[i][0] = (NB_MATRICE/NB_SEC)*i+1;
		bordX[i][1] = ((NB_MATRICE/NB_SEC)*(i+1));
	}
	for(int i=0;i<NB_THREADS;i++)
	{
		bordY[i][0] = (NB_MATRICE/NB_THREADS)*i+1;
		bordY[i][1] = ((NB_MATRICE/NB_THREADS)*(i+1));
	}
	bordX[0][0] = 1;
	bordX[NB_SEC-1][1] = NB_MATRICE-2;
	
	bordY[0][0] = 1;
	bordY[NB_THREADS-1][1] = NB_MATRICE-2;
	

    for(int i=0;i<NB_MATRICE;i++){
        matrice[i] = new Data*[NB_MATRICE];
        
        for(int j=0;j<NB_MATRICE;j++)
            matrice[i][j] = new Data[2];
    }
    /*
    for(int i=0;i<NB_MATRICE;i++)
		for(int j=0;j<NB_MATRICE;j++)
			for(int k=0;k<2;k++)
				matrice[i][j][k].cellule = rand()%2;
	//*/
	//*
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
		delete bordX[i];

    delete[] bordX;
    delete[] matrice;
}
