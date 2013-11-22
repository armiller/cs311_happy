/*
 * Author: Anthony Miller
 * Date: 11/21/2013
 * Name: happy_th.c
 *
 * Threaded implimintaion for finding primes and happy numbers
 */

#include"functions.h"

int main (int argc, char *argv[]) 
{
	int procs;
	int opt;
	int n;

	if (argc < 2)
		usage(argv[0]);

	while ((opt = getopt(argc, argv, "p:h")) != -1) {
		switch(opt) {
			case 'p':
				procs = atoi(optarg);
				break;
			case 'h':
				usage(argv[0]);		
				break;	
			case '?':
				exit(EXIT_FAILURE);
		}
	}

	if (optind == argc) 
		usage(argv[0]);

	n = atoi(argv[optind]);

	return 0;
}
