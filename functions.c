/*
 * Author: Anthony Miller
 * Date: 11/21/2013
 * Name: functions.c
 *
 * Holds functions used by both implimintations
 */
void errEXIT(char *text) 
{
	perror(text);
	exit(EXIT_FAILURE);
}

void usage(char *prog_name) 
{
	printf("%s [-p <number or procs/threads>] <number>\n", prog_name);
	exit(EXIT_SUCCESS);
}

