//
// Created by bvcon_000 on 11/3/2019.
//

#ifndef SIMULATION_PROJECT_PCB_H
#define SIMULATION_PROJECT_PCB_H

#include <random>

enum PCB_State{READY, RUNNING};



class PCB {
public:
    //process variables
    const int pid;
    //how long this process runs (time)
    double process_burst_time;
    double io_mean_inter_arrival_time;
    //how much progress has the process made(time)
    double cpu_served_time;
    double process_arrival_time;
    PCB_State state;

    //double burst_time;
    //how much time is left till io burst; used in rr
    double time_to_io;
    //when to calculate a new busrt after io burst in rr
    bool new_burst_flag;
    //for sjf
    double alpha;
    std::vector<double> past_bursts;
    double predicted;
    //variables for process turnaround time and waiting time
    double turnaround_time;
    double waiting_time;
    double waiting_set_time;


    PCB(int _pid, double _process_burst_time, double _io_mean_inter_arrival_time,
        double _process_arrival_time);

    double get_burst_time();

    void set_waiting(double time);
    void resolve_waiting(double time);
    double compute_predited();
    void set_alpha(double alpha);
};

//compare struct for sjf priority queue for ready queue
struct pcb_compare {
    bool operator() (const PCB * left, const PCB * right) const {
        return left->predicted > right->predicted;
    }
};

#endif //SIMULATION_PROJECT_PCB_H
