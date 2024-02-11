#include "stdlib.h"
#include "element.h"

element_t *create_element(element_type_t type, double value) {
    element_t *element = malloc(sizeof(element_t));
    element->type = type;
    element->voltage = 0.0;
    element->current = 0.0;
    element->resistance = 0.0;
    switch (type) {
        case VOLTAGE:
            element->voltage = value;
        case CURRENT:
            element->current = value;
        case RESISTANCE:
            element->resistance = value;
    }
    return element;
}

void delete_element(element_t *element) {
    free(element);
}