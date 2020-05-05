cmake_minimum_required(VERSION 3.8)
project(Simulation_Project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Event.cpp Event.h PCB.cpp PCB.h FCFS_Simulation.cpp FCFS_Simulation.h RR_Simulation.cpp RR_Simulation.h SJF_Simulation.cpp SJF_Simulation.h Simulation.cpp Simulation.h)
add_executable(Simulation_Project ${SOURCE_FILES})