#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "cjson/cJSON.h"

// Функция для нахождения максимального значения в JSON массиве
int findMax(cJSON *array) {
    int max = INT_MIN; // Начальное значение
    if (!cJSON_IsArray(array)) return max; // Проверка, что это массив

    cJSON *current = NULL;
    cJSON_ArrayForEach(current, array) {
        if (cJSON_IsNumber(current) && current->valueint > max) {
            max = current->valueint;
        }
    }
    return max;
}

// Сохранение значения в JSON файл
void saveIntegerValueToJsonFile(const char *filename, const char *key, int value) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла для чтения");
        return;
    }

    // Чтение содержимого файла
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *jsonData = (char *)malloc(fileSize + 1);
    fread(jsonData, 1, fileSize, file);
    fclose(file);
    jsonData[fileSize] = '\0';

    // Парсинг JSON данных
    cJSON *root = cJSON_Parse(jsonData);
    free(jsonData);
    if (!root) {
        fprintf(stderr, "Ошибка парсинга JSON\n");
        return;
    }

    cJSON *existingArray = cJSON_GetObjectItem(root, key);
    if (existingArray && cJSON_IsArray(existingArray)) {
        cJSON_AddItemToArray(existingArray, cJSON_CreateNumber(value));
    } else {
        cJSON *newArray = cJSON_CreateArray();
        cJSON_AddItemToArray(newArray, cJSON_CreateNumber(value));
        cJSON_AddItemToObject(root, key, newArray);
    }

    char *updatedJsonString = cJSON_Print(root);
    file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s", updatedJsonString);
        fclose(file);
    } else {
        perror("Ошибка записи в файл");
    }

    cJSON_Delete(root);
    free(updatedJsonString);
}

// Функция отрисовки графика
void renderGraph(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    // Чтение содержимого файла
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *jsonData = (char *)malloc(fileSize + 1);
    fread(jsonData, 1, fileSize, file);
    fclose(file);
    jsonData[fileSize] = '\0';

    cJSON *root = cJSON_Parse(jsonData);
    free(jsonData);
    if (!root) {
        fprintf(stderr, "Ошибка парсинга JSON\n");
        return;
    }

    cJSON *array = cJSON_GetObjectItem(root, "error");
    if (!cJSON_IsArray(array)) {
        fprintf(stderr, "Ошибка: ключ 'error' не является массивом\n");
        cJSON_Delete(root);
        return;
    }

    int max = findMax(array);
    int size = cJSON_GetArraySize(array);

    printf("\n \033[1;35mГрафик (error)\033[0m\n");
    for (int y = max; y >= 0; --y) {
        printf("%2d | ", y);
        for (int x = 0; x < size; ++x) {
            cJSON *item = cJSON_GetArrayItem(array, x);
            if (item && cJSON_IsNumber(item) && item->valueint == y) {
                printf("\033[1;31m*\033[0m ");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }

    printf("    ");
    for (int x = 0; x < size; ++x) {
        printf("---");
    }
    printf("\n     ");
    for (int x = 0; x < size; ++x) {
        printf("%2d ", x);
    }
    printf("\n");

    cJSON_Delete(root);
}
