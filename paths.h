#ifndef CIRCUIT_SIMULATOR_PATHS_H
#define CIRCUIT_SIMULATOR_PATHS_H

typedef struct path_s {
    int index;
    int direction;
    struct path_s *next;
} path_t;

path_t *create_path_node(int index, int direction, path_t *next);

void delete_path_node(path_t *node);

void delete_path_list(path_t *head);

void delete_paths(path_t **paths, int count);

int is_index_in_path_list(int index, path_t *head);

int is_index_in_paths(int index, path_t **paths, int count);

int find_paths_shortest_path_index(path_t **paths, int paths_count);

#endif //CIRCUIT_SIMULATOR_PATHS_H
