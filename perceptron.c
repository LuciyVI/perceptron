#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"

#define WEIGHTS 5
#define INPUTS 4
#define BIAS_INDEX (WEIGHTS - 1)
#define LR 0.3

static double w[WEIGHTS] = {0, 0, 0, 0, 1};

void distrib_weights(const bool *inputs, bool value, bool result) {
    double delta = 0.0;
    double error = value - result;

    for (int i = 0; i < INPUTS; i++) {
        delta = LR * error * inputs[i];
        w[i] += delta;
    }

    w[BIAS_INDEX] += LR * error;
}

double first_activate(const double net) {
    return net >= 0 ? 1 : 0;
}

double second_activate(const double net) {
    return 0.5 * ((net / (1 + fabs(net))) + 1);
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
        res += w[i] * inputs[i];
    }
    res += w[BIAS_INDEX];
    return res;
}

bool boolean_func(bool value, bool result, const bool *inputs) {
    if (value == result) {
        return true;
    } else {
        distrib_weights(inputs, value, result);
        return false;
    }
}

int run_generic(const bool *function_values, const bool *truth_table, double (*activate)(const double)) {
    bool inputs[INPUTS];
    int error = 0;
    for (int j = 0; j < 16; ++j) {
        for (int i = 0; i < INPUTS; ++i) {
            inputs[i] = truth_table[j * INPUTS + i];
        }
        bool predicted = activate(net(inputs, INPUTS)) >= 0.5;
        if (!boolean_func(function_values[j], predicted, inputs)) {
            ++error;
        }
    }
    return error;
}

void train_and_render(const char *filename, const bool *function_values, const bool *truth_table, double (*activate)(const double), ErrorSeries *series) {
    int error = run_generic(function_values, truth_table, activate);

    while (true) {
        saveIntegerValueToJsonFile(filename, "error", error, series);
        if (error == 0) {
            break;
        }
        error = run_generic(function_values, truth_table, activate);
    }

    renderGraph(series);
}

int main(int argc, char** argv) {
    srand(time(NULL));

    bool *pTable = output();
    bool *pFunc_ = _table();

    ErrorSeries *first_graph = createErrorSeries();
    train_and_render("data.json", pFunc_, pTable, first_activate, first_graph);

    _null_weights(true);

    ErrorSeries *second_graph = createErrorSeries();
    train_and_render("data2.json", pFunc_, pTable, second_activate, second_graph);

    free(pTable);
    free(pFunc_);
    destroyErrorSeries(first_graph);
    destroyErrorSeries(second_graph);

    return 0;
}
