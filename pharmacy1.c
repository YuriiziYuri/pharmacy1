//������ 23
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
        printf("�� ������� �������� ����!\n");
        return;
    }

    int n;
    printf("������ ��� �� ������ ������? ");
    scanf("%d", &n);

    Medicine med;
    for (int i = 0; i < n; i++) {
        printf("\n=== ˳�� %d ===\n", i + 1);
        printf("���: ");
        scanf("%d", &med.code);
        printf("�����: ");
        scanf(" %[^\n]", med.name);
        printf("�����: ");
        scanf(" %[^\n]", med.composition);
        fwrite(&med, sizeof(Medicine), 1, fp);
    }

    fclose(fp);
    printf("���� ������ �������� �� ���������.\n");
}

void print_all(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("���� �� ��������!\n");
        return;
    }

    Medicine med;
    printf("\n=== ���� ����� ===\n");
    while (fread(&med, sizeof(Medicine), 1, fp)) {
        printf("���: %d | �����: %s | �����: %s\n", med.code, med.name, med.composition);
    }

    fclose(fp);
}

void search_by_code(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("���� �� ��������!\n");
        return;
    }

    int key;
    printf("������ ��� ��� ��� ������: ");
    scanf("%d", &key);

    Medicine med;
    int found = 0;
    while (fread(&med, sizeof(Medicine), 1, fp)) {
        if (med.code == key) {
            printf("��������: ���: %d | �����: %s | �����: %s\n", med.code, med.name, med.composition);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("˳�� � ����� %d �� ��������.\n", key);

    fclose(fp);
}

void append_to_file(const char *filename) {
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        printf("�� ������� ������� ����!\n");
        return;
    }

    Medicine med;
    printf("\n=== ��������� ������ ������ ===\n");
    printf("���: ");
    scanf("%d", &med.code);
    printf("�����: ");
    scanf(" %[^\n]", med.name);
    printf("�����: ");
    scanf(" %[^\n]", med.composition);

    fwrite(&med, sizeof(Medicine), 1, fp);
    fclose(fp);
    printf("����� ������ ������.\n");
}

void delete_by_code(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("���� �� ��������!\n");
        return;
    }

    FILE *temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("������� ��������� ����������� �����.\n");
        fclose(fp);
        return;
    }

    int key;
    printf("������ ��� ���, �� ������� ��������: ");
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
        printf("����� � ����� %d ��������.\n", key);
    else
        printf("˳�� � ����� ����� �� ��������.\n");
}

void menu(const char *filename) {
    int choice;
    do {
        printf("\n===== ���� =====\n");
        printf("1. �������� ���� � ������ ����������\n");
        printf("2. ������� ��� ����������\n");
        printf("3. ����� �� ����� ���\n");
        printf("4. ������ ����������\n");
        printf("5. �������� �����\n");
        printf("6. �����\n");
        printf("��� ����: ");
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
                printf("���������� ��������.\n");
                break;
            default:
                printf("������� ����. ��������� �����.\n");
        }
    } while (choice != 6);
}

int main() {
    setlocale(LC_CTYPE, "ukr");
    char filename[100] = "medicine.bin";

    menu(filename);
    return 0;
}
