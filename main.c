#include <stdio.h>
#include "stdlib.h"
#include "circuit.h"
#include "simulator.h"

const char *circuit_file = "circuit.txt";

int main(int argc, char **argv) {
    if (argc != 3)
        return -1;
    double unit_time = atof(argv[1]);
    int times = atoi(argv[2]);
    circuit_t *circuit = create_circuit();
    FILE *fp = fopen(circuit_file, "r");
    if (fp == NULL)
        return -1;
    const int max_line_length = 128;
    char line[max_line_length];
    while (fgets(line, max_line_length, fp) != NULL && line[0] != '\n') {
        add_component_and_connection_by_text_line(circuit, line);
    }
    fclose(fp);
    simulator_t *simulator = create_simulator(circuit, unit_time);
    if (simulate_static(simulator) != 0) {
        delete_circuit(circuit);
        delete_simulator(simulator);
        return -1;
    }
    for (int i = 0; i < times; i++) {
        if (simulate_step(simulator) != 0) {
            delete_circuit(circuit);
            delete_simulator(simulator);
            return -1;
        }
    }
    for (int i = 0; i < circuit->connections_count; i++) {
        printf("%d voltage: %lf current: %lf\n", i + 1, circuit->connections[i]->element->voltage,
               circuit->connections[i]->element->current);
    }
    delete_circuit(circuit);
    delete_simulator(simulator);
    return 0;
}