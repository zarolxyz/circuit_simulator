#include "stdio.h"
#include "stdlib.h"
#include "element.h"

element_t *create_element() {
    element_t *element = malloc(sizeof(element_t));
    element->type = ELEMENT_UNKNOWN;
    return element;
}

void delete_element(element_t *element) {
    free(element);
}

component_t *create_simple_component(component_type_t type, double value) {
    component_t *component = malloc(sizeof(component_t));
    component->type = type;
    component->element1 = create_element();
    component->value1 = value;
    return component;
}

component_t *create_complex_component(component_type_t type, double value1, double value2) {
    component_t *component = malloc(sizeof(component_t));
    component->type = type;
    component->element1 = create_element();
    component->value1 = value1;
    component->element2 = create_element();
    component->value2 = value2;
    return component;
}

void delete_component(component_t *component) {
    delete_element(component->element1);
    if (is_complex_component_type(component->type))
        delete_element(component->element2);
    free(component);
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
        case 'O':
            return RELAY_OPEN;
        case 'N':
            return RELAY_CLOSED;
        default:
            break;
    }
    return COMPONENT_UNKNOWN;
}

int is_complex_component_type(component_type_t type) {
    switch (type) {
        case RELAY_OPEN:
        case RELAY_CLOSED:
            return 1;
        default:
            break;
    }
    return 0;
}

void init_component_element(component_t *component) {
    switch (component->type) {
        case VOLTAGE_SOURCE:
            component->element1->type = VOLTAGE;
            component->element1->voltage = component->value1;
            break;
        case CURRENT_SOURCE:
            component->element1->type = CURRENT;
            component->element1->current = component->value1;
            break;
        case RESISTOR:
            component->element1->type = RESISTANCE;
            component->element1->resistance = component->value1;
            break;
        case CAPACITOR:
            component->element1->type = VOLTAGE;
            component->element1->voltage = 0.0;
            break;
        case INDUCTOR:
        case DIODE: // 假设二极管是未导通状态
            component->element1->type = CURRENT;
            component->element1->current = 0.0;
            break;
        case RELAY_OPEN:
            component->element1->type = RESISTANCE;
            component->element1->resistance = component->value1;
            component->element2->type = CURRENT;
            component->element2->current = 0.0;
            break;
        case RELAY_CLOSED:
            component->element1->type = RESISTANCE;
            component->element1->resistance = component->value1;
            component->element2->type = VOLTAGE;
            component->element2->current = 0.0;
            break;
        default:
            break;
    }
}

void update_dynamic_component_element(component_t *component, double time) {
    switch (component->type) {
        case CAPACITOR:
            component->element1->voltage += component->element1->current / component->value1 * time;
            break;
        case INDUCTOR:
            component->element1->current += component->element1->voltage / component->value1 * time;
            break;
        default:
            break;
    }
}

void update_static_component_element(component_t *component) {
    switch (component->type) {
        case DIODE:
            if (component->element1->voltage >= component->value1) {
                component->element1->type = VOLTAGE;
                component->element1->voltage = component->value1;
            } else {
                component->element1->type = CURRENT;
                component->element1->current = 0.0;
            }
            break;
        case RELAY_OPEN:
            if (component->element1->voltage >= component->value2 ||
                component->element1->voltage <= -component->value2) {
                component->element2->type = VOLTAGE;
                component->element2->voltage = 0.0;
            } else {
                component->element2->type = CURRENT;
                component->element2->current = 0.0;
            }
            break;
        case RELAY_CLOSED:
            if (component->element1->voltage >= component->value2 ||
                component->element1->voltage <= -component->value2) {
                component->element2->type = CURRENT;
                component->element2->current = 0.0;
            } else {
                component->element2->type = VOLTAGE;
                component->element2->voltage = 0.0;
            }
        default:
            break;
    }
}

void generate_simple_component_state_string(char *state_string, component_t *component) {
    sprintf(state_string, "voltage:%lf current:%lf", component->element1->voltage, component->element1->current);
}

void generate_complex_component_state_string(char *state_string, component_t *component) {
    sprintf(state_string, "voltage1:%lf current1:%lf voltage2:%lf current2:%lf", component->element1->voltage,
            component->element1->current, component->element2->voltage, component->element2->current);
}

void generate_component_state_string(char *state_string, component_t *component) {
    if (is_complex_component_type(component->type))
        generate_complex_component_state_string(state_string, component);
    else
        generate_simple_component_state_string(state_string, component);
}