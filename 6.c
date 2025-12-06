#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

int files_equal(const char *p1, const char *p2) {
    FILE *f1 = fopen(p1, "rb");
    FILE *f2 = fopen(p2, "rb");
    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 0;
    }

    int c1, c2;
    do {
        c1 = fgetc(f1);
        c2 = fgetc(f2);
        if (c1 != c2) {
            fclose(f1);
            fclose(f2);
            return 0;
        }
    } while (c1 != EOF && c2 != EOF);

    fclose(f1);
    fclose(f2);
    return 1;
}

void print_info(const char *path, FILE *out) {
    struct stat st;
    if (stat(path, &st) != 0) return;

    char timebuf[64];
    struct tm *tm = localtime(&st.st_ctime);
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm);

    printf("Файл: %s\nРазмер: %lld\nДата: %s\nПрава: %o\ninode: %lu\n\n",
           path, (long long)st.st_size, timebuf,
           st.st_mode & 0777, (unsigned long)st.st_ino);

    fprintf(out, "Файл: %s\nРазмер: %lld\nДата: %s\nПрава: %o\ninode: %lu\n\n",
            path, (long long)st.st_size, timebuf,
            st.st_mode & 0777, (unsigned long)st.st_ino);
}

void scan_dir2(const char *file1, off_t size1, const char *dir2, FILE *out) {
    DIR *d = opendir(dir2);
    if (!d) return;

    struct dirent *e;
    char path2[PATH_MAX];

    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name,".") || !strcmp(e->d_name,"..")) continue;

        snprintf(path2, sizeof(path2), "%s/%s", dir2, e->d_name);

        struct stat st2;
        if (stat(path2, &st2) != 0) continue;

        if (S_ISDIR(st2.st_mode)) {
            scan_dir2(file1, size1, path2, out);
        } else if (S_ISREG(st2.st_mode)) {
            if (st2.st_size == size1 && files_equal(file1, path2)) {
                printf("Совпадающие файлы:\n");
                fprintf(out, "Совпадающие файлы:\n");
                print_info(file1, out);
                print_info(path2, out);
            }
        }
    }

    closedir(d);
}

void scan_dir1(const char *dir1, const char *dir2, FILE *out) {
    DIR *d = opendir(dir1);
    if (!d) return;

    struct dirent *e;
    char path1[PATH_MAX];

    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name,".") || !strcmp(e->d_name,"..")) continue;

        snprintf(path1, sizeof(path1), "%s/%s", dir1, e->d_name);

        struct stat st1;
        if (stat(path1, &st1) != 0) continue;

        if (S_ISDIR(st1.st_mode)) {
            scan_dir1(path1, dir2, out);
        } else if (S_ISREG(st1.st_mode)) {
            scan_dir2(path1, st1.st_size, dir2, out);
        }
    }

    closedir(d);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Использование: %s <каталог1> <каталог2> <файл_вывода>\n", argv[0]);
        return 1;
    }

    FILE *out = fopen(argv[3], "w");
    if (!out) {
        perror("Ошибка открытия файла вывода");
        return 1;
    }

    scan_dir1(argv[1], argv[2], out);

    fclose(out);
    return 0;
}
