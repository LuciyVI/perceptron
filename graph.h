#include <stdio.h>
#include <GL/glut.h>
#include "cjson/cJSON.h"


void saveIntegerValueToJsonFile(const char* filename, const char* key, int value , cJSON *pointer_to_json) {
    
    
    // Чтение существующего JSON файла
    FILE *file = fopen(filename, "r");
    cJSON *existingValue = cJSON_GetObjectItem(pointer_to_json, key);
        if (existingValue != NULL && cJSON_IsArray(existingValue)) {
    // Если запись по ключу уже существует, добавляем новое значение в массив
    cJSON_AddItemToArray(pointer_to_json, cJSON_CreateNumber(value));
} else {
    // Если записи по ключу нет, создаем новую запись
    cJSON_AddItemToObject(pointer_to_json, key, cJSON_CreateNumber(value));
}
        
        char *updatedJsonString = cJSON_Print(pointer_to_json);

        
        // Запись обновленных данных в файл
        FILE *updatedFile = fopen(filename, "w");
        if (updatedFile != NULL) {
            fprintf(updatedFile, "%s", updatedJsonString);
            
        }
        
        free(updatedJsonString);
    fclose(updatedFile);
    }

// Функция отрисовки графика
void renderGraph(const char* filename) {

FILE *data = fopen(filename, "r");
        if (data == NULL) {
            perror("Ошибка открытия файла\n");
        }

    fseek(data, 0, SEEK_END);
    long fileSize = ftell(data);
    fseek(data, 0, SEEK_SET);
    char *jsonData = (char *)malloc(fileSize + 1);
    fread(jsonData, 1, fileSize, data);
    fclose(data);
    jsonData[fileSize] = '\0';

    cJSON *root = cJSON_Parse(jsonData);
    free(jsonData);
    
    int x,y=0;
    printf("\n \n  \033[1;35mk(error)\033[0m \n");
    for (x = 20;x!=-1 ;--x) 
    { 
    cJSON *value = cJSON_GetObjectItem(root, "error");
        for (y = 0; y <= 25 ; ++y) 
        { 
            if (x == value->valueint) 
                printf("\033[1;31m*\033[0m"); 
            else if (y == 0 && y == 0) 
                printf("\033[1;35m%d\033[0m\033[1;32m|\033[0m",y+x); 
            else if (y == 0) 
                printf(" "); 
            else if (x==0 && y==25)
            printf("\033[1;37m>\033[0m");
            else if (x== 0) 
                printf("\033[1;32m---\033[0m"); 
            else 
                printf("\033[1;37m \033[0m"); 
        value = value->next;
        } 
        printf("\n"); 
    } 
    printf("\t\t\t  \033[1;35mОсь X (эпохи)\033[0m");
}
