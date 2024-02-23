#include "stdlib.h"
#include "connection.h"
#include "node.h"

node_t *create_node(int maximum_connections_count) {
    node_t *node = malloc(sizeof(node_t));
    node->connection_indexes = malloc(sizeof(int) * maximum_connections_count);
    node->directions = malloc(sizeof(direction_t) * maximum_connections_count);
    node->connections_count = 0;
    return node;
}

void delete_node(node_t *node) {
    free(node->connection_indexes);
    free(node->directions);
    free(node);
}

void delete_nodes(node_t **nodes, int nodes_count) {
    for (int i = 0; i < nodes_count; i++)
        delete_node(nodes[i]);
}

int calculate_maximum_nodes_count(int connections_count) {
    return connections_count * 2;
}

void append_node_connection(node_t *node, int connection_index, direction_t direction) {
    node->connection_indexes[node->connections_count] = connection_index;
    node->directions[node->connections_count] = direction;
    node->connections_count++;
}

node_t *create_node_by_node_id(int node_id, connection_t **connections, int connections_count) {
    node_t *node = create_node(count_connections_with_node_id(node_id, connections, connections_count));
    for (int i = 0; i < connections_count; i++) {
        if (is_node_id_in_connection(connections[i], node_id) &&
            get_direction_node_id_to_connection(node_id, connections[i]) != DIRECTION_POSITIVE_NEGATIVE)
            append_node_connection(node, i, get_direction_node_id_to_connection(node_id, connections[i]));
    }
    return node;
}

int search_connections_nodes(node_t **nodes, connection_t **connections, int connections_count) {
    int node_ids[calculate_maximum_nodes_count(connections_count)];
    int nodes_count = find_connections_node_ids(node_ids, connections, connections_count);
    for (int i = 0; i < nodes_count; i++)
        nodes[i] = create_node_by_node_id(node_ids[i], connections, connections_count);
    return nodes_count;
}

int find_node_index_with_least_connections_count(node_t **nodes, int nodes_count) {
    int least_index = 0;
    for (int i = 1; i < nodes_count; i++) {
        if (nodes[i]->connections_count < nodes[least_index]->connections_count)
            least_index = i;
    }
    return least_index;
}

void sort_nodes_by_connections_count(node_t **nodes, int nodes_count) {
    for (int i = 0; i < nodes_count - 1; i++) {
        int least_node_index = i + find_node_index_with_least_connections_count(nodes + i, nodes_count - i);
        node_t *least_node = nodes[least_node_index];
        nodes[least_node_index] = nodes[i];
        nodes[i] = least_node;
    }
}

int is_connection_index_in_nodes(int connection_index, node_t **nodes, int nodes_count) {
    for (int i = 0; i < nodes_count; i++) {
        if (is_connection_index_in_indexes(connection_index, nodes[i]->connection_indexes, nodes[i]->connections_count))
            return 1;
    }
    return 0;
}

int is_node_independent(int node_index, node_t **sorted_nodes, int nodes_count) {
    node_t *node = sorted_nodes[node_index];
    for (int i = 0; i < node->connections_count; i++) {
        if (!is_connection_index_in_nodes(node->connection_indexes[i], sorted_nodes, node_index) &&
            !is_connection_index_in_nodes(node->connection_indexes[i], sorted_nodes + node_index + 1,
                                          nodes_count - node_index - 1))
            return 1;
    }
    return 0;
}

int delete_dependent_nodes(node_t **nodes, int nodes_count) {
    sort_nodes_by_connections_count(nodes, nodes_count);
    for (int i = nodes_count - 1; i >= 0; i--) {
        if (!is_node_independent(i, nodes, nodes_count)) {
            delete_node(nodes[i]);
            nodes[i] = nodes[nodes_count - 1];
            nodes_count--;
        }
    }
    return nodes_count;
}

int search_connections_independent_nodes(node_t **nodes, connection_t **connections, int connections_count) {
    int nodes_count = search_connections_nodes(nodes, connections, connections_count);
    nodes_count = delete_dependent_nodes(nodes, nodes_count);
    return nodes_count;
}