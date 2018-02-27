#define MAX_CHARACTER 256

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>

const int SIZE = 4096;
const char *name = "OS";

void cleanSHM(){
  /* remove the shared memory segment */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}
}
void writeSHM(char* message)
{
	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
	sprintf(ptr,"%s",message);

}

void readSHM(){

	int shm_fd;
	void *ptr;
	int i;

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("Nothing in shared memory\n");
		return;
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	/* now read from the shared memory region */
	printf("%s \n",ptr);

  cleanSHM();

}

/* Asks the user for a sttring */
void readLine(char *name, int size){
  fgets(name, size, stdin);
  name[strcspn(name, "\n")] = '\0';      // retunrs the index of \n and replace it by a 0
}

int main ()
{

  readSHM();
  char *name = (char *)malloc(sizeof MAX_CHARACTER);
  char *choice = (char *)malloc(sizeof MAX_CHARACTER);
  printf("Enter Name and Date: \n");
  readLine(name, MAX_CHARACTER);
  writeSHM(name);

  while(1){
   //Read Employees frm file
      printf("Do you want to terminate? y/n \n");
      readLine(choice, MAX_CHARACTER);
      if(choice[0] ==  'y'){
        cleanSHM();
        return 0;
      }
      //printf("%s\n", choice);
  }
}
