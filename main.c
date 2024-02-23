#include <stdio.h>
#include "stdlib.h"
#include "component.h"
#include "simulator.h"

const char *circuit_file = "circuit.txt";

void printf_circuit_state(component_t **components, int components_count) {
    char state_string[64];
    for (int i = 0; i < components_count; i++) {
        generate_component_state_string(state_string, components[i]);
        printf("%s\n", state_string);
    }
}

int main(int argc, char **argv) {
    if (argc != 3)
        return -1;
    double step_time = atof(argv[1]);
    int times = atoi(argv[2]);
    FILE *fp = fopen(circuit_file, "r");
    if (fp == NULL)
        return -1;
    const int max_line_length = 128;
    char text_line[max_line_length];
    component_t *components[128];
    int components_count = 0;
    while (fgets(text_line, max_line_length, fp) != NULL && components_count < 128) {
        components[components_count] = create_component_by_string(text_line);
        if (components[components_count] != NULL)
            components_count++;
    }
    fclose(fp);
    simulator_t *simulator = create_simulator(components, components_count, step_time);
    simulate_static(simulator);
    for (int i = 0; i < times; i++) {
        simulate_step(simulator);
    }
    printf_circuit_state(components, components_count);
    delete_components(components, components_count);
    delete_simulator(simulator);
    return 0;
}