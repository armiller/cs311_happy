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
#include<sys/stat.h>
#include<math.h>
#include<signal.h>
#define CACHE 256

int avail = 2;
int happy_ind = 1;
int total_happy = 0;
pthread_mutex_t mutex_avail;

enum { h_unknown = 0, h_yes, h_no };
unsigned char buf[CACHE] = {0, h_yes, 0};

struct th_data {
	pthread_t pid;
	int thread_num;
	int maxnum;
	int i;
	int *bit_pointer;
	int next_block;
	int next_index;
	int block_size;
	int start_block;
	int start_index;
	int block;
}; 

struct happy_data {
	pthread_t pid;
	int num;
	int *bit_pointer;
};
 
/* 
 * Happy
 *
 * int n = variable to check
 *
 * Taken from rosettacode example
 *
 * http://rosettacode.org/wiki/Happy_numbers
 */
int happy(int n)
{
	int sum = 0, x, nn;
	if (n < CACHE) {
		if (buf[n]) return 2 - buf[n];
		buf[n] = h_no;
	}
 
	for (nn = n; nn; nn /= 10) x = nn % 10, sum += x * x;
 
	x = happy(sum);
	if (n < CACHE) buf[n] = 2 - x;
	return x;
}



static void parent_sig(int sig) {

	_exit(EXIT_FAILURE);
}


/*
 * Happy Thread
 * 
 * Parllel happy process with threads.
 * each thread gets a unique index
 */
static void *happy_thread (void *arg)
{
	struct happy_data *thread = (struct happy_data*) arg;
	int local = 1;
	int s;

	while (local < thread->num) {

		s = pthread_mutex_lock(&mutex_avail);
		if (s != 0) {
			errEXIT("mutex lock");
		}
		local = happy_ind;
		happy_ind++;
		s = pthread_mutex_unlock(&mutex_avail);
		if (s != 0) {
			errEXIT("mutex lock");
		}
		if (!testbit(thread->bit_pointer, local)) {
			if (happy(local)) {
				s = pthread_mutex_lock(&mutex_avail);
				if (s != 0) {
					errEXIT("mutex lock");
				}
				total_happy++;
				s = pthread_mutex_unlock(&mutex_avail);
				if (s != 0) {
					errEXIT("mutex lock");
				}
			}
		}
	}

	pthread_exit((void *) 0);
}
	
/* 
 * thread_func
 *
 * main parrelle sieve function that gives each thread 
 * a unique index
 */
static void *thread_func (void *arg) 
{ 
	int local; 
	int s; 
	int local_index;
	int next_index;
	int block;
	int next_block;
	int block_size;
	int j;
	int prime = 2;
	struct th_data *thread = (struct th_data*) arg;
	int n = thread->maxnum;

	while (prime < (int)sqrt(thread->maxnum)) {

		pthread_mutex_lock(&mutex_avail);
		if (s != 0) {
			errEXIT("mutex lock");
		}
		prime = avail;
		avail++;
		pthread_mutex_unlock(&mutex_avail);
		if (s != 0) {
			errEXIT("mutex lock");
		}

		pthread_mutex_lock(&mutex_avail);
		if (s != 0) {
			errEXIT("mutex unlock");
		}
		local = testbit(thread->bit_pointer, prime);
		pthread_mutex_unlock(&mutex_avail);
		if (s != 0) {
			errEXIT("mutex unlock");
		}
		if (!local) {

			for (j = 2; prime * j < n; j++) {

				pthread_mutex_lock(&mutex_avail);
				if (s != 0) {
					errEXIT("mutex unlock");
				}
				setbit(thread->bit_pointer, (prime * j));
				pthread_mutex_unlock(&mutex_avail);
				if (s != 0) {
					errEXIT("mutex unlock");
				}
			}
		}
	}

	pthread_exit((void *) 0);
}

int main (int argc, char *argv[]) 
{
	int procs; 
	int *bitmap;
	int opt;
	int n;
	int i;
	int block = 0;
	int index = 2;
	int s;
	struct th_data *threads; 
	struct happy_data *hp_threads;
	pthread_attr_t attr;
	struct sigaction p_sa;

	/* signals */
	sigemptyset(&p_sa.sa_mask);
	if (sigaction(SIGINT, &p_sa, NULL) == -1)
		errEXIT("sigaction");
	if (sigaction(SIGQUIT, &p_sa, NULL) == -1)
		errEXIT("sigaction");
	if (sigaction(SIGHUP, &p_sa, NULL) == -1 )
		errEXIT("sigaction");

	/* mutexs */
	pthread_mutex_init(&mutex_avail, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	n = get_args(argc, argv, &procs);

	threads = (struct th_data*) malloc(procs * sizeof(struct th_data));
	hp_threads = (struct happy_data*) malloc(procs * sizeof(struct happy_data));

	bitmap = malloc(((n/32) + 1) *sizeof(int));

	bzero(bitmap, (((n/32) + 1) * sizeof(int))); 

	/* sieve worker threads */
	for (i = 0; i < procs; i++) {
		threads[i].thread_num = i + 1;
		threads[i].maxnum = n;
		threads[i].bit_pointer = bitmap;
		s = pthread_create(&threads[i].pid, &attr, thread_func, (void *)&threads[i]);
		if (s != 0) {
			error(EXIT_FAILURE, s, "%s\n", "Pthread creation");
		}
	}

	/* close sieve worker threads */
	for (i = 0; i < procs; i++) {
		s = pthread_join(threads[i].pid, NULL);
		if (s != 0) {
			error(EXIT_FAILURE, s, "%s\n", "Pthread join");
		}
	}

	/* happy worker threads */
	for (i = 0; i < procs; i++) {
		hp_threads[i].num = n;
		hp_threads[i].bit_pointer = bitmap;
		s = pthread_create(&hp_threads[i].pid, &attr, happy_thread, (void *)&hp_threads[i]);
		if (s != 0) {
			error(EXIT_FAILURE, s, "%s\n", "Pthread creation");
		}
	}

	/* close happy worker threads */
	for (i = 0; i < procs; i++) {
		s = pthread_join(hp_threads[i].pid, NULL);
		if (s != 0) {
			error(EXIT_FAILURE, s, "%s\n", "Pthread join");
		}
	}

	pthread_attr_destroy(&attr);

	pthread_mutex_destroy(&mutex_avail);

	printf("Total happy %d\n", total_happy);

	return 0;
}
