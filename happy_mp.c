/*
 * Author: Anthony Miller
 * Date: 11/21/2013
 * Name: happy_mp.c
 *
 * Multi-proccess implimintaion for finding primes and happy numbers
 */
#include<sys/mman.h>
#include<sys/stat.h>
#include<semaphore.h>
#include<sys/time.h>
#include<wait.h>
#include<signal.h>
#include"functions.h"
#define CACHE 256
#define _POSIX_C_SOURCE

enum { h_unknown = 0, h_yes, h_no };
int *sieve_index;
int *happy_index;
int *numhappy;
unsigned char buf[CACHE] = {0, h_yes, 0};

static void parent_sig(int sig) 
{
	_exit(EXIT_FAILURE);
}

static void child_sig(int sig) 
{
	_exit(EXIT_FAILURE);
}
 
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
/*
 * Mount Shem
 *
 * char *path = Name of shared memory object
 * int object_size = Size of object
 *
 * This is taken from the course website.
 *
 * http://classes.engr.oregonstate.edu/eecs/fall2013/cs311-001/examples/shmem.c
 */
void *mount_shmem(char *path, int object_size){
	int shmem_fd;
	void *addr;

	/* create and resize it */
	shmem_fd = shm_open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (shmem_fd == -1)
		errEXIT("failed to open shared memory object");

	/* resize it to something reasonable */
	if (ftruncate(shmem_fd, object_size) == -1)
		errEXIT("failed to resize shared memory object");

	//0xdeadbeef
	addr = mmap(NULL, object_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmem_fd, 0);
	if (addr == MAP_FAILED)
		errEXIT("failed to map shared memory object");

	return addr;
}

/*
 * mp_sieve
 * int maxnum = number to find up to
 * int *bitmap = bitmap pointer
 * sem_t *in_sem = sem pointer 
 *
 * Runs sieve in parrall with each process getting a unique index
 */
int mp_sieve(int maxnum, int *bitmap, sem_t *in_sem) {

	int prime = 2;
	int local;
	int j;
	int s;
	int semvalue;

	while (prime < (int) sqrt(maxnum)) {
		s = sem_wait(in_sem);
		if (s != 0)
			errEXIT("sem lock");

		prime = *sieve_index;
		*sieve_index = *sieve_index + 1;
		s = sem_post(in_sem);
		if (s != 0) 
			errEXIT("sem unlock");

		local = testbit(bitmap, prime);

		if (!local) {

			for (j = 2; prime * j < maxnum; j++) {
				setbit(bitmap, (prime * j));
			}
		}
	}

	sem_close(in_sem);

	exit(EXIT_SUCCESS);

	return 0;
}

int main (int argc, char *argv[]) 
{
	int procs; 
	int *bitmap;
	int bitmap_size;
	int opt;
	int n;
	int m;
	int i;
	int b;
	int k;
	void *addr; 
	int objectsize;
	struct sigaction p_sa;
	struct sigaction c_sa;
	sem_t *sem;
	
	/* signals */
	sigemptyset(&c_sa.sa_mask);
	sigemptyset(&p_sa.sa_mask);
	p_sa.sa_flags = 0;
	c_sa.sa_flags = 0;
	p_sa.sa_handler = parent_sig; 
	c_sa.sa_handler = child_sig;
	if (sigaction(SIGINT, &p_sa, NULL) == -1)
		errEXIT("sigaction");
	if (sigaction(SIGQUIT, &p_sa, NULL) == -1)
		errEXIT("sigaction");
	if (sigaction(SIGHUP, &p_sa, NULL) == -1 )
		errEXIT("sigaction");

	/* Open shared memory object */
	sem = sem_open("milleant_sem", O_CREAT, S_IRUSR | S_IWUSR, 1);

	if (!sem) 
		errEXIT("sem creation");

	/* get arguments */
	n = get_args(argc, argv, &procs);
	objectsize = 1024 * 1024 * 512;
	bitmap_size = 2147483646 / 32 + 1;

	addr = mount_shmem("milleant", objectsize);

	/* mapping memory pointers */
	bitmap = (int*) addr;
	sieve_index = (int*) (bitmap + bitmap_size + 1);
	*sieve_index = 2;
	happy_index = (int*) (sieve_index + sizeof(int*));
	*happy_index = 0; 
	numhappy = (int *) (happy_index + sizeof(int*));

	bzero(bitmap, bitmap_size);

	/* Sieve child forks */
	for (i = 0; i < procs; i++) {

		switch(fork()) {
			case -1:
				errEXIT("Child creation");
			case 0:
				if (sigaction(SIGINT, &c_sa, NULL) == -1)
					errEXIT("sigaction");
				if (sigaction(SIGQUIT, &c_sa, NULL) == -1)
					errEXIT("sigaction");
				if (sigaction(SIGHUP, &c_sa, NULL) == -1)
					errEXIT("sigaction");
				mp_sieve(n, bitmap, sem);
			default:
				break;
		}
	}

	for (i = 0; i < procs; i++) {
		wait(NULL);
	}

	for (i = 0; i < n; i++) {

		if (!testbit(bitmap, i))
			printf("bit %d set\n", i);
	}

	/* happy child forks */
	for (i = 0; i < procs; i++) {

		switch(fork()) {

			case -1:
				errEXIT("Child creation");
			case 0:
				if (sigaction(SIGINT, &c_sa, NULL) == -1)
					errEXIT("sigaction");
				if (sigaction(SIGQUIT, &c_sa, NULL) == -1)
					errEXIT("sigaction");
				if (sigaction(SIGHUP, &c_sa, NULL) == -1)
					errEXIT("sigaction");

				printf("Child exueciting\n");
				b = 0;
				while (b < n) {
					sem_wait(sem);
					printf("b = %d happy_index %d\n", b, *happy_index);
					b = *happy_index;
					*happy_index = *happy_index + 1;
					sem_post(sem);
					if (!testbit(bitmap, b)) {
						printf("bit set!");
						printf("happy return %d\n", happy(b));
						if (happy(b)) 
							printf("Happy is happy!\n");
							*numhappy = *numhappy + 1;	
					}
				}
				exit(EXIT_SUCCESS);
			default:
				break;
		}
	}

	for (i = 0; i < procs; i++) {
		wait(NULL);
	}

	printf("Number of happy numbers %d\n", *numhappy);

	shm_unlink("milleant");

	sem_unlink("milleant_sem");

	return 0;
}
