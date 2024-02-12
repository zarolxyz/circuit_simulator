#ifndef CIRCUIT_SIMULATOR_CIRCUIT_H
#define CIRCUIT_SIMULATOR_CIRCUIT_H

#include "element.h"
#include "connection.h"

typedef struct {
    connection_t **connections;
    int connections_count;
    component_t **components;
    int components_count;
} circuit_t;

circuit_t *create_circuit();

void delete_circuit(circuit_t *circuit);

void add_component_and_connection_by_text_line(circuit_t *circuit, char *text_line);

void init_circuit_elements(circuit_t *circuit);

void update_circuit_elements_dynamic(circuit_t *circuit, double time);

void update_circuit_elements_static(circuit_t *circuit);

#endif //CIRCUIT_SIMULATOR_CIRCUIT_H
