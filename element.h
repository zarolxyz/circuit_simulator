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

element_t *create_element();

void delete_element(element_t *element);

void generate_element_state_string(char *state_string, element_t *element);

void update_capacitor_element(double capacitance, double time, element_t *element);

void update_inductor_element(double inductance, double time, element_t *element);

void set_element_voltage_source(double voltage, element_t *element);

void set_element_current_source(double current, element_t *element);

void set_element_resistor(double resistance, element_t *element);

void init_element_capacitor(element_t *element);

void init_element_inductor(element_t *element);

void init_element_diode(element_t *element);

void init_element_transistor(element_t *element_bc, element_t *element_be);

void update_diode_element(double voltage, element_t *element);

#endif //CIRCUIT_SIMULATOR_ELEMENT_H
