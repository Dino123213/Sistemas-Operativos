#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <math.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define ROUTE "/bin/ls"
#define ID 10

struct msg
{
	long tipo;
	int tipoOp;
	int cantIteraciones;
	int res;
	int clienteID;
};

void *op1(void *args)
{
	key_t key = ftok(ROUTE, ID);
	int qid = msgget(key, IPC_CREAT | 0666);
	struct msg m;
	int sum, n;
	while (1)
	{
		msgrcv(qid, &m, sizeof(struct msg) - sizeof(long), 1, 0);
		sum = 0;
		n = m.cantIteraciones;
		for (int i = 0; i < n; i++)
			sum += 2 * i;
		m.res = sum;
		m.tipo = m.clienteID;
		msgsnd(qid, &m, sizeof(struct msg) - sizeof(long), 0);
	}
}
void *op2(void *args)
{
	key_t key = ftok(ROUTE, ID);
	int qid = msgget(key, IPC_CREAT | 0666);
	struct msg m;
	int fact, n;
	while (1)
	{
		msgrcv(qid, &m, sizeof(struct msg) - sizeof(long), 2, 0);
		fact = 1;
		n = m.cantIteraciones;
		for (int i = n; i > 0; i--)
			fact *= i;
		m.res = fact;
		m.tipo = m.clienteID;
		msgsnd(qid, &m, sizeof(struct msg) - sizeof(long), 0);
	}
}
void *op3(void *args)
{
	key_t key = ftok(ROUTE, ID);
	int qid = msgget(key, IPC_CREAT | 0666);
	struct msg m;
	int sum, n;
	while (1)
	{
		msgrcv(qid, &m, sizeof(struct msg) - sizeof(long), 3, 0);
		sum = 0;
		n = m.cantIteraciones;
		for (int i = 0; i < n; i++)
			sum += pow(2, i);
		m.res = sum;
		m.tipo = m.clienteID;
		msgsnd(qid, &m, sizeof(struct msg) - sizeof(long), 0);
	}
}
void *op4(void *args)
{
	key_t key = ftok(ROUTE, ID);
	int qid = msgget(key, IPC_CREAT | 0666);
	struct msg m;
	int prod, n;
	while (1)
	{
		msgrcv(qid, &m, sizeof(struct msg) - sizeof(long), 4, 0);
		prod = 1;
		n = m.cantIteraciones;
		for (int i = 1; i < n * 2; i += 2)
			prod *= i;
		m.res = prod;
		m.tipo = m.clienteID;
		msgsnd(qid, &m, sizeof(struct msg) - sizeof(long), 0);
	}
}
void cliente(int qid, int clienteID)
{
	srand(getpid());
	int sel, n;
	struct msg m;
	for (int i = 0; i < 1; i++)
	{
		sel = rand() % 4 + 1;
		n = rand() % 10;
		m.tipo = 5;
		m.clienteID = clienteID;
		m.tipoOp = sel;
		m.cantIteraciones = n;
		msgsnd(qid, &m, sizeof(struct msg) - sizeof(long), 0);
		msgrcv(qid, &m, sizeof(struct msg) - sizeof(long), clienteID, 0);
		printf("Resultado: %i obtenido por cliente %i\n", m.res, clienteID);
	}
}
void servidor(int qid)
{
	struct msg m;
	pthread_t op1_t, op2_t, op3_t, op4_t;
	pthread_create(&op1_t, NULL, op1, NULL);
	pthread_create(&op2_t, NULL, op2, NULL);
	pthread_create(&op3_t, NULL, op3, NULL);
	pthread_create(&op4_t, NULL, op4, NULL);
	while (1)
	{
		msgrcv(qid, &m, sizeof(struct msg) - sizeof(long), 5, 0);
		printf("Realizo op numero %i con iteraciones %i para cliente %i\n", m.tipoOp, m.cantIteraciones, m.clienteID);
		switch (m.tipoOp)
		{
		case 1:
		{
			m.tipo = 1;
			msgsnd(qid, &m, sizeof(struct msg) - sizeof(long), 0);
		}
		break;
		case 2:
		{
			m.tipo = 2;
			msgsnd(qid, &m, sizeof(struct msg) - sizeof(long), 0);
		}
		break;
		case 3:
		{
			m.tipo = 3;
			msgsnd(qid, &m, sizeof(struct msg) - sizeof(long), 0);
		}
		break;
		case 4:
		{
			m.tipo = 4;
			msgsnd(qid, &m, sizeof(struct msg) - sizeof(long), 0);
		}
		}
	}
	pthread_join(op1_t, NULL);
	pthread_join(op2_t, NULL);
	pthread_join(op3_t, NULL);
	pthread_join(op4_t, NULL);
}

int main(int argc, char **argv)
{
	key_t key = ftok(ROUTE, ID);
	int qid = msgget(key, IPC_CREAT | 0666);
	if (fork() == 0)
	{
		servidor(qid);
		exit(0);
	}
	for (int i = 0; i < 10; i++)
		if (fork() == 0)
		{
			cliente(qid, i + 10);
			exit(0);
		}
	for (int i = 0; i < 11; i++)
		wait(NULL);
	msgctl(qid, IPC_RMID, NULL);
	return 0;
}
