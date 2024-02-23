#ifndef CIRCUIT_SIMULATOR_SIMULATOR_H
#define CIRCUIT_SIMULATOR_SIMULATOR_H

#include "component.h"
#include "connection.h"
#include "analyzer.h"

typedef struct {
    component_t **components;
    int components_count;
    connection_t **connections;
    int connections_count;
    analyzer_t *analyzer;
    double step_time;
} simulator_t;

simulator_t *create_simulator(component_t **components, int components_count, double step_time);

void delete_simulator(simulator_t *simulator);

void simulate_static(simulator_t *simulator);

void simulate_step(simulator_t *simulator);

#endif //CIRCUIT_SIMULATOR_SIMULATOR_H
