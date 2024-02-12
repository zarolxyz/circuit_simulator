#include "stdlib.h"
#include "circuit.h"
#include "simulator.h"

simulator_t *create_simulator(circuit_t *circuit, double unit_time) {
    simulator_t *simulator = malloc(sizeof(simulator_t));
    simulator->circuit = circuit;
    simulator->unit_time = unit_time;
    simulator->analyzer = create_analyzer(circuit->connections, circuit->connections_count);
    return simulator;
}

void delete_simulator(simulator_t *simulator) {
    delete_analyzer(simulator->analyzer);
    free(simulator);
}

int simulate_static(simulator_t *simulator) {
    init_circuit_elements(simulator->circuit);
    if (apply_analyze(simulator->analyzer) != 0)
        return -1;
    update_circuit_elements_static(simulator->circuit);
    return apply_analyze(simulator->analyzer);
}

int simulate_step(simulator_t *simulator) {
    update_circuit_elements_dynamic(simulator->circuit, simulator->unit_time);
    if (apply_analyze(simulator->analyzer) != 0)
        return -1;
    update_circuit_elements_static(simulator->circuit);
    return apply_analyze(simulator->analyzer);
}
