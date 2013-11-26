/*
 * Author: Anthony Miller
 * Date: 11/21/2013
 * Name: happy_mp.c
 *
 * Multi-proccess implimintaion for finding primes and happy numbers
 */
#include<sys/mman.h>
#include<sys/stat.h>
#include"functions.h"

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

//int mp_sieve(int maxnum) {
//
//	int prime;
//
//	while (prime < (int) sqrt(maxnum)) {
//
//	}
//}

int main (int argc, char *argv[]) 
{
	int procs; 
	unsigned int *bitmap
	int opt;
	int n;
	int m;
	int i;
	int k;
	void *addr; 
	int objectsize;

	n = get_args(argc, argv, &procs);
	objectsize = 1024 * 1024 * 512;

	addr = mount_shmem("milleant", objectsize);

	bitmap = (int*) addr;

	for (i = 0; i < procs; i++) {
		switch(fork()) {
			case -1:
				errEXIT("Child creation");
			case 0:
				break;

			default:
				break;



		}
	}
	

	return 0;
}
