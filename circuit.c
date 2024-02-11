#include<stdio.h>
#include "connection.h"
#include "element.h"
#include "circuit.h"

connection_t *create_connection_by_text(char *text_line) {
    element_type_t type;
    char type_char;
    double constant;
    int node1;
    int node2;
    sscanf(text_line, "%c %lf %d %d", &type_char, &constant, &node1, &node2);
    switch (type_char) {
        case 'U':
            type = VOLTAGE;
            break;
        case 'I':
            type = CURRENT;
            break;
        case 'R':
            type = RESISTANCE;
            break;
        default:
            return NULL;
    }
    element_t *element = create_element(type, constant);
    connection_t *connection = create_connection(element, node1, node2);
    return connection;
}

int generate_circuit_by_text_file(connection_t **destination, FILE *fp) {
    int count_generated = 0;
    const int line_length = 64;
    char text_line[line_length];
    while (fgets(text_line, line_length, fp) != NULL) {
        destination[count_generated] = create_connection_by_text(text_line);
        count_generated++;
    }
    return count_generated;
}

void delete_circuit(connection_t **connections, int connections_count) {
    for (int i = 0; i < connections_count; i++) {
        delete_element(connections[i]->element);
        delete_connection(connections[i]);
    }
}
