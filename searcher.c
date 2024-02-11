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

// 通过一个连接器(connection)的索引找到所有与之有连接关系的连接器
int search_connections_connected_recursion(int *searched_indexes, int searched_count, int connection_index,
                                           connection_t **connections,
                                           int connections_count) {
    searched_indexes[searched_count] = connection_index;
    searched_count += 1;
    for (int i = 0; i < connections_count; i++) {
        if (!is_index_in_indexes(i, searched_indexes, searched_count)) {
            if (is_connected_to_connection(connections[connection_index], connections[i])) {
                searched_count = search_connections_connected_recursion(searched_indexes, searched_count, i,
                                                                        connections, connections_count);
            }
        }
    }
    return searched_count;
}

int search_connections_connected(int *connected_indexes, int connection_index, connection_t **connections,
                                 int connections_count) {
    return search_connections_connected_recursion(connected_indexes, 0, connection_index, connections,
                                                  connections_count);
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

int search_nodes_by_indexes(int *nodes, int *indexes, int indexes_count, connection_t **connections) {
    int nodes_count = 0;
    for (int i = 0; i < indexes_count; i++) {
        int node_positive = get_connection_node(connections[indexes[i]], DIRECTION_POSITIVE);
        int node_negative = get_connection_node(connections[indexes[i]], DIRECTION_NEGATIVE);
        if (!is_node_in_nodes(node_positive, nodes, nodes_count)) {
            nodes[nodes_count] = node_positive;
            nodes_count++;
        }
        if (!is_node_in_nodes(node_negative, nodes, nodes_count)) {
            nodes[nodes_count] = node_negative;
            nodes_count++;
        }
    }
    return nodes_count;
}

int search_connections_loops(path_t **loops, connection_t **connections, int connections_count) {
    int loops_count = 0;
    for (int i = 0; i < connections_count; i++) {
        if (!is_index_in_paths(i, loops, loops_count))
            loops_count += search_connection_loops(&loops[loops_count], i, connections, connections_count);
    }
    return loops_count;
}

int search_related_paths(int *related_indexes, int *indexes, int indexes_count, path_t **paths, int paths_count) {
    int related_count = 0;
    for (int i = 0; i < paths_count; i++) {
        if (is_index_in_indexes(paths[i]->index, indexes, indexes_count)) {
            related_indexes[related_count] = i;
            related_count++;
        }
    }
    return related_count;
}

int delete_dependent_loops(path_t **loops, int loops_count, connection_t **connections, int connections_count) {
    int searched_connections_indexes[connections_count];
    int searched_connections_count = 0;
    for (int i = 0; i < connections_count; i++) {
        if (!is_index_in_indexes(i, searched_connections_indexes, searched_connections_count)) {
            int connected_count = search_connections_connected(
                    &searched_connections_indexes[searched_connections_count], i,
                    connections, connections_count);
            int related_loops_indexes[loops_count];
            int related_loops_count = search_related_paths(related_loops_indexes,
                                                           &searched_connections_indexes[searched_connections_count],
                                                           connected_count, loops,
                                                           loops_count);
            int related_nodes[calculate_maximum_nodes_count(connected_count)];
            int related_nodes_count = search_nodes_by_indexes(related_nodes,
                                                              &searched_connections_indexes[searched_connections_count],
                                                              connected_count,
                                                              connections);
            int delete_count = related_loops_count - (connected_count - (related_nodes_count - 1));
            for (int j = 0; j < delete_count; j++) {
                loops_count = delete_path_list_in_paths(related_loops_indexes[j], loops, loops_count);
            }
            searched_connections_count += connected_count;
        }
    }
    return loops_count;
}

int search_connections_independent_loops(path_t **loops, connection_t **connections, int connections_count) {
    int loops_count = search_connections_loops(loops, connections, connections_count);
    loops_count = delete_dependent_loops(loops, loops_count, connections, connections_count);
    return loops_count;
}

int search_connections_independent_node(int *nodes, connection_t **connections, int connections_count) {
    int nodes_count = 0;
    int searched_connections_indexes[connections_count];
    int searched_connections_count = 0;
    for (int i = 0; i < connections_count; i++) {
        if (!is_index_in_indexes(i, searched_connections_indexes, searched_connections_count)) {
            int connected_count = search_connections_connected(
                    &searched_connections_indexes[searched_connections_count], i, connections, connections_count);
            nodes_count += search_nodes_by_indexes(&nodes[nodes_count],
                                                   &searched_connections_indexes[searched_connections_count],
                                                   connected_count,
                                                   connections);
            nodes_count -= 1;
            searched_connections_count += connected_count;
        }
    }
    return nodes_count;
}