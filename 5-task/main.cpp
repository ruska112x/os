#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t north_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t south_cond = PTHREAD_COND_INITIALIZER;

int north_waiting = 0;
int south_waiting = 0;
int north_crossing = 0;
int south_crossing = 0;

struct car_args {
  int id;
  int direction; // 0 - north, 1 - south
};

void *car(void *arg) {
  car_args args = *((car_args *)arg);

  pthread_mutex_lock(&mutex);
  if (args.direction == 0) {
    north_waiting++;
    while (south_crossing > 0 || (south_waiting > 0 && north_crossing == 0)) {
      pthread_cond_wait(&north_cond, &mutex);
    }
    north_waiting--;
    north_crossing++;
  } else {
    south_waiting++;
    while (north_crossing > 0 || (north_waiting > 0 && south_crossing == 0)) {
      pthread_cond_wait(&south_cond, &mutex);
    }
    south_waiting--;
    south_crossing++;
  }
  pthread_mutex_unlock(&mutex);

  if (args.direction == 0) {
    printf("%d North Car is crossing the bridge\n", args.id);
  } else {
    printf("%d South Car is crossing the bridge\n", args.id);
  }
  sleep(1);

  pthread_mutex_lock(&mutex);
  if (args.direction == 0) {
    north_crossing--;
    if (north_crossing == 0 && south_waiting > 0) {
      pthread_cond_broadcast(&south_cond);
    } else {
      pthread_cond_broadcast(&north_cond);
    }
  } else {
    south_crossing--;
    if (south_crossing == 0 && north_waiting > 0) {
      pthread_cond_broadcast(&north_cond);
    } else {
      pthread_cond_broadcast(&south_cond);
    }
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <number_of_cars>\n", argv[0]);
    return 1;
  }

  int batch_size = atoi(argv[1]);
  if (batch_size < 1) {
    fprintf(stderr, "Number of cars can't be less than 1\n");
    return 1;
  }

  while (true) {
    int size = 1 + rand() % batch_size;

    pthread_t cars[size];
    car_args c_args[size];

    for (int i = 0; i < size; i++) {
      c_args[i].id = i + 1;
      c_args[i].direction = rand() % 2;
      pthread_create(&cars[i], NULL, car, &c_args[i]);
    }
    for (int i = 0; i < size; i++) {
      pthread_join(cars[i], NULL);
    }
    printf("---------------------------------------\n");
  }

  return 0;
}
