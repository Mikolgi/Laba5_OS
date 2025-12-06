#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "w");
    if (f == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    printf("Вводите текст. Для выхода нажмите Ctrl+F, затем Enter.\n");

    int ch;
    while ((ch = getc(stdin)) != EOF) {
        // Ctrl+F имеет код 6
        if (ch == 6) {
            printf("Выход.\n");
            break;
        }

        // Пишем символ в файл
        if (fputc(ch, f) == EOF) {
            perror("Ошибка записи в файл");
            fclose(f);
            return 1;
        }
    }

    if (ferror(stdin)) {
        fprintf(stderr, "Ошибка чтения с клавиатуры\n");
        fclose(f);
        return 1;
    }

    if (fclose(f) == EOF) {
        perror("Ошибка закрытия файла");
        return 1;
    }

    return 0;
}
