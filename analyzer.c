#include "stdlib.h"
#include "element.h"
#include "connection.h"
#include "searcher.h"
#include "equation.h"
#include "analyzer.h"

analyzer_t *create_analyzer(connection_t **connections, int connections_count) {
    analyzer_t *analyzer = malloc(sizeof(analyzer_t));
    analyzer->connections = connections;
    analyzer->connections_count = connections_count;
    analyzer->nodes = malloc(sizeof(int) * calculate_maximum_nodes_count(connections_count));
    analyzer->nodes_count = search_connections_independent_nodes(analyzer->nodes, connections, connections_count);
    analyzer->loops = malloc(sizeof(path_t *) * connections_count);
    analyzer->loops_count = search_connections_independent_loops(analyzer->loops, connections, connections_count);
    analyzer->equations_count = connections_count;
    analyzer->equations = create_equations(connections_count);
    return analyzer;
}

void delete_analyzer(analyzer_t *analyzer) {
    free(analyzer->nodes);
    delete_equations(analyzer->equations, analyzer->equations_count);
    delete_paths(analyzer->loops, analyzer->loops_count);
    free(analyzer->loops);
    free(analyzer);
}

void generate_analyze_equations(double **destination, connection_t **connections, int connections_count, path_t **loops,
                                int loops_count, int *nodes, int nodes_count) {
    int equations_count = connections_count;
    int xn_count = equations_count;
    int generated_equations_count = 0;
    for (int i = 0; i < loops_count; i++) {
        erase_equation(destination[generated_equations_count], xn_count);
        path_t *loop_node = loops[i];
        while (loop_node != NULL) {
            int index = loop_node->index;
            element_t *element = connections[index]->element;
            if (element->type == VOLTAGE) {
                destination[generated_equations_count][xn_count] -=
                        element->voltage * get_calculation_direction(loop_node->direction);
            }
            if (element->type == CURRENT) {
                destination[generated_equations_count][index] = get_calculation_direction(
                        loop_node->direction);
            }
            if (element->type == RESISTANCE) {
                destination[generated_equations_count][index] =
                        get_calculation_direction(loop_node->direction) * element->resistance;
            }
            loop_node = loop_node->next;
        }
        generated_equations_count++;
    }
    for (int i = 0; i < nodes_count; i++) {
        erase_equation(destination[generated_equations_count], xn_count);
        for (int j = 0; j < connections_count; j++) {
            if (is_connected_to_node(connections[j], nodes[i])) {
                element_t *element = connections[j]->element;
                int direction = get_direction_node_to_connection(nodes[i], connections[j]);
                if (element->type == VOLTAGE || element->type == RESISTANCE) {
                    destination[generated_equations_count][j] = get_calculation_direction(direction);
                }
                if (element->type == CURRENT) {
                    destination[generated_equations_count][xn_count] -=
                            element->current * get_calculation_direction(direction);
                }
            }
        }
        generated_equations_count++;
    }
}

void apply_analyze(analyzer_t *analyzer) {
    generate_analyze_equations(analyzer->equations, analyzer->connections, analyzer->connections_count,
                               analyzer->loops,
                               analyzer->loops_count, analyzer->nodes, analyzer->nodes_count);
    solve_equations(analyzer->equations, analyzer->equations_count);
    for (int i = 0; i < analyzer->connections_count; i++) {
        element_t *element = analyzer->connections[i]->element;
        if (element->type == VOLTAGE) {
            element->current = analyzer->equations[i][analyzer->equations_count];
        }
        if (element->type == CURRENT) {
            element->voltage = analyzer->equations[i][analyzer->equations_count];
        }
        if (element->type == RESISTANCE) {
            element->current = analyzer->equations[i][analyzer->equations_count];
            element->voltage = element->current * element->resistance;
        }
    }
    return 0;
}
