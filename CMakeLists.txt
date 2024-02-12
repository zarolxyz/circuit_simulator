cmake_minimum_required(VERSION 3.27)
project(circuit_simulator C)

set(CMAKE_C_STANDARD 11)

add_executable(circuit_simulator main.c
        element.c
        element.h
        connection.h
        connection.c
        searcher.c
        searcher.h
        equation.c
        equation.h
        analyzer.c
        analyzer.h
        paths.c
        paths.h
        circuit.c
        circuit.h
        simulator.c
        simulator.h
)
