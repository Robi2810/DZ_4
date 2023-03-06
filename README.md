# Операционные системы
## домашнее задание №4

### Вишняков Родион Сергеевич 
##### группа БПИ213

Задание: Разработать программу использующую для работы с текстовыми файлами системные вызовы. Программа на языке C должна прочитать, используя буфер, размер которого превышает читаемые файлы и записать прочитанный файл в файл с другим именем. Имена файлов для чтения и записи задавать с использованием аргументов командной строки.

Опционально +1. Использовать для работы с файлами буфер ограниченного размера, требующий циклического использования.

Опционально +1. Читать и переписывать не только текстовые, но и исполняемые файлы, включая скрипты, которые сохраняют режим доступа, обеспечивающий их запуск. При этом обычные текстовые файлы запускаться не должны.
## Код программы с соответствующими комментариями:


'#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const int size = 1024; // Задание размера буфера

int main(int argc, char **argv) {
    int fd;
    int fdout;
    char buffer[size];
    ssize_t read_bytes;
    ssize_t written_bytes;

    // Проверка количества переданных аргументов
    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        exit(1);
    }

    // Открытие файла для чтения
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Cannot open file\n");
        exit(1);
    }

    // Открытие файла для записи. 
    fdout = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fdout < 0) {
        fprintf(stderr, "Cannot create file\n");
        close(fd);
        exit(1);
    }
    // Чтение данных из файла и запись их в другой файл.
    while ((read_bytes = read(fd, buffer, size)) > 0) {
        written_bytes = write(fdout, buffer, read_bytes);
        if (written_bytes != read_bytes) {
            fprintf(stderr, "Cannot write\n");
            close(fd);
            close(fdout);
            exit(1);
        }
    }

    // Проверка на появление ошибки при чтении файла.
    if (read_bytes < 0) {
        fprintf(stderr, "Cannot read file\n");
        close(fd);
        close(fdout);
        exit(1);
    }

    // Проверка типа файла и устанавливка разрешения выходного файла 
    // на те же, что и у входного файла.
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
}'

## Компиляция и запуск исполняемого файла:

gcc -o copy_file copy_file.c

./copy_file input.txt output.txt

Файлы input.txt и output.txt приложены к отчёту.
