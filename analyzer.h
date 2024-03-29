#ifndef CIRCUIT_SIMULATOR_ANALYZER_H
#define CIRCUIT_SIMULATOR_ANALYZER_H

#include "connection.h"
#include "path.h"
#include "node.h"

typedef struct {
    connection_t **connections;
    int connections_count;
    path_t **loops;
    int loops_count;
    node_t **nodes;
    int nodes_count;
    double **equations;
    int equations_count;
} analyzer_t;

analyzer_t *create_analyzer(connection_t **connections, int connections_count);

void delete_analyzer(analyzer_t *analyzer);

void apply_analyze(analyzer_t *analyzer);

#endif //CIRCUIT_SIMULATOR_ANALYZER_H
