//варіант 23
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    int code;
    char name[50];
    char composition[100];
} Medicine;

void create_file(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Не вдалося створити файл!\n");
        return;
    }

    int n;
    printf("Скільки ліків ви хочете внести? ");
    scanf("%d", &n);

    Medicine med;
    for (int i = 0; i < n; i++) {
        printf("\n=== Ліки %d ===\n", i + 1);
        printf("Код: ");
        scanf("%d", &med.code);
        printf("Назва: ");
        scanf(" %[^\n]", med.name);
        printf("Склад: ");
        scanf(" %[^\n]", med.composition);
        fwrite(&med, sizeof(Medicine), 1, fp);
    }

    fclose(fp);
    printf("Файл успішно створено та заповнено.\n");
}

void print_all(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Файл не знайдено!\n");
        return;
    }

    Medicine med;
    printf("\n=== Вміст файлу ===\n");
    while (fread(&med, sizeof(Medicine), 1, fp)) {
        printf("Код: %d | Назва: %s | Склад: %s\n", med.code, med.name, med.composition);
    }

    fclose(fp);
}

void search_by_code(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Файл не знайдено!\n");
        return;
    }

    int key;
    printf("Введіть код ліків для пошуку: ");
    scanf("%d", &key);

    Medicine med;
    int found = 0;
    while (fread(&med, sizeof(Medicine), 1, fp)) {
        if (med.code == key) {
            printf("Знайдено: Код: %d | Назва: %s | Склад: %s\n", med.code, med.name, med.composition);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Ліки з кодом %d не знайдено.\n", key);

    fclose(fp);
}

void append_to_file(const char *filename) {
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        printf("Не вдалося відкрити файл!\n");
        return;
    }

    Medicine med;
    printf("\n=== Додавання нового запису ===\n");
    printf("Код: ");
    scanf("%d", &med.code);
    printf("Назва: ");
    scanf(" %[^\n]", med.name);
    printf("Склад: ");
    scanf(" %[^\n]", med.composition);

    fwrite(&med, sizeof(Medicine), 1, fp);
    fclose(fp);
    printf("Запис успішно додано.\n");
}

void delete_by_code(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Файл не знайдено!\n");
        return;
    }

    FILE *temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Помилка створення тимчасового файлу.\n");
        fclose(fp);
        return;
    }

    int key;
    printf("Введіть код ліків, які потрібно видалити: ");
    scanf("%d", &key);

    Medicine med;
    int deleted = 0;
    while (fread(&med, sizeof(Medicine), 1, fp)) {
        if (med.code == key) {
            deleted = 1;
            continue;
        }
        fwrite(&med, sizeof(Medicine), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(filename);
    rename("temp.bin", filename);

    if (deleted)
        printf("Запис з кодом %d видалено.\n", key);
    else
        printf("Ліки з таким кодом не знайдено.\n");
}

void menu(const char *filename) {
    int choice;
    do {
        printf("\n===== МЕНЮ =====\n");
        printf("1. Створити файл і ввести інформацію\n");
        printf("2. Вивести всю інформацію\n");
        printf("3. Пошук за кодом ліків\n");
        printf("4. Додати інформацію\n");
        printf("5. Видалити запис\n");
        printf("6. Вийти\n");
        printf("Ваш вибір: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_file(filename);
                break;
            case 2:
                print_all(filename);
                break;
            case 3:
                search_by_code(filename);
                break;
            case 4:
                append_to_file(filename);
                break;
            case 5:
                delete_by_code(filename);
                break;
            case 6:
                printf("Завершення програми.\n");
                break;
            default:
                printf("Невірний вибір. Спробуйте знову.\n");
        }
    } while (choice != 6);
}

int main() {
    setlocale(LC_CTYPE, "ukr");
    char filename[100] = "medicine.bin";

    menu(filename);
    return 0;
}
