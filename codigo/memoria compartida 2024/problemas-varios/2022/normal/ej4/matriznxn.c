#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>

/*
FALTA LO DE SINCRONIZAR LA SALIDA 
POR SEMAFOROS



  */
#define N 5
#define SEG_SIZE sizeof(struct matrix)

void initMatrix(int matriz[N][N],int valores[N][N]){



	for(int r = 0; r < N; r++){		
		for(int g=0; g< N; g++){
			matriz[r][g] = valores[r][g];
				
		}
					
	}

	
}

struct matrix {
		int matrix1[N][N];	
		int matrix2[N][N];	
		int matrixRes[N][N];	
		
	
};



int main(){
	
	pid_t procs[N];
	int status, id;
	
	key_t key = ftok("/tmp",'A');
	if(key == -1){printf("fallo al hacer la llave %s\n",strerror(errno)); exit(0);}

	id = shmget(key,SEG_SIZE,IPC_CREAT | 0666);
	if(id == -1){
		printf("fallo al crear el segmento: %s",strerror(errno));
		exit(0);
		
	}
	
	struct matrix *init = (struct matrix *) shmat(id,0,0);
	if(init == (void*)-1){
		printf("fallo al adjuntar el segmento init: %s",strerror(errno));
		exit(0);
		
	}
	

	int valores1[N][N] = {
					{11,2,33,46,2},
					{4,5,6,4,31},
					{4,5,67,2,3},
					{51,2,3,40,4},
					{21,24,3,44,5}					
					};
	int valores2[N][N] = {
					{23,2,24,4,9},
					{3,34,3,1,8},
					{3,23,1,2,7},
					{1,13,21,5,0},
					{1,1,21,4,70}
					};
	
				
				
		
			
		
		initMatrix(init->matrix1, valores1);
		initMatrix(init->matrix2, valores2);
		
		
		int i;
	
		for( int procesos = 0; procesos < N; procesos++){
			
			i = procesos;
			procs[procesos] = fork();
				
			if(procs[procesos] == 0){
				shmdt(init);
				
				struct matrix *info = (struct matrix *) shmat(id,0,0);
				if(info == (void*)-1){
					printf("fallo al adjuntar el segmento info %d: %s",i,strerror(errno));
					exit(0);
		
				}
				
				
				int sum;
				for(int j = 0; j < N; j++){
					
					sum = 0;
					for(int k = 0; k< N; k++){
						sum+= (info->matrix1[i][k] * info->matrix2[k][j]);	
					
					}
				
					
					info->matrixRes[i][j] = sum;
					
				}
				shmdt(info);
				exit(0);
			}

		}

		
	
		for( int h = 0; h < N; h++){
			waitpid(procs[h], &status, 0);

		}
	
	for(int o = 0; o < N; o++){
		for(int p = 0; p < N; p++){
			printf("%d ",init->matrixRes[o][p]);
	
	
		}
		printf("\n");
	
	
	}
	
	
	
	shmdt(init);
	shmctl(id,IPC_RMID,NULL);
	
	
	exit(0);	
}
