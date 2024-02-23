#ifndef CIRCUIT_SIMULATOR_NODE_H
#define CIRCUIT_SIMULATOR_NODE_H

#include "connection.h"

typedef struct {
    int *connection_indexes;
    direction_t *directions;
    int connections_count;
} node_t;

void delete_nodes(node_t **nodes, int nodes_count);

int calculate_maximum_nodes_count(int connections_count);

int search_connections_independent_nodes(node_t **nodes, connection_t **connections, int connections_count);

#endif //CIRCUIT_SIMULATOR_NODE_H
