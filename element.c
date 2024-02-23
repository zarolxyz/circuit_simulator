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

void generate_element_state_string(char *state_string, element_t *element) {
    sprintf(state_string, "voltage:%lf current:%lf", element->voltage, element->current);
}

void update_capacitor_element(double capacitance, double time, element_t *element) {
    element->voltage += element->current / capacitance * time;
}

void update_inductor_element(double inductance, double time, element_t *element) {
    element->current += element->voltage / inductance * time;
}

void set_element_voltage_source(double voltage, element_t *element) {
    element->type = VOLTAGE;
    element->voltage = voltage;
}

void set_element_current_source(double current, element_t *element) {
    element->type = CURRENT;
    element->current = current;
}

void set_element_resistor(double resistance, element_t *element) {
    element->type = RESISTANCE;
    element->resistance = resistance;
}

void set_element_open(element_t *element) {
    set_element_current_source(0.0, element);
}

void set_element_short(element_t *element) {
    set_element_voltage_source(0.0, element);
}

void init_element_capacitor(element_t *element) {
    set_element_short(element);
}

void init_element_inductor(element_t *element) {
    set_element_open(element);
}

void init_element_diode(element_t *element) {
    set_element_open(element);
}

void init_element_transistor(element_t *element_bc, element_t *element_be) {
    init_element_diode(element_bc);
    init_element_diode(element_be);
}

void set_diode_element_conduction(double voltage, element_t *element) {
    set_element_voltage_source(voltage, element);
}

void set_diode_element_close(element_t *element) {
    set_element_open(element);
}

int is_diode_element_close(element_t *element) {
    return element->type == CURRENT;
}

void update_diode_element(double voltage, element_t *element) {
    if (is_diode_element_close(element) && !(element->voltage < voltage))
        set_diode_element_conduction(voltage, element);
    else if (!(element->current >= 0))
        set_diode_element_close(element);
}