#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <имя_файла> <N>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int N = atoi(argv[2]); 
    if (N < 0) {
        N = 0; 
    }

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    int ch;
    int lines_printed = 0;

    if (N == 0) {
        while ((ch = fgetc(f)) != EOF) {
            if (fputc(ch, stdout) == EOF) {
                perror("Ошибка записи на экран");
                fclose(f);
                return 1;
            }
        }

        if (ferror(f)) {
            fprintf(stderr, "Ошибка чтения файла\n");
            fclose(f);
            return 1;
        }

        if (fclose(f) == EOF) {
            perror("Ошибка закрытия файла");
            return 1;
        }

        return 0;
    }

    while ((ch = fgetc(f)) != EOF) {
        if (fputc(ch, stdout) == EOF) {
            perror("Ошибка записи на экран");
            fclose(f);
            return 1;
        }

        if (ch == '\n') {
            lines_printed++;

            if (lines_printed % N == 0) {
                fputs("-- Нажмите любую клавишу для продолжения --", stdout);
                fflush(stdout);
                int c = getc(stdin);
                (void)c;            

                fputc('\n', stdout);
            }
        }
    }

    if (ferror(f)) {
        fprintf(stderr, "Ошибка чтения файла\n");
        fclose(f);
        return 1;
    }

    if (fclose(f) == EOF) {
        perror("Ошибка закрытия файла");
        return 1;
    }

    return 0;
}
