#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<getopt.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

void usage(char *prog_name) 
{
	printf("%s [-p <number or procs/threads>] <number>\n", prog_name);
	exit(EXIT_SUCCESS);
}

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
