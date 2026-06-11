
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
int main () {
	int pid,i, m=10;
	int tiempoinicial, tiempoactual;
	tiempoinicial = time(NULL);
	tiempoactual = time(NULL) - tiempoinicial;
	printf(" %d: Inicio del programa \n",tiempoactual );
	for (i=0; i<3; i++) {
		pid=fork();
		sleep(1);
		switch (pid) {
			case -1:
				perror("Error en la creación de procesos");
				exit(-1);
			case 0:
				m++;
				tiempoactual = time(NULL) - tiempoinicial;
				printf(" %d: Hijo %d m= %d\n",tiempoactual, i, m);
				sleep(2);
				exit(0);
			default:
				tiempoactual = time(NULL) - tiempoinicial;
				printf(" %d: Creado el proceso %d\n", tiempoactual, i);
			if( i %2 == 0 ) {
				wait(NULL);
				tiempoactual = time(NULL) - tiempoinicial;
				printf(" %d: Finalizó un proceso, valor de m= %d\n", tiempoactual,m);
			}
	}
	}
	wait(NULL);
	tiempoactual = time(NULL) - tiempoinicial;
	printf(" %d: Finalizó un proceso (padre), valor de m= %d \n",tiempoactual, m);
	return 0;
}
