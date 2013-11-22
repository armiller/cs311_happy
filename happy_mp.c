/*
 * Author: Anthony Miller
 * Date: 11/21/2013
 * Name: happy_mp.c
 *
 * Multi-proccess implimintaion for finding primes and happy numbers
 */

#include"functions.h"

int main (int argc, char *argv[]) 
{
	int procs; 
	int opt;
	int n;
	int i;
	int *bitmap; 

	n = get_args(argc, argv, &procs);

	bitmap = malloc(((n/32) + 1) * sizeof(int));

	bzero(bitmap, sizeof(bitmap));

	return 0;
}
