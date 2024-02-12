#ifndef CIRCUIT_SIMULATOR_CONNECTION_H
#define CIRCUIT_SIMULATOR_CONNECTION_H

#include "element.h"

// 从连接器通过的方向
#define DIRECTION_POSITIVE 1
#define DIRECTION_POSITIVE_NEGATIVE 0
#define DIRECTION_NEGATIVE (-1)

// 节点是一个整数标志，两个连接器有相同节点则两个连接器是互相连接的
typedef struct connection_s {
    element_t *element;
    int node_positive; // 从node_positive进入为正方向
    int node_negative;
} connection_t;
// 连接器用于描述元素之间的连接关系，node_positive表示元件的正极或形式正极连接的节点

connection_t *create_connection(element_t *element, int node_positive, int node_negative);

void delete_connection(connection_t *connection);

int is_connected_to_node(connection_t *connection, int node);

int is_connected_to_connection(connection_t *connection1, connection_t *connection2);

int is_connected_in_direction(connection_t *parent, connection_t *child, int direction);

int get_connection_direction(connection_t *parent, connection_t *child, int parent_direction);

int find_nodes_by_indexes(int *nodes, int *indexes, int indexes_count, connection_t **connections);

int calculate_maximum_nodes_count(int connections_count);

double get_calculation_direction(int direction);

int get_direction_node_to_connection(int node, connection_t *connection);

#endif //CIRCUIT_SIMULATOR_CONNECTION_H
