#ifndef CIRCUIT_SIMULATOR_EQUATION_H
#define CIRCUIT_SIMULATOR_EQUATION_H

#define IS_ZERO(x) ((x) >= -1e-15 && (x) <= 1e-15)
#define NOT_NUMBER (0.0 / 0.0)

double **create_equations(int count);

void delete_equations(double **equations, int count);

void erase_equation(double *target, int xn_count);

void solve_equations(double **target, int count);

#endif //CIRCUIT_SIMULATOR_EQUATION_H
