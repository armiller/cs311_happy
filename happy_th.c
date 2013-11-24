/*
 * Author: Anthony Miller
 * Date: 11/21/2013
 * Name: happy_th.c
 *
 * Threaded implimintaion for finding primes and happy numbers
 */

#include"functions.h"
#include<pthread.h>

int main (int argc, char *argv[]) 
{
	int procs; 
	int *bitmap;
	int opt;
	int n;
	int i;
	int s;
	pthread_t *threads;

	n = get_args(argc, argv, &procs);

	threads = malloc(procs * sizeof(pthread_t));

	bitmap = malloc(((n/32) + 1) *sizeof(int));

	bzero(bitmap, (((n/32) + 1) * sizeof(int))); 

	for (i = 0; i < procs; i++) {
	}

	return 0;
}
