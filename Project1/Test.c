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

// Функции для работы с базой данных
void write_to_file(SoftwareProduct* products, int count, const char* filename);
int read_from_file(SoftwareProduct* products, int max_count, const char* filename);
void add_product(SoftwareProduct* products, int* count, const SoftwareProduct* new_product);
SoftwareProduct* search_by_name(SoftwareProduct* products, int count, const char* name);
SoftwareProduct* search_by_release_year(SoftwareProduct* products, int count, int year);
void sort_by_name(SoftwareProduct* products, int count);
void update_product(SoftwareProduct* product, const char* new_name, const char* new_developer, int new_version, int new_year, float new_price);
void print_product(const SoftwareProduct* product, int count);
void print_all_products(const SoftwareProduct* products, int count);

// Основная программа с интерфейсом
int main() {
    system("chcp 1251");

    SoftwareProduct products[MAX_PRODUCTS];
    int count = 0;
    char filename[100];
    int choice;

    while (1) {
        printf("\nМеню:\n");
        printf("1. Загрузить данные из файла\n");
        printf("2. Вывести все данные\n");
        printf("3. Найти запись по имени\n");
        printf("4. Найти запись по году выпуска\n");
        printf("5. Добавить новую запись\n");
        printf("6. Обновить запись\n");
        printf("7. Сортировать по названию\n");
        printf("8. Сохранить данные в файл\n");
        printf("9. Выйти\n");
        printf("Выберите операцию: ");
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: некорректный ввод. Попробуйте снова.\n");
            while (getchar() != '\n'); // очистка буфера
            continue;
        }

        switch (choice) {
        case 1:
            printf("Введите имя файла для загрузки: ");
            scanf("%s", filename);
            count = read_from_file(products, MAX_PRODUCTS, filename);
            printf("%d записей загружено из файла '%s'.\n", count, filename);
            break;
        case 2:
            print_all_products(products, count);
            break;
        case 3: {
            char search_name[MAX_NAME_LEN];
            printf("Введите имя для поиска: ");
            scanf("%s", search_name);
            SoftwareProduct* found = search_by_name(products, count, search_name);
            if (found != NULL) {
                print_product(found, count);
            }
            else {
                printf("Запись не найдена.\n");
            }
            break;
        }
        case 4: {
            int search_year;
            printf("Введите год выпуска для поиска: ");
            if (scanf("%d", &search_year) != 1) {
                printf("Ошибка: некорректный ввод года.\n");
                while (getchar() != '\n'); // очистка буфера
                continue;
            }
            SoftwareProduct* found = search_by_release_year(products, count, search_year);
            if (found != NULL) {
                print_product(found, count);
            }
            else {
                printf("Запись не найдена.\n");
            }
            break;
        }
        case 5: {
            SoftwareProduct new_product;
            printf("Введите название продукта: ");
            scanf("%s", new_product.name);
            printf("Введите разработчика: ");
            scanf("%s", new_product.developer);
            printf("Введите версию: ");
            if (scanf("%d", &new_product.version) != 1) {
                printf("Ошибка: некорректный ввод версии.\n");
                while (getchar() != '\n'); // очистка буфера
                continue;
            }
            printf("Введите год выпуска: ");
            if (scanf("%d", &new_product.release_year) != 1) {
                printf("Ошибка: некорректный ввод года выпуска.\n");
                while (getchar() != '\n'); // очистка буфера
                continue;
            }
            printf("Введите цену: ");
            if (scanf("%f", &new_product.price) != 1) {
                printf("Ошибка: некорректный ввод цены.\n");
                while (getchar() != '\n'); // очистка буфера
                continue;
            }
            add_product(products, &count, &new_product);
            break;
        }
        case 6: {
            char search_name[MAX_NAME_LEN];
            printf("Введите имя записи для обновления: ");
            scanf("%s", search_name);
            SoftwareProduct* found = search_by_name(products, count, search_name);
            if (found != NULL) {
                printf("Введите новое название продукта: ");
                scanf("%s", found->name);
                printf("Введите нового разработчика: ");
                scanf("%s", found->developer);
                printf("Введите новую версию: ");
                if (scanf("%d", &found->version) != 1) {
                    printf("Ошибка: некорректный ввод версии.\n");
                    while (getchar() != '\n'); // очистка буфера
                    continue;
                }
                printf("Введите новый год выпуска: ");
                if (scanf("%d", &found->release_year) != 1) {
                    printf("Ошибка: некорректный ввод года выпуска.\n");
                    while (getchar() != '\n'); // очистка буфера
                    continue;
                }
                printf("Введите новую цену: ");
                if (scanf("%f", &found->price) != 1) {
                    printf("Ошибка: некорректный ввод цены.\n");
                    while (getchar() != '\n'); // очистка буфера
                    continue;
                }
                printf("Запись обновлена.\n");
            }
            else {
                printf("Запись не найдена.\n");
            }
            break;
        }
        case 7:
            sort_by_name(products, count);       
            break;
        case 8:
            printf("Введите имя файла для сохранения: ");
            scanf("%s", filename);
            write_to_file(products, count, filename);
            printf("Данные сохранены в файл '%s'.\n", filename);
            break;
        case 9:
            printf("Выход из программы.\n");
            return 0;
        default:
            printf("Ошибка: некорректный выбор. Попробуйте снова.\n");
            break;
        }
    }

    return 0;
}

// Реализация функций

void write_to_file(const SoftwareProduct* products, int count, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Не удалось открыть файл для записи");
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
        perror("Не удалось открыть файл для чтения");
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
            printf("Ошибка чтения данных из строки.\n");
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
        printf("Новая запись добавлена.\n");
    }
    else {
        printf("Ошибка: превышен максимальный размер базы данных.\n");
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
        printf("Записи отсортированы по названию.\n");
    }
    else {
        printf("Записи для сортировки отсутствуют!\n");
    }
}

void print_product(const SoftwareProduct* product, int count) {
    if (count != 0) {
    printf("Название: %s, Разработчик: %s, Версия: %d, Год выпуска: %d, Цена: %.2f\n",
        product->name, product->developer, product->version, product->release_year, product->price);
    }
    else {
        printf("Записи для сортировки отсутствуют!\n");
    }
}

void print_all_products(const SoftwareProduct* products, int count) {
    if (count == 0) {
        printf("База данных пуста.\n");
    }
    else {
        for (int i = 0; i < count; i++) {
            print_product(&products[i], count);
        }
    }
}
