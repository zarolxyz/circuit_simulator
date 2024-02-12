#include "stdio.h"
#include "stdlib.h"
#include "connection.h"
#include "element.h"
#include "circuit.h"

circuit_t *create_circuit() {
    circuit_t *circuit = malloc(sizeof(circuit_t));
    circuit->connections = NULL;
    circuit->components = NULL;
    circuit->connections_count = 0;
    circuit->components_count = 0;
    return circuit;
}

void delete_circuit(circuit_t *circuit) {
    for (int i = 0; i < circuit->connections_count; i++) {
        delete_connection(circuit->connections[i]);
    }
    free(circuit->connections);
    for (int i = 0; i < circuit->components_count; i++) {
        delete_component(circuit->components[i]);
    }
    free(circuit->components);
    free(circuit);
}

void add_connection_to_circuit(circuit_t *circuit, connection_t *connection) {
    circuit->connections = realloc(circuit->connections, (circuit->connections_count + 1) * sizeof(connection_t *));
    circuit->connections[circuit->connections_count] = connection;
    circuit->connections_count++;
}

void add_component_to_circuit(circuit_t *circuit, component_t *component) {
    circuit->components = realloc(circuit->components, (circuit->components_count + 1) * sizeof(component_t *));
    circuit->components[circuit->components_count] = component;
    circuit->components_count++;
}

void add_component_and_connection_by_text_line(circuit_t *circuit, char *text_line) {
    char type_text[8];
    double value;
    int node_positive;
    int node_negative;
    sscanf(text_line, "%s %lf %d %d", type_text, &value, &node_negative, &node_positive);
    component_type_t type = get_component_type_from_character(type_text[0]);
    component_t *component = create_simple_component(type, value);
    connection_t *connection = create_connection(component->element1, node_positive, node_negative);
    add_component_to_circuit(circuit, component);
    add_connection_to_circuit(circuit, connection);
}

void init_circuit_elements(circuit_t *circuit) {
    for (int i = 0; i < circuit->components_count; i++) {
        init_component_element(circuit->components[i]);
    }
}

void update_circuit_elements_dynamic(circuit_t *circuit, double time) {
    for (int i = 0; i < circuit->components_count; i++) {
        update_dynamic_component_element(circuit->components[i], time);
    }
}

void update_circuit_elements_static(circuit_t *circuit) {
    for (int i = 0; i < circuit->components_count; i++) {
        update_static_component_element(circuit->components[i]);
    }
}
