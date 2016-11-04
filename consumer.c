/************************************
Name:           Manu Khandelwal
Student #ID:    5272109
CSE Lab Login:  khand055
Assignment No:  4
**************************************/

#include "prodcon.h"

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

int flag = 1023;  

struct timeval tv;
unsigned long time_in_micros;
int totalItemToConsume = 3000;


void main(int argc, char *argv[])
{

	/* Creating Shared memory segment for the buffer data*/
    shmem_id = shmget (key_buffer, 0, IPC_CREAT);
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

	FILE *fp_cons;
	char consumer_filename[100] = "Consumer.txt";
	remove(consumer_filename);
	fp_cons = fopen(consumer_filename, "a");

	do {
         pthread_mutex_lock (&sharedDataRef->shmem_ptr_lock);    /* Enter critical section  */
         while ( sharedDataRef->shmem_ptr_count == 0 )     /* Make sure that buffer is NOT empty */
             while (  pthread_cond_wait( &sharedDataRef->shmem_ptr_item_available, &sharedDataRef->shmem_ptr_lock) != 0 ) ; /* Sleep using a cond variable *

         /* count must be > 0    */
         if  ( sharedDataRef->shmem_ptr_count > 0 ) {
                fprintf(fp_cons, "%-10s", sharedDataRef->shmem_ptr_buffer[sharedDataRef->shmem_ptr_out]);
                
               sharedDataRef->shmem_ptr_out = (sharedDataRef->shmem_ptr_out + 1) % BUFFER_SIZE; 
               if(DEBUG)
                    printf("Consumer Consumed item %d\n", sharedDataRef->shmem_ptr_count);
               (sharedDataRef->shmem_ptr_count)--; 
               totalItemToConsume--;          
         }

         pthread_mutex_unlock ( &sharedDataRef->shmem_ptr_lock);   /* exit critical seciton  */
         pthread_cond_signal( &sharedDataRef->shmem_ptr_space_available );  /* Wakeup prodcuer, if waiting */

   } while ( totalItemToConsume > 0 );

    shmdt(sharedDataRef);
    fclose(fp_cons);
}