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
	if (shmem_fd == -1){
		fprintf(stdout, "failed to open shared memory object\n");
		exit(EXIT_FAILURE);
	}
	/* resize it to something reasonable */
	if (ftruncate(shmem_fd, object_size) == -1){
		fprintf(stdout, "failed to resize shared memory object\n");
		exit(EXIT_FAILURE);		
	}

	//0xdeadbeef
	addr = mmap(NULL, object_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmem_fd, 0);
	if (addr == MAP_FAILED){
		fprintf(stdout, "failed to map shared memory object\n");
		exit(EXIT_FAILURE);
	}

	return addr;
}

int main (int argc, char *argv[]) 
{
	int procs; 
	int opt;
	int n;
	int m;
	int i;
	int k;
	int *bitmap; 

	n = get_args(argc, argv, &procs);

	bitmap = malloc(((n/32) + 1) * sizeof(int));

	bzero(bitmap, sizeof(bitmap));

	return 0;
}
