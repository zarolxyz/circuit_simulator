#ifndef CIRCUIT_SIMULATOR_CONNECTION_H
#define CIRCUIT_SIMULATOR_CONNECTION_H

#include "element.h"

typedef enum {
    DIRECTION_POSITIVE, DIRECTION_POSITIVE_NEGATIVE, DIRECTION_NEGATIVE
} direction_t;

typedef struct connection_s {
    element_t *element;
    int node_id_positive;
    int node_id_negative;
} connection_t;

connection_t *create_connection(element_t *element);

void delete_connection(connection_t *connection);

int is_connection_index_in_indexes(int connection_index, int *connection_indexes, int connections_count);

int is_node_id_in_connection(connection_t *connection, int node_id);

int is_connected_in_direction(connection_t *parent, connection_t *child, direction_t direction);

direction_t get_connection_direction(connection_t *parent, connection_t *child, direction_t parent_direction);

int find_connections_node_ids(int *node_ids, connection_t **connections, int connections_count);

int count_connections_with_node_id(int node_id, connection_t **connections, int connections_count);

double get_calculation_direction(direction_t direction);

int get_direction_node_id_to_connection(int node_id, connection_t *connection);

#endif //CIRCUIT_SIMULATOR_CONNECTION_H
