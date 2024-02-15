#include "stdlib.h"
#include "paths.h"

// 创建一个路径链表节点
path_t *create_path_node(int index, int direction, path_t *next) {
    path_t *path_node = malloc(sizeof(path_t));
    path_node->index = index;
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

int is_index_in_path_list(int index, path_t *head) {
    while (head != NULL) {
        if (head->index == index)
            return 1;
        head = head->next;
    }
    return 0;
}

int is_index_in_paths(int index, path_t **paths, int count) {
    for (int i = 0; i < count; i++) {
        if (is_index_in_path_list(index, paths[i]))
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