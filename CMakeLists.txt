cmake_minimum_required(VERSION 3.27)
project(circuit_simulator C)

set(CMAKE_C_STANDARD 11)

add_executable(circuit_simulator main.c
        element.c
        element.h
        connection.h
        connection.c
        path.c
        path.h
        equation.c
        equation.h
        analyzer.c
        analyzer.h
        simulator.c
        simulator.h
        component.c
        component.h
        node.c
        node.h
)
