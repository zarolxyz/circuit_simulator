#include "stdlib.h"
#include "connection.h"

connection_t *create_connection(element_t *element) {
    connection_t *connection = malloc(sizeof(connection_t));
    connection->element = element;
    return connection;
}

void delete_connection(connection_t *connection) {
    free(connection);
}

int is_connection_index_in_indexes(int connection_index, int *connection_indexes, int connections_count) {
    for (int i = 0; i < connections_count; i++) {
        if (connection_index == connection_indexes[i])
            return 1;
    }
    return 0;
}

int is_node_id_in_connection(connection_t *connection, int node_id) {
    if (connection->node_id_positive == node_id || connection->node_id_negative == node_id)
        return 1;
    return 0;
}

int is_connected_in_direction(connection_t *parent, connection_t *child, direction_t direction) {
    if (direction == DIRECTION_NEGATIVE)
        return is_node_id_in_connection(child, parent->node_id_negative);
    return is_node_id_in_connection(child, parent->node_id_positive);
}

// 获取parent连接器进入child连接器再出去的方向
direction_t get_connection_direction(connection_t *parent, connection_t *child, direction_t parent_direction) {
    if (child->node_id_positive == child->node_id_negative) {
        if (child->node_id_positive != parent_direction)
            return DIRECTION_POSITIVE_NEGATIVE;
        else
            return parent_direction;
    }
    if (parent_direction == DIRECTION_POSITIVE) {
        if (child->node_id_positive == parent->node_id_positive)
            return DIRECTION_NEGATIVE;
        return DIRECTION_POSITIVE;
    }
    if (child->node_id_positive == parent->node_id_negative)
        return DIRECTION_NEGATIVE;
    return DIRECTION_POSITIVE;
}

int is_node_id_in_node_ids(int node_id, int *node_ids, int node_ids_count) {
    for (int i = 0; i < node_ids_count; i++) {
        if (node_ids[i] == node_id)
            return 1;
    }
    return 0;
}

int get_connection_node_id(connection_t *connection, int direction) {
    if (direction == DIRECTION_NEGATIVE)
        return connection->node_id_negative;
    return connection->node_id_positive;
}

int find_connections_node_ids(int *node_ids, connection_t **connections, int connections_count) {
    int node_ids_count = 0;
    for (int i = 0; i < connections_count; i++) {
        int node_id_positive = get_connection_node_id(connections[i], DIRECTION_POSITIVE);
        int node_id_negative = get_connection_node_id(connections[i], DIRECTION_NEGATIVE);
        if (!is_node_id_in_node_ids(node_id_positive, node_ids, node_ids_count)) {
            node_ids[node_ids_count] = node_id_positive;
            node_ids_count++;
        }
        if (!is_node_id_in_node_ids(node_id_negative, node_ids, node_ids_count)) {
            node_ids[node_ids_count] = node_id_negative;
            node_ids_count++;
        }
    }
    return node_ids_count;
}

int count_connections_with_node_id(int node_id, connection_t **connections, int connections_count) {
    int connected_count = 0;
    for (int i = 0; i < connections_count; i++) {
        if (is_node_id_in_connection(connections[i], node_id))
            connected_count++;
    }
    return connected_count;
}

double get_calculation_direction(direction_t direction) {
    if (direction == DIRECTION_NEGATIVE)
        return -1.0;
    if (direction == DIRECTION_POSITIVE)
        return 1.0;
    return 0.0;
}

int get_direction_node_id_to_connection(int node_id, connection_t *connection) {
    if (connection->node_id_positive == connection->node_id_negative)
        return DIRECTION_POSITIVE_NEGATIVE;
    if (connection->node_id_negative == node_id)
        return DIRECTION_POSITIVE;
    if (connection->node_id_positive == node_id)
        return DIRECTION_NEGATIVE;
    return DIRECTION_POSITIVE_NEGATIVE;
}