#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 128

int pipe_impar[2];
int pipe_par[2];
int pipe_p_par[2];
int pipe_p_impar[2];

void lector(FILE* file){
	close(pipe_par[0]);
	close(pipe_impar[0]);
	int i = 0;
	char buffer[BUFFER_SIZE];
	while(fgets(buffer, sizeof(buffer), file) != NULL){
		printf("Linea %i: %s\n",i,buffer);
		if(i%2==0)
			write(pipe_par[1],&buffer,sizeof(buffer));
		else
			write(pipe_impar[1],&buffer,sizeof(buffer));
		i++;
	}
	close(pipe_par[1]);
	close(pipe_impar[1]);
	fclose(file);
	exit(0);
}

void impar(){
	close(pipe_par[0]);
	close(pipe_par[1]);
	close(pipe_impar[1]);
	close(pipe_p_impar[0]);
	close(pipe_p_par[0]);
	close(pipe_p_par[1]);
	char buffer[BUFFER_SIZE];
	int cont;
	while(1){//Es un bucle pq no tengo ganas de pasar la cantidad de lineas
		cont = 0;
		read(pipe_impar[0],&buffer,sizeof(buffer));
		for(int i = 0; i<BUFFER_SIZE;i++)
			if(buffer[i]=='A' || buffer[i]=='a')
				cont++;
		write(pipe_p_impar[1],&cont,sizeof(int));
	}
	close(pipe_impar[0]);
	close(pipe_p_impar[1]);
	exit(0);
}

void par(){
	close(pipe_par[1]);
	close(pipe_impar[0]);
	close(pipe_impar[1]);
	close(pipe_p_impar[0]);
	close(pipe_p_impar[1]);
	close(pipe_p_par[0]);
	char buffer[BUFFER_SIZE];
	int cont;
	while(1){//Es un bucle pq no tengo ganas de pasar la cantidad de lineas
		cont = 0;
		read(pipe_par[0],&buffer,sizeof(buffer));
		if(buffer[0]!=' ')
			cont++;
		for(int i = 1; i<BUFFER_SIZE-1;i++)
			if(buffer[i]==' ' && buffer[i+1]!=' ')
				cont++;
		write(pipe_p_par[1],&cont,sizeof(int));
	}
	close(pipe_par[0]);
	close(pipe_p_par[1]);
	exit(0);
}

int main(int argc, char** argv){
	close(pipe_par[0]);
	close(pipe_par[1]);
	close(pipe_impar[0]);
	close(pipe_impar[1]);
	close(pipe_p_impar[1]);
	close(pipe_p_par[1]);
	FILE* file = fopen("./texto.txt","r");
	int cantA,cantP;
	pipe(pipe_impar);
	pipe(pipe_par);
	pipe(pipe_p_impar);
	pipe(pipe_p_par);
	if(fork()==0)
		lector(file);
	else if(fork()==0)
			impar();
		 else if(fork()==0)
				par();
	while(1){//Es un bucle pq no tengo ganas de pasar la cantidad de lineas
		read(pipe_p_par[0],&cantP,sizeof(int));
		printf("Cantidad de palabras: %i\n",cantP);
		read(pipe_p_impar[0],&cantA,sizeof(int));
		printf("Cantidad de letras A: %i\n",cantA);
	}
	close(pipe_p_impar[0]);
	close(pipe_p_par[0]);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	return 0;
}
