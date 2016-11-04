#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>	/*For using gettimeofday() function*/
#include <stdlib.h>     /*exit function*/
#include <sys/ipc.h>
#include <sys/shm.h>    /* For Shared Memory */
#include <sys/wait.h>


#define DEBUG 0
#define BUFFER_SIZE 2

