#include "stdlib.h"
#include "connection.h"
#include "simulator.h"

simulator_t *create_simulator(component_t **components, int components_count, double step_time) {
    simulator_t *simulator = malloc(sizeof(simulator_t));
    simulator->components = components;
    simulator->components_count = components_count;
    simulator->connections = malloc(
            sizeof(connection_t *) * calculate_components_maximum_connections(components_count));
    simulator->connections_count = find_components_connections(simulator->connections, components, components_count);
    simulator->step_time = step_time;
    simulator->analyzer = create_analyzer(simulator->connections, simulator->connections_count);
    return simulator;
}

void delete_simulator(simulator_t *simulator) {
    delete_analyzer(simulator->analyzer);
    free(simulator->connections);
    free(simulator);
}

void simulate_static(simulator_t *simulator) {
    init_components(simulator->components, simulator->components_count);
    apply_analyze(simulator->analyzer);
    update_static_components(simulator->components, simulator->components_count);
    apply_analyze(simulator->analyzer);
}

void simulate_step(simulator_t *simulator) {
    update_dynamic_components(simulator->components, simulator->components_count, simulator->step_time);
    apply_analyze(simulator->analyzer);
    update_static_components(simulator->components, simulator->components_count);
    apply_analyze(simulator->analyzer);
}
