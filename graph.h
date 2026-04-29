#ifndef GRAPH_H
#define GRAPH_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    size_t size;
    size_t capacity;
} ErrorSeries;

ErrorSeries *createErrorSeries(void) {
    return (ErrorSeries *)calloc(1, sizeof(ErrorSeries));
}

void destroyErrorSeries(ErrorSeries *series) {
    if (!series) {
        return;
    }

    free(series->values);
    free(series);
}

int findMax(const ErrorSeries *series) {
    if (!series || series->size == 0) {
        return 0;
    }

    int max = INT_MIN;
    for (size_t i = 0; i < series->size; ++i) {
        if (series->values[i] > max) {
            max = series->values[i];
        }
    }
    return max;
}

bool ensureCapacity(ErrorSeries *series) {
    if (series->size < series->capacity) {
        return true;
    }

    size_t new_capacity = series->capacity == 0 ? 8 : series->capacity * 2;
    int *new_values = (int *)realloc(series->values, new_capacity * sizeof(int));
    if (!new_values) {
        fprintf(stderr, "Ошибка выделения памяти для графика\n");
        return false;
    }

    series->values = new_values;
    series->capacity = new_capacity;
    return true;
}

bool writeSeriesToJsonFile(const char *filename, const char *key, const ErrorSeries *series) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Ошибка записи в файл");
        return false;
    }

    fprintf(file, "{\n  \"%s\": [", key);
    for (size_t i = 0; i < series->size; ++i) {
        fprintf(file, "%s%d", i == 0 ? "" : ", ", series->values[i]);
    }
    fprintf(file, "]\n}\n");
    fclose(file);

    return true;
}

void saveIntegerValueToJsonFile(const char *filename, const char *key, int value, ErrorSeries *series) {
    if (!series) {
        fprintf(stderr, "Ошибка: серия значений не инициализирована\n");
        return;
    }

    if (!ensureCapacity(series)) {
        return;
    }

    series->values[series->size++] = value;
    writeSeriesToJsonFile(filename, key, series);
}

void renderGraph(const ErrorSeries *series) {
    if (!series || series->size == 0) {
        fprintf(stderr, "Ошибка: нет данных для графика\n");
        return;
    }

    int max = findMax(series);

    printf("\n \033[1;35mГрафик (error)\033[0m\n");
    for (int y = max; y >= 0; --y) {
        printf("%2d | ", y);
        for (size_t x = 0; x < series->size; ++x) {
            if (series->values[x] == y) {
                printf("\033[1;31m*\033[0m ");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }

    printf("    ");
    for (size_t x = 0; x < series->size; ++x) {
        printf("---");
    }
    printf("\n     ");
    for (size_t x = 0; x < series->size; ++x) {
        printf("%2zu ", x);
    }
    printf("\n");
}

#endif
