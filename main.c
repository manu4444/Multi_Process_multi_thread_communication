/************************************
Name:           Manu Khandelwal
Student #ID:    5272109
CSE Lab Login:  khand055
Assignment No:  4
**************************************/

#include "prodcon.h"

struct timeval tv;
unsigned long time_in_micros;

pid_t consumer;
pid_t producer1;
pid_t producer2;
pid_t producer3;

key_t key_buffer = 6444;      /* A key to access shared memory segments */

struct sharedData {
    char shmem_ptr_buffer[BUFFER_SIZE][100];
    pthread_mutex_t shmem_ptr_lock;
    pthread_mutexattr_t shmem_ptr_attr_lock;
    pthread_cond_t shmem_ptr_space_available;
    pthread_cond_t shmem_ptr_item_available;
    pthread_condattr_t shmem_ptr_attr_space_available;
    pthread_condattr_t shmem_ptr_attr_item_available;
    int shmem_ptr_in;
    int shmem_ptr_out;
    int shmem_ptr_count;
};


int shmem_id;
struct sharedData * sharedDataRef;

int flag = 1023;                                /* Controls things like r/w permissions */

void main(int argc, char *argv[])
{
    int stat;

    /* Creating Shared memory segment for the buffer data*/
    shmem_id = shmget (key_buffer, sizeof(struct sharedData), flag);
    if (shmem_id == -1)
    {
        perror ("shmget failed");
        exit (1);
    }
    sharedDataRef = (struct sharedData *)shmat (shmem_id, (void *) NULL, flag);
    if (sharedDataRef == (void *) -1)
    {
        perror ("shmat failed");
        exit (2);
    }


    int rtn;

    /* Initializing the mutex variable attribute */
    if (rtn = pthread_mutexattr_init(&sharedDataRef->shmem_ptr_attr_lock)) {
        fprintf(stderr,"pthreas_mutexattr_init: %s",strerror(rtn)),exit(1);
    }
    if (rtn = pthread_mutexattr_setpshared ( &sharedDataRef->shmem_ptr_attr_lock, PTHREAD_PROCESS_SHARED)){
        fprintf(stderr,"pthread_mutexattr_setpshared %s",strerror(rtn)),exit(1);
    }
    if (rtn = pthread_mutex_init( &sharedDataRef->shmem_ptr_lock, &sharedDataRef->shmem_ptr_attr_lock )){
        fprintf(stderr,"pthread_mutex_init %s",strerror(rtn)), exit(1);
    }

    /* Initializing the condition variable -space- attribute */
    if (rtn = pthread_condattr_init(&sharedDataRef->shmem_ptr_attr_space_available)) {
        fprintf(stderr,"pthreas_condattr_init: %s",strerror(rtn)),exit(1);
    }
    if (rtn = pthread_condattr_setpshared ( &sharedDataRef->shmem_ptr_attr_space_available, PTHREAD_PROCESS_SHARED)){
        fprintf(stderr,"pthread_condattr_setpshared %s",strerror(rtn)),exit(1);
    }
    if (rtn = pthread_cond_init( &sharedDataRef->shmem_ptr_space_available, &sharedDataRef->shmem_ptr_attr_space_available )){
        fprintf(stderr,"pthread_cond_init %s",strerror(rtn)), exit(1);
    }

    /* Initializing the condition variable -item- attribute */
    if (rtn = pthread_condattr_init(&sharedDataRef->shmem_ptr_attr_item_available)) {
        fprintf(stderr,"pthreas_condattr_init: %s",strerror(rtn)),exit(1);
    }
    if (rtn = pthread_condattr_setpshared ( &sharedDataRef->shmem_ptr_attr_item_available, PTHREAD_PROCESS_SHARED)){
        fprintf(stderr,"pthread_condattr_setpshared %s",strerror(rtn)),exit(1);
    }
    if (rtn = pthread_cond_init( &sharedDataRef->shmem_ptr_item_available, &sharedDataRef->shmem_ptr_attr_item_available )){
        fprintf(stderr,"pthread_cond_init %s",strerror(rtn)), exit(1);
    }


    sharedDataRef->shmem_ptr_count = 0;
    sharedDataRef->shmem_ptr_in = 0;
    sharedDataRef->shmem_ptr_out = 0;

    if (consumer=fork()) 
    {
       if (producer1=fork()) 
       {
            if (producer2=fork()) 
            {
                if (producer3=fork()) 
                {
                    waitpid( consumer, &stat, 0 );
                    printf("Execution Finished!!\n");
                } 
                else {  /* this is the producer1 process */
                      if(DEBUG)  
                        printf("I am the newly producer1 process\n");
                    execl ("./prod",  "WHITE",  NULL);
                }
            } 
            else {  /* this is the producer2 process */
                if(DEBUG)
                    printf("I am the newly producer2 process\n");
                execl ("./prod",  "RED",  NULL);
            }
       } 
       else {   /* this is the producer3 process */
            if(DEBUG)
                printf("I am the newly producer3 process\n");
            execl ("./prod",  "BLACK",  NULL);
       }
       
    }
    else { /* this is the consumer process */
        if(DEBUG)
            printf("I am the newly created consumer process\n");
        execl ("./cons", "",  NULL);
    }
}