#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Вы не ввели длину буфера!\n");
        return 1;
    }

    int bufferSize;
    if (sscanf(argv[1], "%d", &bufferSize) == 0 || bufferSize < 0) {
        printf("Введена неправильная длина буфера!\n");
        return 1;
    }

    char *buffer = (char *) malloc(bufferSize);
    if (buffer == NULL) {
        printf("Не удалось выделить память для буффера\n");
        return 1;
    }

    int cycles = 0;
    int bytes_num;
    while ((bytes_num = read(STDIN_FILENO, buffer, bufferSize)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_num);
        cycles++;
    }

    fprintf(stderr, "Всего циклов: %d\n", cycles);

    return 0;
}
