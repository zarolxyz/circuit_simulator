#ifndef CIRCUIT_SIMULATOR_ELEMENT_H
#define CIRCUIT_SIMULATOR_ELEMENT_H

typedef enum {
    ELEMENT_UNKNOWN, VOLTAGE, CURRENT, RESISTANCE
} element_type_t;

typedef struct {
    element_type_t type;
    double voltage;
    double current;
    double resistance;
} element_t;

typedef enum {
    COMPONENT_UNKNOWN, VOLTAGE_SOURCE, CURRENT_SOURCE, RESISTOR, CAPACITOR, INDUCTOR, DIODE
} component_type_t;

typedef struct {
    component_type_t type;
    element_t *element1;
    double value1;
    //element_t *element2;
    //double value2;
} component_t;

component_t *create_simple_component(component_type_t type, double value);

void delete_component(component_t *component);

component_type_t get_component_type_from_character(char component_type);

void init_component_element(component_t *component);

void update_dynamic_component_element(component_t *component, double time);

void update_static_component_element(component_t *component);

#endif //CIRCUIT_SIMULATOR_ELEMENT_H
