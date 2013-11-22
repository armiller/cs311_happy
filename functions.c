/*
 * Author: Anthony Miller
 * Date: 11/21/2013
 * Name: functions.c
 *
 * Holds functions used by both implimintations
 */

/*
 * errEXIT
 *
 * char *text = text to print on error
 *
 * prints inputed text and displays error and exits
 */
void errEXIT(char *text) 
{
	perror(text);
	exit(EXIT_FAILURE);
}

/*
 * Usage
 *
 * char *prog_name = argv[0] 
 *
 * Outputs proper usage
 */
void usage(char *prog_name) 
{
	printf("%s [-p <number or procs/threads>] <number>\n", prog_name);
	exit(EXIT_SUCCESS);
}

/*
 * Get Args
 *
 * int argc = argc from main functions
 * char *argv[] = char pointer from main function
 * int *num_procs = integer pointer from main function that will populate number of procs
 *
 * returns max number of primes and number of processes to be run
 */
int get_args(int argc, char *argv[], int *num_procs)
{
	int opt;
	int n;
	if (argc < 2)
		usage(argv[0]);

	while ((opt = getopt(argc, argv, "p:h")) != -1) {
		switch(opt) {
			case 'p':
				num_procs = atoi(optarg);
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

	return n;
}

