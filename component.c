#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "element.h"
#include "component.h"

component_t *create_simple_component(component_type_t type, double value) {
    component_t *component = malloc(sizeof(component_t));
    component->type = type;
    component->connection1 = create_connection(create_element());
    component->value1 = value;
    component->connection2 = NULL;
    return component;
}

component_t *create_complex_component(component_type_t type, double value1, double value2) {
    component_t *component = malloc(sizeof(component_t));
    component->type = type;
    component->connection1 = create_connection(create_element());
    component->connection2 = create_connection(create_element());
    component->value1 = value1;
    component->value2 = value2;
    return component;
}

int is_complex_component_type(component_type_t type) {
    switch (type) {
        case TRANSISTOR:
            return 1;
        default:
            break;
    }
    return 0;
}

void delete_component(component_t *component) {
    delete_element(component->connection1->element);
    delete_connection(component->connection1);
    if (component->connection2 != NULL) {
        delete_element(component->connection2->element);
        delete_connection(component->connection2);
    }
    free(component);
}

void delete_components(component_t **components, int components_count) {
    for (int i = 0; i < components_count; i++)
        delete_component(components[i]);
}

int calculate_components_maximum_connections(int components_count) {
    return components_count * 2;
}

int find_components_connections(connection_t **connections, component_t **components, int components_count) {
    int connections_count = 0;
    for (int i = 0; i < components_count; i++) {
        if (components[i]->connection1 != NULL) {
            connections[connections_count] = components[i]->connection1;
            connections_count++;
        }
        if (components[i]->connection2 != NULL) {
            connections[connections_count] = components[i]->connection2;
            connections_count++;
        }
    }
    return connections_count;
}

component_type_t get_component_type_from_character(char component_type) {
    switch (component_type) {
        case 'U':
            return VOLTAGE_SOURCE;
        case 'I':
            return CURRENT_SOURCE;
        case 'R':
            return RESISTOR;
        case 'C':
            return CAPACITOR;
        case 'L':
            return INDUCTOR;
        case 'D':
            return DIODE;
        case 'T':
            return TRANSISTOR;
        default:
            break;
    }
    return COMPONENT_UNKNOWN;
}

void init_component(component_t *component) {
    switch (component->type) {
        case VOLTAGE_SOURCE:
            set_element_voltage_source(component->value1, component->connection1->element);
            break;
        case CURRENT_SOURCE:
            set_element_current_source(component->value1, component->connection1->element);
            break;
        case RESISTOR:
            set_element_resistor(component->value1, component->connection1->element);
            break;
        case CAPACITOR:
            init_element_capacitor(component->connection1->element);
            break;
        case INDUCTOR:
            init_element_inductor(component->connection1->element);
            break;
        case DIODE: // 假设二极管是未导通状态
            init_element_diode(component->connection1->element);
            break;
        default:
            break;
    }
}

void update_dynamic_component(component_t *component, double time) {
    switch (component->type) {
        case CAPACITOR:
            update_capacitor_element(component->value1, time, component->connection1->element);
            break;
        case INDUCTOR:
            update_inductor_element(component->value1, time, component->connection1->element);
            break;
        default:
            break;
    }
}

void update_static_component(component_t *component) {
    switch (component->type) {
        case DIODE:
            update_diode_element(component->value1, component->connection1->element);
            break;
        default:
            break;
    }
}

void init_components(component_t **components, int components_count) {
    for (int i = 0; i < components_count; i++)
        init_component(components[i]);
}

void update_static_components(component_t **components, int components_count) {
    for (int i = 0; i < components_count; i++)
        update_static_component(components[i]);
}

void update_dynamic_components(component_t **components, int components_count, double time) {
    for (int i = 0; i < components_count; i++)
        update_dynamic_component(components[i], time);
}

component_t *create_complex_component_by_string(component_type_t component_type, char *component_string) {
    double value1;
    double value2;
    int node_id1;
    int node_id2;
    int node_id3;
    if (sscanf(component_string, "%*s%lf%lf%d%d%d", &value1, &value2, &node_id1, &node_id2, &node_id3) != 5)
        return NULL;
    component_t *component = create_complex_component(component_type, value1, value2);
    component->connection1->node_id_negative = component->connection2->node_id_negative = node_id1;
    component->connection1->node_id_positive = node_id2;
    component->connection2->node_id_positive = node_id3;
    return component;
}

component_t *create_simple_component_by_string(component_type_t component_type, char *component_string) {
    double value;
    int node_id1;
    int node_id2;
    if (sscanf(component_string, "%*s%lf%d%d", &value, &node_id1, &node_id2) != 3)
        return NULL;
    component_t *component = create_simple_component(component_type, value);
    component->connection1->node_id_positive = node_id1;
    component->connection1->node_id_negative = node_id2;
    return component;
}

component_t *create_component_by_string(char *component_string) {
    char type_char[2];
    if (sscanf(component_string, "%1s", type_char) != 1)
        return NULL;
    component_type_t type = get_component_type_from_character(type_char[0]);
    if (type == COMPONENT_UNKNOWN)
        return NULL;
    if (is_complex_component_type(type))
        return create_complex_component_by_string(type, component_string);
    return create_simple_component_by_string(type, component_string);
}

void generate_component_state_string(char *state_string, component_t *component) {
    if (is_complex_component_type(component->type)) {
        generate_element_state_string(state_string, component->connection1->element);
        strcat(state_string, " ");
        generate_element_state_string(state_string + strlen(state_string), component->connection1->element);
    }
    generate_element_state_string(state_string, component->connection1->element);
}