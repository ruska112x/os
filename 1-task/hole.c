#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

char buf1[] = "abcdefghijklmnop";
char buf2[] = "ABCDEFGHIJKLMNOP";

int main(void)
{
    int fd;

    /*
     * Default file access permissions for new files
     * S_IRUSR | read permisson for user
     * S_IWUSR | write pertmisson for user
     * S_IRGRP | read permisson for group
     * S_IROTH | read permisson for other`
     */

    if ((fd = creat("file.hole", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
    {
        perror("ошибка вызова creat");
        exit(1);
    }

    if (write(fd, buf1, 16) != 16)
    {
        perror("ошибка записи buf1");
        exit(1);
    }
    /* теперь текущая позиция = 16 */

    if (lseek(fd, 16368, SEEK_SET) == -1)
    {
        perror("ошибка вызова lseek");
        exit(1);
    }
    /* теперь текущая позиция = 16368 */

    if (write(fd, buf2, 16) != 16)
    {
        perror("ошибка записи buf2");
        exit(1);
    }
    /* теперь текущая позиция = 16384 */

    exit(0);
}