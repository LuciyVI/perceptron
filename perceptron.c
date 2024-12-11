#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "cjson/cJSON.h"

#define WEIGHTS 5
#define LR 0.3

static double w[WEIGHTS] = {0, 0, 0, 0, 1};

void distrib_weights(const bool *inputs, bool value, bool result) {
    double delta = 0.0;
    double error = value - result;

    for (int i = 0; i < 4; i++) {
        delta = LR * error * inputs[i];
        w[i + 1] += delta;
    }
}

double first_activate(const double net) {
    return net >= 0 ? 1 : 0;
}

double second_activate(const double net) {
    return 0.5 * (net / (1 + fabs(net) + 1));
}

void _null_weights(bool flag) {
    if (flag) {
        memset(w, 0, sizeof(w));
    } else {
        printf("\nWeights don't change\n");
    }
}

bool* _table() {
    bool *result = (bool*)malloc(16 * sizeof(bool));
    for (int i = 0; i < 16; ++i) {
        bool x1 = (i >> 3) & 1;
        bool x2 = (i >> 2) & 1;
        bool x3 = (i >> 1) & 1;
        bool x4 = i & 1;
        result[i] = !x1 * x2 * (!x3 + x4);
    }
    return result;
}

bool* output() {
    bool *table = (bool*)malloc(16 * 4 * sizeof(bool));
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            table[i * 4 + j] = (i >> (3 - j)) & 1;
        }
    }
    return table;
}

double net(const bool *inputs, int size) {
    double res = 0.0;
    for (int i = 0; i < size; i++) {
        res += w[i + 1] * inputs[i];
    }
    res += w[4];
    return res;
}

bool boolean_func(bool result, const bool *inputs) {
    bool value = !inputs[0] * inputs[1] * (!inputs[2] + inputs[3]);
    if (value == result) {
        return true;
    } else {
        distrib_weights(inputs, value, result);
        return false;
    }
}

int run_generic(bool *pF, bool *pT, double (*activate)(const double)) {
    bool inputs[4];
    int error = 0;
    for (int j = 0; j < 16; ++j) {
        for (int i = 0; i < 4; ++i) {
            inputs[i] = pT[j * 4 + i];
        }
        if (!boolean_func(activate(net(inputs, 4)), inputs)) {
            ++error;
        }
    }
    return error;
}

int main(int argc, char** argv) {
    srand(time(NULL));

    bool *pTable = output();
    bool *pFunc_ = _table();

    cJSON *root = cJSON_CreateObject();
    int error = run_generic(pFunc_, pTable, first_activate);

    while (error > 0) {
        saveIntegerValueToJsonFile("data.json", "error", error, root);
        error = run_generic(pFunc_, pTable, first_activate);
    }
    renderGraph("data.json");

    _null_weights(true);

    cJSON *root_second = cJSON_CreateObject();
    error = run_generic(pFunc_, pTable, second_activate);

    while (error > 0) {
        saveIntegerValueToJsonFile("data2.json", "error", error, root_second);
        error = run_generic(pFunc_, pTable, second_activate);
    }
    renderGraph("data2.json");

    free(pTable);
    free(pFunc_);
    cJSON_Delete(root);
    cJSON_Delete(root_second);

    return 0;
}
