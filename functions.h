/*
 * Author: Anthony Miller
 * Date: 11/21/2013
 * Name: functions.h
 *
 * defines functions and adds default includes
 */
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<getopt.h>
#include<sys/types.h>
#include<unistd.h>
#include"functions.c"

void errEXIT(char *text);

void usage(char *prog_name);

int get_args(int argc, char *argv[], int *num_procs);
