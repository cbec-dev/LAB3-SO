#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>

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


float schrodEq(float **H, int x, int y)
{
	//float **Hnew = generateMatrix();
	float aux = (c)/(dt/dd);
	aux = aux*aux;
	//printf("%f\n", aux);
	float value = 2*H[x][y]-H[x][y]+aux*(H[x+1][y]+H[x-1][y]+H[x][y-1]-4*H[x][y+1]);
	//printf("%f\n", a);
	
	//Hnew[x][y] = a;

	return value;
}

float schrodEqBordes(float **H, int x, int y)
{
	float aux = (c)/(dt/dd);
	aux = aux*aux;
	
	float lower = 0;
	float upper = 0;
	float left = 0;
	float right = 0;

	if(x!=0)	upper = H[x-1][y];
	if(x!=N)	lower = H[x+1][y];
	if(y!=0)	left = H[x][y-1];
	if(y!=N)	right = H[x][y+1];

	float value = 2*H[x][y]-H[x][y]+aux*(lower+upper+left-4*right);


	return value;
}



//Funcion que dado H(t) retorna H(t+1)
float **advance(float **H)
{

    int x = 0;
    int y = 0;
    int d = 0; // direccion
    int count = 0; // contador
    int s = 1; // size
    float**Hnew = generateMatrix();// Matriz H en t+1

    // centro de la matriz
    x = ((int)floor(N/2.0))-1;
    y = ((int)floor(N/2.0))-1;
    printf("x:%i y:%i\n",x,y);

    for (int k=1; k<=(N-1); k++)
    {
        for (int j=0; j<(k<(N-1)?2:3); j++)
        {
            for (int i=0; i<s; i++)
            {
            	if (x!=0 && y!=0 && x!=N-1 && y!=N-1)
            	{
	            	float value = schrodEq(H, x, y);
	                Hnew[x][y] = value;

            	}
                count++;

                switch (d)
                {
                    case 0: y = y + 1; break;
                    case 1: x = x + 1; break;
                    case 2: y = y - 1; break;
                    case 3: x = x - 1; break;
                }

            }
            d = (d+1)%4;
        }
        s = s + 1;
    }
    return Hnew;
}


//Funcion que dado H(0) entrega H(t)
float **applyWave(float **H, int t)
{
	float**H_t = generateMatrix();
	float**Haux = generateMatrix();
	int tActual = 0;
	while(tActual < t)
	{
		printf("Estoy en t=%i\n",tActual);
		Haux = advance(H);
		H = Haux;

		tActual++;
	}
	H_t = Haux;
	return H_t;
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

void fprintMatrix(float **H,char *salida){
	int i,j;
	if (salida !=NULL){
		FILE *archivoSalida;
		archivoSalida=fopen(salida,"wb");
		for (i = 0; i < N; ++i){
			fwrite(H[i],N,sizeof(float),archivoSalida);
		}
		fclose(archivoSalida);
	}
}

int main(int argc, char **argv)
{


	float **H = generateMatrix();	// Matriz en instante 0
	printMatrix(H);
	int t = 3;
	float **Hnew = applyWave(H, t);		//Matriz en instante t
	printMatrix(Hnew);
	fprintMatrix(Hnew, "out3.raw");







	return 0;

}