//traspuesta.c
#include<stdio.h>
#include<stdlib.h>
#include<omp.h> 

/* Time in seconds from some point in the past */
double dwalltime();


int main(int argc,char*argv[]){
 double *A;
 double temp; 
 int i,j, N,tid;
 int check=1;
 double timetick;

 //Controla los argumentos al programa
  if (argc < 3){
   printf("\n Faltan argumentos:: N dimension de la matriz, T cantidad de threads \n");
   return 0;
  }

   N=atoi(argv[1]);
   int numThreads = atoi(argv[2]);
   omp_set_num_threads(numThreads);	

  //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);

  //Inicializa la matriz con unos en el triangulo inferior y ceros en el triangulo superior.
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
		if (i>=j)
		{
			A[i*N+j]= 1.0;
		}
		else
		{
			A[i*N+j]= 0.0;
		}

   }
  }   
/*
Si no pones default, si las varibles internas del shared no estan declaradas, tira error de compilacion 
*/
//printf("%d , %d",i,j);
#pragma omp parallel default(none) private(i,j,temp,timetick,tid) shared(A,N)
{ 
 tid= omp_get_thread_num();
 timetick = dwalltime();
  //saque private i,j,temp
 //el nowait hace que no tengas que esperar a la sincronizacion, osea q si termino el for siga siga en el printf
  /*
      Con el scheduling defino la cantidad de CARGA que va tener cada hilo. SI yo lo dejo que solo divida parejo
    entre todos los que hace el pragma for, voy a tener q recorrer todo y la matriz es una triangular, por lo que
    no seria correcto.
      Con el dynamic dividis entre comillas estaticamente la carga entre los hilos, por mas que sean 4 y tenga matriz e 1024
    el numero que le coloque va ser la misma para cada uno.
    RESPUESTA: hay un error en el recorrido por ser matriz triangular y no una completa.
  */
  
  #pragma omp for nowait schedule(dynamic,2)
  for(i=0;i<N;i++){
   for(j=i+1;j<N;j++){
		temp = A[i*N+j];
		A[i*N+j]= A[j*N+i];
		A[j*N+i]= temp;
   }
  }   
  printf("Tiempo en segundos para el thread %d: %f \n", tid,dwalltime() - timetick);
}

  //Chequea los resultados
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
		if (i>j)
		{
			check= check&&(A[i*N+j]==0.0);
		}
		else
		{
			check= check&&(A[i*N+j]== 1.0);
		}
   }
  }   

  if(check){
   printf("Resultado correcto\n");
  }else{
   printf("Resultado erroneo\n");
  }
 free(A);
 return(0);
}



/*****************************************************************/

#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}


