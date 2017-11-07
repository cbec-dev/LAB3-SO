#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <locale.h>

#define N 10
#define T 5
#define c 1.0
#define dt 0.1
#define dd 2.0


float **generateMatrix()
{

	int i, j;
	float **Hnew;
	Hnew = (float **)malloc(sizeof(float*)*N);
	for (i = 0; i < N; ++i){
		Hnew[i]=(float*)malloc(sizeof(float)*N);
		for (j = 0; j < N; ++j){
			Hnew[i][j] = 0;
			if(0.4*N<i && i<0.6*N)
			{
				Hnew[i][j]=20;
			}
			if(0.4*N<j && j<0.6*N)
			{
				Hnew[i][j]=20;
			}
		}
	}
	return Hnew;
}


float **schrodEq(float **H, int x, int y)
{
	float **Hnew = generateMatrix();
	float aux = (c)/(dt/dd);
	aux = aux*aux;
	printf("%f\n", aux);
	float a = 2*H[x][y]-H[x][y]+aux*(H[x+1][y]+H[x-1][y]+H[x][y-1]-4*H[x][y+1]);
	printf("%f\n", a);
	
	Hnew[x][y] = a;

	return Hnew;
}


void printMatrix(float **H)
{
	int i,j;
	for (i = 0; i < N; ++i){
		for (j = 0; j < N; ++j){
			printf("%f", H[i][j]);	
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{


	float **H = generateMatrix();
	printMatrix(H);
	float **H_1 = schrodEq(H, 5, 5);
	printMatrix(H_1);







	return 0;

}