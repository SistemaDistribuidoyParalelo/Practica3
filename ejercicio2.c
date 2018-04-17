//Ejercicio 2
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>


int main(int argc,char*argv[]){
 double x,scale;
 int i;
 int numThreads = atoi(argv[2]);
 int N=atoi(argv[1]);
 omp_set_num_threads(numThreads);
 scale=2.78;
 x=0.0;
/*
	sqrt(interno) = raiz cuadrada de lo interno
	El ejercicio esta realizando una suma y la esta guardando en los valores de X. Lo que sucede es que las cuentas 
	se iran superponiendo o se iran guardando mal dado que se pueden pisar los resultados de los hilos.
	Es seccion critica, cada uno deberia de entrar de a uno, hacer la cuenta, guardar el resultado y seguir.
*/
 #pragma omp parallel for
 for(i=1;i<=N;i++){
	#pragma omp critical(sumaX)
		x = x + sqrt(i*scale) + 2*x;
 }

 printf("\n Resultado: %f \n",x);

 return(0);
}





