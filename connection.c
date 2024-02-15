#include "stdlib.h"
#include "connection.h"

connection_t *create_connection(element_t *element, int node_positive, int node_negative) {
    connection_t *connection = malloc(sizeof(connection_t));
    connection->element = element;
    connection->node_positive = node_positive;
    connection->node_negative = node_negative;
    return connection;
}

void delete_connection(connection_t *connection) {
    free(connection);
}

int is_connected_to_node(connection_t *connection, int node) {
    if (connection->node_positive == node || connection->node_negative == node)
        return 1;
    return 0;
}

int is_connected_to_nodes(connection_t *connection, int *nodes, int nodes_count) {
    for (int i = 0; i < nodes_count; i++) {
        if (is_connected_to_node(connection, nodes[i]))
            return 1;
    }
    return 0;
}

int is_connected_in_direction(connection_t *parent, connection_t *child, int direction) {
    if (direction == DIRECTION_NEGATIVE)
        return is_connected_to_node(child, parent->node_negative);
    return is_connected_to_node(child, parent->node_positive);
}

// 获取parent连接器进入child连接器再出去的方向
int get_connection_direction(connection_t *parent, connection_t *child, int parent_direction) {
    if (child->node_positive == child->node_negative)
        return DIRECTION_POSITIVE_NEGATIVE;
    if (parent_direction == DIRECTION_POSITIVE) {
        if (child->node_positive == parent->node_positive)
            return DIRECTION_NEGATIVE;
        return DIRECTION_POSITIVE;
    }
    if (child->node_positive == parent->node_negative)
        return DIRECTION_NEGATIVE;
    return DIRECTION_POSITIVE; // 默认正方向
}

int is_node_in_nodes(int node, int *nodes, int nodes_count) {
    for (int i = 0; i < nodes_count; i++) {
        if (nodes[i] == node)
            return 1;
    }
    return 0;
}

int get_connection_node(connection_t *connection, int direction) {
    if (direction == DIRECTION_NEGATIVE)
        return connection->node_negative;
    return connection->node_positive;
}

int find_connections_nodes(int *nodes, connection_t **connections, int connections_count) {
    int nodes_count = 0;
    for (int i = 0; i < connections_count; i++) {
        int node_positive = get_connection_node(connections[i], DIRECTION_POSITIVE);
        int node_negative = get_connection_node(connections[i], DIRECTION_NEGATIVE);
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

int calculate_maximum_nodes_count(int connections_count) {
    return connections_count * 2;
}

double get_calculation_direction(int direction) {
    if (direction == DIRECTION_NEGATIVE)
        return -1.0;
    if (direction == DIRECTION_POSITIVE)
        return 1.0;
    return 0.0;
}

int get_direction_node_to_connection(int node, connection_t *connection) {
    if (connection->node_positive == connection->node_negative)
        return DIRECTION_POSITIVE_NEGATIVE;
    if (connection->node_negative == node)
        return DIRECTION_POSITIVE;
    if (connection->node_positive == node)
        return DIRECTION_NEGATIVE;
    return DIRECTION_POSITIVE_NEGATIVE;
}