#include <stdio.h>
#include <GL/glut.h>
#include "cjson/cJSON.h"


void saveIntegerValueToJsonFile(const char* filename, const char* key, int value , cJSON *pointer_to_json) {
    
    
    // Чтение существующего JSON файла
    FILE *file = fopen("data.json", "r");
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
        FILE *updatedFile = fopen("data.json", "w");
        if (updatedFile != NULL) {
            fprintf(updatedFile, "%s", updatedJsonString);
            
        }
        
        free(updatedJsonString);
    fclose(updatedFile);
    }

// Функция отрисовки графика
void renderGraph() {

    
    // Настройка параметров графика
    glColor3f(1.0, 0.0, 0.0); // Установка цвета (красный)
    glLineWidth(10.0); // Установка толщины линии
FILE *data = fopen("data.json", "r");
        if (data == NULL) {
            printf("Ошибка открытия файла\n");
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
    cJSON *value = cJSON_GetObjectItem(root, "error");
    
    int x = 1;
    
    // Отрисовка графика
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-40.0f, 40.0f, -40.0f, 40.0f, -1.0f, 1.0f);
            
    glBegin(GL_LINE_STRIP); // Начало отрисовки линии
    while (value != NULL) {
            
            printf("x=%d\n",value->valueint);
            glVertex2i(x,value->valueint);
            
            value = value->next;
            
            printf("y=%d\n",x);
        x++;
            
        }
    glEnd(); // Завершение отрисовки линии
    

    glFlush(); // Принудительная отрисовка
}
