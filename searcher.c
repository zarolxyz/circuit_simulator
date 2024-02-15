#include "stdlib.h"
#include "connection.h"
#include "searcher.h"

int is_index_in_indexes(int index, int *indexes, int indexes_count) {
    for (int i = 0; i < indexes_count; i++) {
        if (index == indexes[i])
            return 1;
    }
    return 0;
}

int
search_path_to_connection_recursion(path_t **paths, int *connections_searched_indexes, int searched_count,
                                    int direction,
                                    int current_index,
                                    int end_index, int end_direction,
                                    connection_t **connections,
                                    int connections_count) {
    connections_searched_indexes[searched_count] = current_index;
    searched_count++;
    int paths_count = 0;
    for (int i = 0; i < connections_count; i++) {
        if (is_connected_in_direction(connections[current_index], connections[i], direction)) {
            if (i == end_index &&
                (get_connection_direction(connections[current_index], connections[end_index], direction) ==
                 end_direction ||
                 get_connection_direction(connections[current_index], connections[end_index], direction) ==
                 DIRECTION_POSITIVE_NEGATIVE)) {
                paths[paths_count] = create_path_node(current_index, direction, NULL);
                paths_count++;
            } else if (!is_index_in_indexes(i, connections_searched_indexes, searched_count)) {
                int paths_count_searched = search_path_to_connection_recursion(&paths[paths_count],
                                                                               connections_searched_indexes,
                                                                               searched_count,
                                                                               get_connection_direction(
                                                                                       connections[current_index],
                                                                                       connections[i], direction),
                                                                               i, end_index, end_direction, connections,
                                                                               connections_count);
                for (int j = 0; j < paths_count_searched; j++) {
                    path_t *current_path_node = create_path_node(current_index, direction, paths[paths_count]);
                    paths[paths_count] = current_path_node;
                    paths_count++;
                }
            }
        }
    }
    return paths_count;
}

int search_connection_loops(path_t **loops, int start_index, connection_t **connections, int connections_count) {
    int connection_searched_indexes[connections_count];
    return search_path_to_connection_recursion(loops, connection_searched_indexes, 0, DIRECTION_POSITIVE, start_index,
                                               start_index, DIRECTION_POSITIVE, connections, connections_count);
}

path_t *search_connection_shortest_loop(int start_index, connection_t **connections, int connections_count) {
    path_t *loops[connections_count];
    int loops_count = search_connection_loops(loops, start_index, connections, connections_count);
    if (loops_count < 1)
        return NULL;
    int shortest_loop_index = find_paths_shortest_path_index(loops, loops_count);
    delete_paths(loops, shortest_loop_index);
    delete_paths(&loops[shortest_loop_index + 1], loops_count - (shortest_loop_index + 1));
    return loops[shortest_loop_index];
}

int search_connections_independent_loops(path_t **loops, connection_t **connections, int connections_count) {
    int loops_count = 0;
    for (int i = 0; i < connections_count; i++) {
        if (!is_index_in_paths(i, loops, loops_count)) {
            path_t *shortest_loop = search_connection_shortest_loop(i, connections, connections_count);
            if (shortest_loop != NULL) {
                loops[loops_count] = shortest_loop;
                loops_count++;
            }
        }
    }
    return loops_count;
}

int is_node_independent(int index, int *nodes, int nodes_count, connection_t **connections, int connections_count) {
    if (nodes_count < 1)
        return 1;
    int node = nodes[index];
    for (int i = 0; i < connections_count; i++) {
        if (is_connected_to_node(connections[i], node)) {
            if (get_direction_node_to_connection(node, connections[i]) != DIRECTION_POSITIVE_NEGATIVE) {
                if (!is_connected_to_nodes(connections[i], nodes, index) &&
                    !is_connected_to_nodes(connections[i], nodes + index + 1, nodes_count - index - 1))
                    return 1;
            }
        }
    }
    return 0;
}

int search_connections_independent_nodes(int *nodes, connection_t **connections, int connections_count) {
    int nodes_count = find_connections_nodes(nodes, connections, connections_count);
    for (int i = nodes_count - 1; i >= 0; i--) {
        if (!is_node_independent(i, nodes, nodes_count, connections, connections_count)) {
            nodes[i] = nodes[nodes_count - 1];
            nodes_count--;
        }
    }
    return nodes_count;
}
