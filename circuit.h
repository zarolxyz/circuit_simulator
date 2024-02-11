#ifndef CIRCUIT_SIMULATOR_CIRCUIT_H
#define CIRCUIT_SIMULATOR_CIRCUIT_H

int generate_circuit_by_text_file(connection_t **destination, FILE *fp);

void delete_circuit(connection_t **connections, int connections_count);

#endif //CIRCUIT_SIMULATOR_CIRCUIT_H
