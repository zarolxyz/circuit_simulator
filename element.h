#ifndef CIRCUIT_SIMULATOR_ELEMENT_H
#define CIRCUIT_SIMULATOR_ELEMENT_H

typedef enum {
    VOLTAGE, CURRENT, RESISTANCE
} element_type_t;

typedef struct {
    element_type_t type;
    double voltage;
    double current;
    double resistance;
} element_t;

element_t *create_element(element_type_t type, double value);

void delete_element(element_t *element);

#endif //CIRCUIT_SIMULATOR_ELEMENT_H
