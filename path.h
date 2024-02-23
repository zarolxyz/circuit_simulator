#ifndef CIRCUIT_SIMULATOR_PATH_H
#define CIRCUIT_SIMULATOR_PATH_H

#include "connection.h"

typedef struct path_s {
    int connection_index;
    direction_t direction;
    struct path_s *next;
} path_t;

void delete_paths(path_t **paths, int count);

int search_connections_independent_loops(path_t **loops, connection_t **connections, int connections_count);

#endif //CIRCUIT_SIMULATOR_PATH_H
