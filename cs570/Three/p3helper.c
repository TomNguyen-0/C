// CS 570 - program 3
// Name: Tom Nguyen
// Professor: Dr. John Carroll
// Username: cssc0062@edoras.sdsu.edu
// April 06, 2018
// File: p3.c
/*
 * 3helper.c
 * Program 3 assignment
 * CS570
 * SDSU
 * Spring 2018
 *
 * This is the ONLY file you are allowed to change. (In fact, the other
 * files should be symbolic links to
 *   ~cs570/Three/p3main.c
 *   ~cs570/Three/p3robot.c
 *   ~cs570/Three/p3.h
 *   ~cs570/Three/makefile
 *   ~cs570/Three/CHK.h    )
 * The file ~cs570/Three/createlinks is a script that will create these for you.
 */
#include "p3.h"
#define COURSEID "570"
/* You may put declarations/definitions here.
 * In particular, you will probably want access to information
 * about the job (for details see the assignment and the documentation
 * in p3robot.c):
 */
extern int nrRobots;
extern int seed;
extern int width;
extern int quota;

int flag_pmutx = O_RDWR|O_CREAT|O_EXCL;
int flag_fd = O_RDWR|O_CREAT|O_TRUNC;
int flag_fd2= S_IRUSR|S_IWUSR;
int file;
int counter=0;


sem_t *semaphore;//define a variable semaphore
char semaphoreArray[SEMNAMESIZE];//hold the name of semaphore file

/* General documentation for the following functions is in p3.h
 * Here you supply the code, and internal documentation:
 */
void initStudentStuff(void) {
	sprintf(semaphoreArray,"/%s%ldmutx",COURSEID,(long)getuid());
	CHK((semaphore = sem_open(semaphoreArray,flag_pmutx,flag_fd2,1)));//page 15 from Dr.Carroll

	if(semaphore == SEM_FAILED){
		CHK((int)(semaphore = sem_open(semaphoreArray,O_RDWR)));
		if ( file = open("countfile",O_RDWR) ){
			CHK(sem_wait(semaphore));//down
			CHK(file = open("/home/cs/carroll/cssc0062/Three/countfile",O_RDWR));
			CHK(sem_post(semaphore)); //up
		}
		else{
			CHK(sem_wait(semaphore));//down
			CHK(file = open("countfile",O_RDWR));
			CHK(sem_post(semaphore)); //up
		}
	}
	else{
		CHK(sem_wait(semaphore)); //down
		CHK(file = open("countfile",flag_fd,flag_fd2));
		counter =0;
		CHK(lseek(file,0,SEEK_SET));
		assert(sizeof(counter) == write(file,&counter,sizeof(counter))); 
		CHK(sem_post(semaphore)); //up
	}

}

/* In this braindamaged version of placeWidget, the widget builders don't
 * coordinate at all, and merely print a random pattern. You should replace
 * this code with something that fully follows the p3 specification.
 */
void placeWidget(int n) {
	//printeger(n);
	int total = nrRobots*quota;// the total number of widget that will display
	CHK(sem_wait(semaphore));//down
	CHK(lseek(file,0,SEEK_SET));
	assert(sizeof(counter) == read(file,&counter,sizeof(counter)));
	counter++;
	if( counter == total){//reach the end print F
		printeger(n);
		printf("F\n");
		fflush(stdout);
		CHK(close(file));//exit out and close/unlink files
		CHK(unlink("countfile"));
		CHK(sem_close(semaphore));
		CHK(sem_unlink(semaphoreArray));
	}
	else if(counter%width==0){//the number of widget for that row is now the width size.
		printeger(n);
		printf("N\n");
		fflush(stdout);
		CHK(lseek(file,0,SEEK_SET));
		assert(sizeof(counter) == write(file,&counter,sizeof(counter)));
		CHK(sem_post(semaphore));//up
	}
	else{//not the final widget or the end of row widget.
		printeger(n);
		fflush(stdout);
		CHK(lseek(file,0,SEEK_SET));
		assert(sizeof(counter) == write(file,&counter,sizeof(counter)));
		CHK(sem_post(semaphore));//up
	}

}

/* If you feel the need to create any additional functions, please
 * write them below here, with appropriate documentation:
 */

