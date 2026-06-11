#include <stdio.h>
#include <unistd.h> //fork(), getpid(), getppid()
#include <sys/wait.h> //wait()

int main() {
	pid_t pid;

	pid = fork();

	if (pid < 0) {
		//ERROR
		perror("fork failed");
		return 1;
	}

	if (pid == 0) {
		//proceso hijo
		printf("HIJO: Mi PID es %d, el PID de mi padre es %d\n", getpid(), getppid());
		sleep(2); //simula trabajo del hijo
	} else {
		//proceso padre
		printf("PADRE: Mi PID es %d, el PID de mi padre es %d\n", getpid(), getppid());
		wait(NULL);

		printf("PADRE: Mi hijo terminó, Fin del programa.\n");
	}

	return 0;
}
