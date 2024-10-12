#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_NAME_LEN 50
#define MAX_DEV_LEN 50
#define MAX_PRODUCTS 100

typedef struct {
    char name[MAX_NAME_LEN];
    char developer[MAX_DEV_LEN];
    int version;
    int release_year;
    float price;
} SoftwareProduct;

// ������� ��� ������ � ����� ������
void write_to_file(SoftwareProduct* products, int count, const char* filename);
int read_from_file(SoftwareProduct* products, int max_count, const char* filename);
void add_product(SoftwareProduct* products, int* count, const SoftwareProduct* new_product);
SoftwareProduct* search_by_name(SoftwareProduct* products, int count, const char* name);
SoftwareProduct* search_by_release_year(SoftwareProduct* products, int count, int year);
void sort_by_name(SoftwareProduct* products, int count);
void update_product(SoftwareProduct* product, const char* new_name, const char* new_developer, int new_version, int new_year, float new_price);
void print_product(const SoftwareProduct* product, int count);
void print_all_products(const SoftwareProduct* products, int count);

// �������� ��������� � �����������
int main() {
    system("chcp 1251");

    SoftwareProduct products[MAX_PRODUCTS];
    int count = 0;
    char filename[100];
    int choice;

    while (1) {
        printf("\n����:\n");
        printf("1. ��������� ������ �� �����\n");
        printf("2. ������� ��� ������\n");
        printf("3. ����� ������ �� �����\n");
        printf("4. ����� ������ �� ���� �������\n");
        printf("5. �������� ����� ������\n");
        printf("6. �������� ������\n");
        printf("7. ����������� �� ��������\n");
        printf("8. ��������� ������ � ����\n");
        printf("9. �����\n");
        printf("�������� ��������: ");
        if (scanf("%d", &choice) != 1) {
            printf("������: ������������ ����. ���������� �����.\n");
            while (getchar() != '\n'); // ������� ������
            continue;
        }

        switch (choice) {
        case 1:
            printf("������� ��� ����� ��� ��������: ");
            scanf("%s", filename);
            count = read_from_file(products, MAX_PRODUCTS, filename);
            printf("%d ������� ��������� �� ����� '%s'.\n", count, filename);
            break;
        case 2:
            print_all_products(products, count);
            break;
        case 3: {
            char search_name[MAX_NAME_LEN];
            printf("������� ��� ��� ������: ");
            scanf("%s", search_name);
            SoftwareProduct* found = search_by_name(products, count, search_name);
            if (found != NULL) {
                print_product(found, count);
            }
            else {
                printf("������ �� �������.\n");
            }
            break;
        }
        case 4: {
            int search_year;
            printf("������� ��� ������� ��� ������: ");
            if (scanf("%d", &search_year) != 1) {
                printf("������: ������������ ���� ����.\n");
                while (getchar() != '\n'); // ������� ������
                continue;
            }
            SoftwareProduct* found = search_by_release_year(products, count, search_year);
            if (found != NULL) {
                print_product(found, count);
            }
            else {
                printf("������ �� �������.\n");
            }
            break;
        }
        case 5: {
            SoftwareProduct new_product;
            printf("������� �������� ��������: ");
            scanf("%s", new_product.name);
            printf("������� ������������: ");
            scanf("%s", new_product.developer);
            printf("������� ������: ");
            if (scanf("%d", &new_product.version) != 1) {
                printf("������: ������������ ���� ������.\n");
                while (getchar() != '\n'); // ������� ������
                continue;
            }
            printf("������� ��� �������: ");
            if (scanf("%d", &new_product.release_year) != 1) {
                printf("������: ������������ ���� ���� �������.\n");
                while (getchar() != '\n'); // ������� ������
                continue;
            }
            printf("������� ����: ");
            if (scanf("%f", &new_product.price) != 1) {
                printf("������: ������������ ���� ����.\n");
                while (getchar() != '\n'); // ������� ������
                continue;
            }
            add_product(products, &count, &new_product);
            break;
        }
        case 6: {
            char search_name[MAX_NAME_LEN];
            printf("������� ��� ������ ��� ����������: ");
            scanf("%s", search_name);
            SoftwareProduct* found = search_by_name(products, count, search_name);
            if (found != NULL) {
                printf("������� ����� �������� ��������: ");
                scanf("%s", found->name);
                printf("������� ������ ������������: ");
                scanf("%s", found->developer);
                printf("������� ����� ������: ");
                if (scanf("%d", &found->version) != 1) {
                    printf("������: ������������ ���� ������.\n");
                    while (getchar() != '\n'); // ������� ������
                    continue;
                }
                printf("������� ����� ��� �������: ");
                if (scanf("%d", &found->release_year) != 1) {
                    printf("������: ������������ ���� ���� �������.\n");
                    while (getchar() != '\n'); // ������� ������
                    continue;
                }
                printf("������� ����� ����: ");
                if (scanf("%f", &found->price) != 1) {
                    printf("������: ������������ ���� ����.\n");
                    while (getchar() != '\n'); // ������� ������
                    continue;
                }
                printf("������ ���������.\n");
            }
            else {
                printf("������ �� �������.\n");
            }
            break;
        }
        case 7:
            sort_by_name(products, count);       
            break;
        case 8:
            printf("������� ��� ����� ��� ����������: ");
            scanf("%s", filename);
            write_to_file(products, count, filename);
            printf("������ ��������� � ���� '%s'.\n", filename);
            break;
        case 9:
            printf("����� �� ���������.\n");
            return 0;
        default:
            printf("������: ������������ �����. ���������� �����.\n");
            break;
        }
    }

    return 0;
}

// ���������� �������

void write_to_file(const SoftwareProduct* products, int count, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("�� ������� ������� ���� ��� ������");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %d %d %.2f\n", products[i].name, products[i].developer,
            products[i].version, products[i].release_year, products[i].price);
    }

    fclose(file);
}

int read_from_file(SoftwareProduct* products, int max_count, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("�� ������� ������� ���� ��� ������");
        return 0;
    }

    int count = 0;
    while (count < max_count && !feof(file)) {
        SoftwareProduct product;
        if (fscanf(file, "%49s %49s %d %d %f",
            product.name, product.developer, &product.version,
            &product.release_year, &product.price) == 5) {
            products[count++] = product;
        }
        else {
            printf("������ ������ ������ �� ������.\n");
            break;
        }
    }

    fclose(file);

    return count;
}

void add_product(SoftwareProduct* products, int* count, const SoftwareProduct* new_product) {
    if (*count < MAX_PRODUCTS) {
        products[*count] = *new_product;
        (*count)++;
        printf("����� ������ ���������.\n");
    }
    else {
        printf("������: �������� ������������ ������ ���� ������.\n");
    }
}

SoftwareProduct* search_by_name(SoftwareProduct* products, int count, const char* name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(products[i].name, name) == 0) {
            return &products[i];
        }
    }
    return NULL;
}

SoftwareProduct* search_by_release_year(SoftwareProduct* products, int count, int year) {
    for (int i = 0; i < count; i++) {
        if (products[i].release_year == year) {
            return &products[i];
        }
    }
    return NULL;
}

int compare_by_name(const void* a, const void* b) {
    return strcmp(((SoftwareProduct*)a)->name, ((SoftwareProduct*)b)->name);
}

void sort_by_name(SoftwareProduct* products, int count) {
    if (count != 0) {
        qsort(products, count, sizeof(SoftwareProduct), compare_by_name);
        printf("������ ������������� �� ��������.\n");
    }
    else {
        printf("������ ��� ���������� �����������!\n");
    }
}

void print_product(const SoftwareProduct* product, int count) {
    if (count != 0) {
    printf("��������: %s, �����������: %s, ������: %d, ��� �������: %d, ����: %.2f\n",
        product->name, product->developer, product->version, product->release_year, product->price);
    }
    else {
        printf("������ ��� ���������� �����������!\n");
    }
}

void print_all_products(const SoftwareProduct* products, int count) {
    if (count == 0) {
        printf("���� ������ �����.\n");
    }
    else {
        for (int i = 0; i < count; i++) {
            print_product(&products[i], count);
        }
    }
}
