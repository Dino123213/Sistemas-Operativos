#include <stdio.h>
#include <unistd.h>    // fork(), execlp()
#include <sys/wait.h>  // wait(), waitpid()

int main() { 
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork failed");
		return 1;
	}

	if (pid == 0) {
		// **Proceso hijo**
		printf("HIJO: Ejecutando 'ls -l' usando exec...\n");

		execlp("ls", "ls", "-l", NULL);

		// Si exec falla, esta línea se ejecuta
		perror("exec failed");
		return 1;
	} else {
		// **Proceso padre**
		wait(NULL);
		printf("PADRE: El hijo terminó. Fin del programa.\n");
	}

	return 0;
}
