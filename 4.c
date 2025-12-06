#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <исходный_файл> <целевой_файл>\n", argv[0]);
        return 1;
    }

    const char *src_name = argv[1];
    const char *dst_name = argv[2];

    struct stat st;
    if (stat(src_name, &st) != 0) {
        perror("Ошибка получения информации об исходном файле (stat)");
        return 1;
    }

    FILE *src = fopen(src_name, "r");
    if (src == NULL) {
        perror("Ошибка открытия исходного файла");
        return 1;
    }

    FILE *dst = fopen(dst_name, "w");
    if (dst == NULL) {
        perror("Ошибка открытия целевого файла");
        fclose(src);
        return 1;
    }

    int ch;
  
    while ((ch = fgetc(src)) != EOF) {
        if (fputc(ch, dst) == EOF) {
            perror("Ошибка записи в целевой файл");
            fclose(src);
            fclose(dst);
            return 1;
        }
    }

    if (ferror(src)) {
        fprintf(stderr, "Ошибка чтения исходного файла\n");
        fclose(src);
        fclose(dst);
        return 1;
    }

    if (fclose(src) == EOF) {
        perror("Ошибка закрытия исходного файла");
        fclose(dst);
        return 1;
    }

    if (fclose(dst) == EOF) {
        perror("Ошибка закрытия целевого файла");
        return 1;
    }

    if (chmod(dst_name, st.st_mode & 0777) != 0) {
        perror("Ошибка установки прав доступа целевого файла (chmod)");
        return 1;
    }

    return 0;
}
