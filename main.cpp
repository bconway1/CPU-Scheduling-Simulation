#include <iostream>
#include "FCFS_Simulation.h"
#include "RR_Simulation.h"
#include "SJF_Simulation.h"
int main() {
    //FCFS_Simulation sim;
    //SJF_Simulation sim(0.33333);
    RR_Simulation sim(0.3);
    sim.init();
    sim.run();
    return 0;

}