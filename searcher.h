#ifndef CIRCUIT_SIMULATOR_SEARCHER_H
#define CIRCUIT_SIMULATOR_SEARCHER_H

#include "connection.h"
#include "paths.h"

int search_connections_independent_loops(path_t **loops, connection_t **connections, int connections_count);

int search_connections_independent_nodes(int *nodes, connection_t **connections, int connections_count);

#endif //CIRCUIT_SIMULATOR_SEARCHER_H
