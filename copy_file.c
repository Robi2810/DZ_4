#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const int size = 1024;

int main(int argc, char **argv) {
    int fd;
    int fdout;
    char buffer[size];
    ssize_t read_bytes;
    ssize_t written_bytes;

    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Cannot open file\n");
        exit(1);
    }

    fdout = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fdout < 0) {
        fprintf(stderr, "Cannot create file\n");
        close(fd);
        exit(1);
    }
    while ((read_bytes = read(fd, buffer, size)) > 0) {
        written_bytes = write(fdout, buffer, read_bytes);
        if (written_bytes != read_bytes) {
            fprintf(stderr, "Cannot write\n");
            close(fd);
            close(fdout);
            exit(1);
        }
    }

    if (read_bytes < 0) {
        fprintf(stderr, "Cannot read file\n");
        close(fd);
        close(fdout);
        exit(1);
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        fprintf(stderr, "Cannot get file stat\n");
        close(fd);
        close(fdout);
        exit(1);
    }

    if (S_ISREG(file_stat.st_mode) || S_ISLNK(file_stat.st_mode)) {
        if (fchmod(fdout, file_stat.st_mode) == -1) {
            fprintf(stderr, "Cannot set file permissions\n");
            close(fd);
            close(fdout);
            exit(1);
        }
    }

    close(fd);
    close(fdout);
    return 0;
}
