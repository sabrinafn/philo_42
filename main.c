#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *thread_function_1(void *arg);
void *thread_function_2(void *arg);

/*int	main(int ac, char **av)
{
	pthread_t	t1;
	pthread_t	t2;
	int	i = 5;

	pthread_create(&t1, NULL, (void *)&routine, NULL);
	pthread_create(&t2, NULL, (void *)&routine, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return (0);
}*/

// tempo de morrer = 5gb available
// temp de comer = 
// 200 tempo de morrer - 3 philo - consome a cada 100#include <stdio.h>

int main() {
    pthread_t thread1, thread2; // Thread identifiers
    int loop_count = 5;         // Number of iterations for the loop

    // Create the first thread
    if (pthread_create(&thread1, NULL, thread_function_1, &loop_count) != 0) {
        perror("Failed to create thread1");
        exit(1);
    }

    // Create the second thread
    if (pthread_create(&thread2, NULL, thread_function_2, &loop_count) != 0) {
        perror("Failed to create thread2");
        exit(1);
    }

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Both threads have finished.\n");
    return 0;
}

// Thread function for thread 1
void *thread_function_1(void *arg) {
    int loop_count = *(int *)arg; // Retrieve the loop count
    for (int i = 0; i < loop_count; i++) {
        printf("Thread 1: iteration %d\n", i + 1);
        sleep(1); // Simulate some work
    }
    return NULL;
}

// Thread function for thread 2
void *thread_function_2(void *arg) {
    int loop_count = *(int *)arg; // Retrieve the loop count
    for (int i = 0; i < loop_count; i++) {
        printf("Thread 2: iteration %d\n", i + 1);
        sleep(1); // Simulate some work
    }
    return NULL;
}

