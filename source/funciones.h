

#define c 1.0
#define dt 0.1
#define dd 2.0
pthread_mutex_t **mutex;	//Inicialización mutex
float **H;	// Matriz en instante t
float **H_t_2;
float **H_t_1;

typedef struct{

	int posX;
	int posY;
}coordenada;

typedef struct{

	float value;
}elemento;

typedef struct {
     pthread_t 			tid;
     int 				elementosPorHebra;
     coordenada			*coordenadas;
     int 				matrixSize;
     int 				tActual;
}hebra;


//DECLARACION DE FUNCIONES
void crearHebras(pthread_t threads[], int numeroHebras, float **H, int N, int t);
float **generateMatrix(int N);
float schrodEq(int x, int y,int N, int t);
float **advance(float **H, int x, int y, int n, int N);
float **applyWave(float **H, int t,int N);
void printMatrix(float **H,int N);
void fprintMatrix(float **H,char *salida, int N);
void waitHebras(pthread_t threads[], int numeroHebras);
void getNextMatrix(pthread_t threads[], hebra **threads_data, float **Hprev, int numeroHebras, int N);
void *applySchrod(void *arg);
void enterSC(int x, int y, int N);
void exitSC(int x, int y, int N);