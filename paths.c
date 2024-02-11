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

int delete_path_list_in_paths(int index, path_t **paths, int paths_count) {
    delete_path_list(paths[index]);
    paths[index] = paths[paths_count - 1];
    paths_count--;
    return paths_count;
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