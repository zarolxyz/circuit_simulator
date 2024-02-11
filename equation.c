#include <stdlib.h>
#include "equation.h"

double *create_equation(int xn_count) {
    return malloc(sizeof(double) * (xn_count + 1));
}

double **create_equations(int count) {
    double **equations = malloc(sizeof(double *) * count);
    for (int i = 0; i < count; i++) {
        equations[i] = create_equation(count);
    }
    return equations;
}

void delete_equation(double *equation) {
    free(equation);
}

void delete_equations(double **equations, int count) {
    for (int i = 0; i < count; i++)
        delete_equation(equations[i]);
    free(equations);
}

void copy_equation(double *destination, double *equation, int xn_count) {
    for (int i = 0; i < (xn_count + 1); i++) {
        destination[i] = equation[i];
    }
}

void erase_equation(double *target, int xn_count) {
    for (int i = 0; i < xn_count + 1; i++)
        target[i] = 0.0;
}

// 找到一个指定未知量系数不为0的方程索引
int find_equation_index_xn(int index_start, double **equations, int count, int n) {
    for (int i = index_start; i < count; i++) {
        if (!IS_ZERO(equations[i][n]))
            return i;
    }
    return -1;
}

void swap_equation(double **equations, int index1, int index2) {
    double *tmp = equations[index1];
    equations[index1] = equations[index2];
    equations[index2] = tmp;
}

// 消元
void elimination_xn(double *target, double *source, int xn_count, int n) {
    double ratio = -target[n] / source[n];
    for (int i = 0; i < xn_count + 1; i++) {
        if (i != n) {
            target[i] += ratio * source[i];
        }
    }
    target[n] = 0.0;
}

void elimination_equations(double **target, int count) {
    for (int i = 0; i < count; i++) {
        int index_found = find_equation_index_xn(i, target, count, i);
        if (index_found == -1) {
            target[i][i] = 1.0;
            continue;
        }
        swap_equation(target, i, index_found);
        for (int j = 0; j < count; j++) {
            if (j != i)
                elimination_xn(target[j], target[i], count, i);
        }
    }
}

void divide_equation_coefficient(double *target, int n, int xn_count) {
    for (int i = 0; i < xn_count + 1; i++) {
        if (i != n)
            target[i] /= target[n];
    }
    target[n] = 1.0;
}

void divide_equations_coefficient(double **equations, int count) {
    for (int i = 0; i < count; i++)
        divide_equation_coefficient(equations[i], i, count);
}

void solve_equations(double **target, int count) {
    elimination_equations(target, count);
    divide_equations_coefficient(target, count);
}
