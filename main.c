#include <stdio.h>
#include "searcher.h"
#include "analyzer.h"
#include "circuit.h"

const char *circuit_file = "circuit.txt";
const int connections_max = 128;

int main() {
    FILE *fp = fopen(circuit_file, "r");
    if (fp == NULL)
        return -1;
    connection_t *connections[connections_max];
    int connections_count = generate_circuit_by_text_file(connections, fp);
    fclose(fp);
    analyzer_t *analyzer = create_analyzer(connections, connections_count);
    apply_analyze(analyzer);
    delete_analyzer(analyzer);
    for (int i = 0; i < connections_count; i++) {
        printf("%d voltage: %lf current: %lf\n", i + 1, connections[i]->element->voltage,
               connections[i]->element->current);
    }
    delete_circuit(connections, connections_count);
    return 0;
}