//
// Created by bvcon_000 on 11/3/2019.
//

#include "PCB.h"
#include <random>

//std::random_device pcb_rd;
//std::mt19937_64 pcb_gen(pcb_rd());
//std::exponential_distribution<double> exp_dis(0.1);
//auto exp_dis_ptr = &exp_dis;

PCB::PCB(int _pid, double _process_burst_time, double _io_mean_inter_arrival_time, double _process_arrival_time) :
        pid(_pid), process_burst_time(_process_burst_time), io_mean_inter_arrival_time(_io_mean_inter_arrival_time),
        process_arrival_time(_process_arrival_time), cpu_served_time(0.0) {

    turnaround_time = 0;
    waiting_time= 0;
    waiting_set_time= 0;

    past_bursts.push_back(0.055);

}

double PCB::get_burst_time() {
//get burst time that is exponentially dist.
    std::random_device pcb_rd;
    std::mt19937_64 pcb_gen(pcb_rd());
    std::exponential_distribution<double> exp_dis(io_mean_inter_arrival_time);

    return exp_dis(pcb_gen);
}

void PCB::set_waiting(double time){
    //set waiting time
    waiting_set_time = time;
}
void PCB::resolve_waiting(double time){
    waiting_time = waiting_time + (time - waiting_set_time);
}

double PCB::compute_predited(){
    //compute predicted burst value for sjf
    double new_predicted;
    int num_of_past_bursts = past_bursts.size() - 1;
    new_predicted = alpha * past_bursts[num_of_past_bursts];
    int j = 1;
    for(int i = num_of_past_bursts - 1; i >= 0; i--){
        new_predicted = new_predicted + (pow((1.0 - alpha), j) * alpha * past_bursts[i]);
        j++;
    }
    return new_predicted;
}

void PCB::set_alpha(double alpha){
    this->alpha = alpha;
}