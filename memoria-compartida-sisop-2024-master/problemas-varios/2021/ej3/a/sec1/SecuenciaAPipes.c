#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


//(AoB)CD (AoB)CD (AoB)CD

int pipeAoB[2];
int pipeC[2];
int pipeD[2];

char mensaje = 'X';

void funA(){

	close(pipeAoB[1]);
	close(pipeC[0]);
	close(pipeD[0]);
	close(pipeD[1]);
	
	while(1){
		
		read(pipeAoB[0],&mensaje,sizeof(char));
		printf("A");
		fflush(stdout);
		write(pipeC[1],&mensaje,sizeof(char));
	}
}

void funB(){
	close(pipeAoB[1]);
	close(pipeC[0]);
	close(pipeD[0]);
	close(pipeD[1]);
	
	while(1){
		
		read(pipeAoB[0],&mensaje,sizeof(char));
		printf("B");
		fflush(stdout);
		write(pipeC[1],&mensaje,sizeof(char));
	}
}

void funC(){

	close(pipeAoB[1]);
	close(pipeC[1]);
	close(pipeD[0]);
	close(pipeAoB[0]);
	
	while(1){
		
		read(pipeC[0],&mensaje,sizeof(char));
		printf("C");
		fflush(stdout);
		write(pipeD[1],&mensaje,sizeof(char));
	}
}


void funD(){

	close(pipeAoB[0]);
	close(pipeC[0]);
	close(pipeC[1]);
	close(pipeD[1]);
	
	while(1){
		
		read(pipeD[0],&mensaje,sizeof(char));
		printf("D\n");
		fflush(stdout);
		write(pipeAoB[1],&mensaje,sizeof(char));
	}
}

int main(){
	pid_t pidA;
	pid_t pidB;
	pid_t pidC;
	pid_t pidD;
	
	pipe(pipeAoB);
	pipe(pipeC);
	pipe(pipeD);
	

	
	pidA = fork();
	if(pidA == -1){
		printf("Error al crear pidA\n");
	}
	else if(pidA == 0)
		funA();
		
	pidB = fork();
	if(pidB == -1){
		printf("Error al crear pidB\n");
	}
	else if(pidB == 0)
		funB();
	
	pidC = fork();
	if(pidC == -1){
		printf("Error al crear pidC\n");
	}
	else if(pidC == 0)
		funC();
		
	pidD = fork();
	if(pidD == -1){
		printf("Error al crear pidD\n");
	}
	else if(pidD == 0)
		funD();
		
	close(pipeAoB[0]);
	close(pipeC[0]);
	close(pipeC[1]);
	close(pipeD[1]);
	close(pipeD[0]);
	write(pipeAoB[1], &mensaje,sizeof(char));
	close(pipeAoB[1]);
	
	for(int i = 0; i < 4; i++){
		wait(NULL);
	}
}
