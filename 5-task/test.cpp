#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NORTH 0
#define SOUTH 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t north_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t south_cond = PTHREAD_COND_INITIALIZER;

int bridge_count = 0;
int bridge_direction = NORTH;

void *drive(void *arg) {
  int direction =
      rand() % 100; // Рандомное направление: 0 для NORTH, 1 для SOUTH
  // fprintf(stdout, "%d\n", direction);
  if (direction >= 49) {
    direction = NORTH;
  } else {
    direction = SOUTH;
  }
  // pthread_mutex_lock(&mutex);

  // Ожидание, пока мост не освободится или не появится возможность двигаться в
  // нужном направлении
  while (bridge_count > 0 && bridge_direction != direction) {
    if (direction == NORTH) {
      pthread_cond_wait(&north_cond, &mutex);
    } else {
      pthread_cond_wait(&south_cond, &mutex);
    }
  }

  // Въезд на мост
  bridge_direction = direction;
  bridge_count++;
  printf("Car entered on the bridge going %s. Cars on bridge: %d\n",
         direction == NORTH ? "N" : "S", bridge_count);

  pthread_mutex_unlock(&mutex);

  // Имитация проезда через мост
  sleep(1);

  pthread_mutex_lock(&mutex);

  // Покидаем мост
  bridge_count--;
  printf("Car leaving the bridge going %s. Cars on bridge: %d\n",
         direction == NORTH ? "N" : "S", bridge_count);
  if (bridge_count == 0) {
    // Оповещаем ожидающие потоки в обоих направлениях
    pthread_cond_broadcast(&north_cond);
    pthread_cond_broadcast(&south_cond);
  }

  pthread_mutex_unlock(&mutex);
  return NULL;
}

int main() {
  int size = 100;
  pthread_t cars[size];
  srand(time(NULL)); // Инициализация генератора случайных чисел

  for (int i = 0; i < size; i++) {
    pthread_create(&cars[i], NULL, drive, NULL);
  }

  for (int i = 0; i < size; i++) {
    pthread_join(cars[i], NULL);
  }

  return 0;
}
