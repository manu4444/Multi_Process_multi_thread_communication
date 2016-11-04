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
 const int totalItemToProduce = 1000;

/* Child process executes this code */

void main(int argc, char *argv[])
{
	int i;

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


	FILE *fp_prod;
	char producer_filename[100] = "Producer";
	strcat(producer_filename, "_");
	strcat(producer_filename, argv[0]);
  strcat(producer_filename, ".txt");
	remove(producer_filename);
	fp_prod = fopen(producer_filename, "a");

	for ( i = 0; i< totalItemToProduce; i++) {
      pthread_mutex_lock ( &sharedDataRef->shmem_ptr_lock);  /* Enter critical section  */  

      while ( sharedDataRef->shmem_ptr_count == BUFFER_SIZE )          /* Make sure that buffer is NOT full   */
         while (  pthread_cond_wait( &sharedDataRef->shmem_ptr_space_available, &sharedDataRef->shmem_ptr_lock) != 0 ) ; /* Sleep using a cond variable */

       /* count must be less than N    */
       gettimeofday(&tv, NULL);
       time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;

       sprintf(sharedDataRef->shmem_ptr_buffer[sharedDataRef->shmem_ptr_in], "%-10s %lu \n", argv[0], time_in_micros);
       fprintf(fp_prod, "%s", sharedDataRef->shmem_ptr_buffer[sharedDataRef->shmem_ptr_in]);

       sharedDataRef->shmem_ptr_in = (sharedDataRef->shmem_ptr_in + 1) % BUFFER_SIZE;
       (sharedDataRef->shmem_ptr_count)++;
       if(DEBUG)
            printf("Producer produced item %d\n", sharedDataRef->shmem_ptr_count);               /* Increment the count of items in the buffer */

       pthread_mutex_unlock ( &sharedDataRef->shmem_ptr_lock );  
       pthread_cond_signal( &sharedDataRef->shmem_ptr_item_available ); /* Wakeup consumer, if waiting */
 }
   shmdt(sharedDataRef);
   fclose(fp_prod);
}