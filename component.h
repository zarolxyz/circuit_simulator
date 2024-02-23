#ifndef CIRCUIT_SIMULATOR_COMPONENT_H
#define CIRCUIT_SIMULATOR_COMPONENT_H

#include "connection.h"

typedef enum {
    COMPONENT_UNKNOWN, VOLTAGE_SOURCE, CURRENT_SOURCE, RESISTOR, CAPACITOR, INDUCTOR, DIODE, TRANSISTOR
} component_type_t;

typedef struct {
    component_type_t type;
    connection_t *connection1;
    connection_t *connection2;
    double value1;
    double value2;
} component_t;

component_t *create_simple_component(component_type_t type, double value);

component_t *create_complex_component(component_type_t type, double value1, double value2);

void delete_component(component_t *component);

void delete_components(component_t **components, int components_count);

int calculate_components_maximum_connections(int components_count);

int find_components_connections(connection_t **connections, component_t **components, int components_count);

component_type_t get_component_type_from_character(char component_type);

void init_components(component_t **components, int components_count);

void update_static_components(component_t **components, int components_count);

void update_dynamic_components(component_t **components, int components_count, double time);

component_t *create_component_by_string(char *component_string);

void generate_component_state_string(char *state_string, component_t *component);

#endif //CIRCUIT_SIMULATOR_COMPONENT_H
