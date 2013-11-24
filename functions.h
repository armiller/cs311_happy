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
#include<strings.h>
#include"functions.c"

#define _BSD_SOURCE
#define _POSIX_C_SOURCE

#define setbit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define clearbit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )            
#define testbit(A,k)    ( A[(k/32)] & (1 << (k%32)) )

void errEXIT(char *text);

void usage(char *prog_name);

int get_args(int argc, char *argv[], int *num_procs);
