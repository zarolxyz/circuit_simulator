#ifndef CIRCUIT_SIMULATOR_SIMULATOR_H
#define CIRCUIT_SIMULATOR_SIMULATOR_H

#include "circuit.h"
#include "analyzer.h"

typedef struct {
    circuit_t *circuit;
    analyzer_t *analyzer;
    double unit_time;
} simulator_t;

simulator_t *create_simulator(circuit_t *circuit, double unit_time);

void delete_simulator(simulator_t *simulator);

int simulate_static(simulator_t *simulator);

int simulate_step(simulator_t *simulator);

#endif //CIRCUIT_SIMULATOR_SIMULATOR_H
