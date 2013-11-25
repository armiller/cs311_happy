/*
 * Author: Anthony Miller
 * Date: 11/21/2013
 * Name: happy_th.c
 *
 * Threaded implimintaion for finding primes and happy numbers
 */

#include"functions.h"
#include<pthread.h>
#include<error.h>
#include<math.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int avail = 2;

struct th_data {
	pthread_t pid;
	int thread_num;
	int maxnum;
	int i;
	int *bit_pointer;
};

static void *thread_func (void *arg)
{
	int local;
	int s;
	int j;
	int prime;
	struct th_data *thread = (struct th_data*) arg;
	int n = thread->maxnum;


	while (avail < (int)sqrt(thread->maxnum)) {
	//	printf("avail %d\n", avail);
		prime = avail;
		avail++;
		//s = pthread_mutex_lock(&mtx);
		if (s != 0) {
			errEXIT("mutex lock");
		}
		if (!testbit(thread->bit_pointer, prime)) {
		//	s = pthread_mutex_unlock(&mtx);
			if (s != 0) {
				errEXIT("mutex unlock");
			}
			for (j = 2; prime * j < n; j++) {
				setbit(thread->bit_pointer, (prime * j));
			}
		}
	}

	return 0;
}

int main (int argc, char *argv[]) 
{
	int procs; 
	int *bitmap;
	int opt;
	int n;
	int i;
	int s;
	struct th_data *threads; 

	n = get_args(argc, argv, &procs);

	threads = (struct th_data*) malloc(procs * sizeof(struct th_data));

	bitmap = malloc(((n/32) + 1) *sizeof(int));

	bzero(bitmap, (((n/32) + 1) * sizeof(int))); 

	for (i = 0; i < procs; i++) {
		threads[i].thread_num = i + 1;
		threads[i].maxnum = n;
		threads[i].bit_pointer = bitmap;
		s = pthread_create(&threads[i].pid, NULL, thread_func, (void *)&threads[i]);
		if (s != 0) {
			error(EXIT_FAILURE, s, "%s\n", "Pthread creation");
		}
	}

	for (i = 0; i < procs; i++) {
		s = pthread_join(threads[i].pid, NULL);
		if (s != 0) {
			error(EXIT_FAILURE, s, "%s\n", "Pthread join");
		}
	}

	for (i = 1; i < n; i++) {
		if (!testbit(bitmap, i))
			printf("bit %d set\n", i);
	}

	return 0;
}
