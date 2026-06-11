/*
 * 2.c
 * 
 * Copyright 2024  <basher@raspberry>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

sem_t sa,sb,sc,sac;

void* a(void* data){
	while(1){
	sem_wait(&sa);
	sem_wait(&sac);
	sem_wait(&sac);
	printf("a\n");
	sem_post(&sb);
	sem_post(&sb);
	sem_post(&sc);
	} return 0;
}
void* b(void* data){
	while(1){
	sem_wait(&sb);
	printf("b\n");
	sem_post(&sac);
	} return 0;
}
void* c(void* data){
	while(1){
	sem_wait(&sc);
	sem_wait(&sac);
	sem_wait(&sac);
	printf("c\n");
	sleep(1);
	sem_post(&sb);
	sem_post(&sb);
	sem_post(&sa);
	} return 0;
}
int main(int argc, char **argv)
{
	pthread_t ta,tb,tc;
	sem_init(&sa,0,1);
	sem_init(&sb,0,0);
	sem_init(&sc,0,0);
	sem_init(&sac,0,2);
	pthread_create(&ta,NULL,a,NULL);
	pthread_create(&tb,NULL,b,NULL);
	pthread_create(&tc,NULL,c,NULL);
	pthread_join(ta,NULL);
	pthread_join(tb,NULL);
	pthread_join(tc,NULL);
	return 0;
}
