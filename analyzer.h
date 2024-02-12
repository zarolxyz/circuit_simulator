#ifndef CIRCUIT_SIMULATOR_ANALYZER_H
#define CIRCUIT_SIMULATOR_ANALYZER_H

#include "connection.h"
#include "paths.h"

typedef struct {
    connection_t **connections;
    int connections_count;
    path_t **loops;
    int loops_count;
    int *nodes;
    int nodes_count;
    double **equations;
    int equations_count;
} analyzer_t;

analyzer_t *create_analyzer(connection_t **connections, int connections_count);

void delete_analyzer(analyzer_t *analyzer);

int apply_analyze(analyzer_t *analyzer);

#endif //CIRCUIT_SIMULATOR_ANALYZER_H
