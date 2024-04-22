#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Invalid params count\n");
    return 1;
  }

  int src_fd = open(argv[1], O_RDONLY);
  if (src_fd == -1) {
    fprintf(stderr, "Error opening \"%s\" file\n", argv[1]);
    return 1;
  }

  int dest_fd = creat(argv[2], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (dest_fd == -1) {
    fprintf(stderr, "Error opening \"%s\" file\n", argv[2]);
    return 1;
  }

  char buf[BUF_SIZE];
  int bytes;

  while ((bytes = read(src_fd, buf, sizeof(buf))) > 0) {
    for (int i = 0; i < bytes; ++i) {
      if (buf[i] != 0) {
        if (write(dest_fd, &buf[i], 1) != 1) {
          fprintf(stderr, "Error writing to \"%s\" file\n", argv[2]);
          return 1;
        }
      }
    }
  }

  if (bytes == -1) {
    fprintf(stderr, "Error reading \"%s\"file\n", argv[1]);
    return 1;
  }

  fprintf(stdout, "File copied successfully\n");
  return 0;
}
