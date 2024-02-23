#include "stdlib.h"
#include "connection.h"
#include "path.h"

// 创建一个路径链表节点
path_t *create_path_node(int connection_index, direction_t direction, path_t *next) {
    path_t *path_node = malloc(sizeof(path_t));
    path_node->connection_index = connection_index;
    path_node->direction = direction;
    path_node->next = next;
    return path_node;
}

void delete_path_node(path_t *node) {
    free(node);
}

void delete_path_list(path_t *head) {
    path_t *next;
    while (head != NULL) {
        next = head->next;
        delete_path_node(head);
        head = next;
    }
}

void delete_paths(path_t **paths, int count) {
    for (int i = 0; i < count; i++) {
        delete_path_list(paths[i]);
    }
}

int is_connection_index_in_path_list(int connection_index, path_t *head) {
    while (head != NULL) {
        if (head->connection_index == connection_index)
            return 1;
        head = head->next;
    }
    return 0;
}

int is_connection_index_in_paths(int connection_index, path_t **paths, int count) {
    for (int i = 0; i < count; i++) {
        if (is_connection_index_in_path_list(connection_index, paths[i]))
            return 1;
    }
    return 0;
}

int count_path_node(path_t *head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

int find_paths_shortest_path_index(path_t **paths, int paths_count) {
    if (paths_count < 1)
        return -1;
    int shortest_index = 0;
    int shortest_length = count_path_node(paths[0]);
    for (int i = 1; i < paths_count; i++) {
        int length = count_path_node(paths[i]);
        if (length < shortest_length) {
            shortest_index = i;
            shortest_length = count_path_node(paths[shortest_index]);
        }
    }
    return shortest_index;
}

int
search_path_to_connection_recursion(path_t **paths, int *connections_searched_indexes, int searched_count,
                                    direction_t direction,
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
                 end_direction)) {
                paths[paths_count] = create_path_node(current_index, direction, NULL);
                paths_count++;
            } else if (!is_connection_index_in_indexes(i, connections_searched_indexes, searched_count) &&
                       get_connection_direction(connections[current_index], connections[i], direction) !=
                       DIRECTION_POSITIVE_NEGATIVE) {
                int paths_count_searched = search_path_to_connection_recursion(paths + paths_count,
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
    delete_paths(loops + shortest_loop_index + 1, loops_count - (shortest_loop_index + 1));
    return loops[shortest_loop_index];
}

int search_connections_independent_loops(path_t **loops, connection_t **connections, int connections_count) {
    int loops_count = 0;
    for (int i = 0; i < connections_count; i++) {
        if (!is_connection_index_in_paths(i, loops, loops_count)) {
            path_t *shortest_loop = search_connection_shortest_loop(i, connections, connections_count);
            if (shortest_loop != NULL) {
                loops[loops_count] = shortest_loop;
                loops_count++;
            }
        }
    }
    return loops_count;
}
