#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

void print_dir(const char *path) {
    DIR *dir;
    struct dirent *entry;

    printf("Содержимое каталога: %s\n", path);

    dir = opendir(path);
    if (dir == NULL) {
        perror("Ошибка открытия каталога");
        return;
    }

    errno = 0;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    if (errno != 0) {
        perror("Ошибка чтения каталога");
        closedir(dir);
        return;
    }

    if (closedir(dir) != 0) {
        perror("Ошибка закрытия каталога");
        return;
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <каталог>\n", argv[0]);
        return 1;
    }

    print_dir(".");
    print_dir(argv[1]);

    return 0;
}
