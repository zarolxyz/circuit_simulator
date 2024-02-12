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
    //component->element2 = NULL;
    return component;
}

void delete_component(component_t *component) {
    delete_element(component->element1);
    //delete_element(component->element2);
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
        default:
            break;
    }
    return COMPONENT_UNKNOWN;
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
            component->element1->type = CURRENT;
            component->element1->current = 0.0;
        case DIODE: // 假设二极管是未导通状态
            component->element1->type = CURRENT;
            component->element1->current = 0.0;
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
        default:
            break;
    }
}