#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Initialize mutex
int direction = 0; // 0: North, 1: South (initially North)

void *northbound(void *arg) {
  while (1) {
    printf("Car approaching from the north\n");
    pthread_mutex_lock(&mutex); // Acquire lock

    if (direction != 0) {
      printf("Northbound car waiting...\n");
      while (direction != 0) {
        pthread_cond_wait(NULL, &mutex); // Wait for southbound cars
      }
    }

    printf("Northbound car crossing the bridge\n");
    direction = 1; // Switch direction to South

    pthread_mutex_unlock(&mutex); // Release lock
    sleep(2);                     // Simulate crossing time
  }
}

void *southbound(void *arg) {
  while (1) {
    printf("Car approaching from the south\n");
    pthread_mutex_lock(&mutex); // Acquire lock

    if (direction != 1) {
      printf("Southbound car waiting...\n");
      while (direction != 1) {
        pthread_cond_wait(NULL, &mutex); // Wait for northbound cars
      }
    }

    printf("Southbound car crossing the bridge\n");
    direction = 0; // Switch direction to North

    pthread_mutex_unlock(&mutex); // Release lock
    sleep(2);                     // Simulate crossing time
  }
}

int main() {
  pthread_t northbound_thread, southbound_thread;

  pthread_create(&northbound_thread, NULL, northbound, NULL);
  pthread_create(&southbound_thread, NULL, southbound, NULL);

  pthread_join(northbound_thread, NULL);
  pthread_join(southbound_thread, NULL);

  pthread_mutex_destroy(&mutex); // Destroy mutex
  return 0;
}
