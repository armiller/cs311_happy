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

int avail = 2;
pthread_mutex_t mutex_avail;

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

	local_index = thread->start_index;
	next_index = thread->next_index;
	next_block = thread->next_block;
	block_size = thread->block_size;
	block = thread->block;

	printf("thread %ld block %d local_index %d next_index %d next_block %d\n", thread->pid, block, local_index, next_index, next_block);

	while (prime < (int) sqrt(n)) {
		
		if (!testbit(thread->bit_pointer, block + prime)) {
			for (j = 2; prime * j < next_block; j++) {
				setbit(thread->bit_pointer, block + (prime * j));
			}
		}

		prime++;
	}

	//while (prime < (int)sqrt(thread->maxnum)) {

	//	pthread_mutex_lock(&mutex_avail);
	//	if (testbit(thread->bit_pointer, avail)){
	//		
	//	}
	//	prime = avail;
	//	avail++;
	//	pthread_mutex_unlock(&mutex_avail);
	//	//s = pthread_mutex_lock(&mtx);
	//	//if (s != 0) {
	//	//	errEXIT("mutex lock");
	//	//}
	//	if (!testbit(thread->bit_pointer, prime)) {
	//	//	s = pthread_mutex_unlock(&mtx);
	//		//if (s != 0) {
	//		//	errEXIT("mutex unlock");
	//		//}
	//		for (j = 2; prime * j < n; j++) {
	//			setbit(thread->bit_pointer, (prime * j));
	//		}
	//	}
	//}

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
	int thread_block;
	int thread_index;
	int s;
	struct th_data *threads; 
	pthread_attr_t attr;

	pthread_mutex_init(&mutex_avail, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	n = get_args(argc, argv, &procs);

	threads = (struct th_data*) malloc(procs * sizeof(struct th_data));

	bitmap = malloc(((n/32) + 1) *sizeof(int));

	bzero(bitmap, (((n/32) + 1) * sizeof(int))); 

	thread_index = (int) sqrt(n) / procs;

	thread_block = n / procs;

	for (i = 0; i < procs; i++) {
		threads[i].start_index = index;
		threads[i].block = block;
		block += thread_block;
		index += thread_index;

		threads[i].next_block = block;
		threads[i].next_index = index;
		threads[i].thread_num = i + 1;
		threads[i].maxnum = n;
		threads[i].block_size = thread_block;
		threads[i].bit_pointer = bitmap;
		s = pthread_create(&threads[i].pid, &attr, thread_func, (void *)&threads[i]);
		if (s != 0) {
			error(EXIT_FAILURE, s, "%s\n", "Pthread creation");
		}
	}

	pthread_attr_destroy(&attr);

	for (i = 0; i < procs; i++) {
		s = pthread_join(threads[i].pid, NULL);
		if (s != 0) {
			error(EXIT_FAILURE, s, "%s\n", "Pthread join");
		}
	}

	pthread_mutex_destroy(&mutex_avail);

	for (i = 1; i < n; i++) {
		if (!testbit(bitmap, i))
			printf("bit %d set\n", i);
	}

	return 0;
}
