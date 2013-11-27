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
#include"functions.h"


int *sieve_index;
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

int mp_sieve(int maxnum, int *bitmap, sem_t *in_sem) {

	int prime = 2;
	int local;
	int j;


	while (prime < (int) sqrt(maxnum)) {
		sem_wait(in_sem);
		prime = *sieve_index;
		*sieve_index = *sieve_index + 1;
		sem_post(in_sem);

//		sem_wait(in_sem);
		local = testbit(bitmap, prime);
//		sem_post(in_sem);

		if (!local) {

			for (j = 2; prime * j < maxnum; j++) {
//				printf("child %d setting bit %d\n", getpid(), (prime * j));
//				sem_wait(in_sem);
				setbit(bitmap, (prime * j));
//				sem_post(in_sem);
			}
		}
	}

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
	int k;
	void *addr; 
	int objectsize;
	sem_t *sem = sem_open("milleant_sem", O_CREAT, S_IRUSR | S_IWUSR, 1);

	if (!sem) 
		errEXIT("sem creation");

	n = get_args(argc, argv, &procs);
	objectsize = 1024 * 1024 * 512;
	bitmap_size = 2147483647 / 32 + 1;

	addr = mount_shmem("milleant", objectsize);

	bitmap = (int*) addr;
	sieve_index = (int*) (bitmap + bitmap_size + 1);
	*sieve_index = 2;

	bzero(bitmap, bitmap_size);

	for (i = 0; i < procs; i++) {

		switch(fork()) {
			case -1:
				errEXIT("Child creation");
			case 0:
//				printf("child %d sieve_index %d", getpid(), *sieve_index);
				mp_sieve(n, bitmap, sem);
				break;

			default:
				break;

		}
	}

	for (i = 0; i < procs; i++) {
		wait(NULL);
	}

	for (i = 1; i < n; i++) {
		if(!testbit(bitmap, i))
			printf("set bit %d\n", i);
	}

	shm_unlink("milleant");

	return 0;
}
