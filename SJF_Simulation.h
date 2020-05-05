//
// Created by bvcon_000 on 11/9/2019.
//

#ifndef SIMULATION_PROJECT_SJF_SIMULATION_H
#define SIMULATION_PROJECT_SJF_SIMULATION_H

#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Event.h"
#include "PCB.h"



class SJF_Simulation {
public:
    double time;
    //stats var's
    double throughput;
    double time_idle;
    double time_set_idle;

    double alpha;

    std::string log_file_name = "D:/Classes/CS520/Simulation_Project/SJF_log.txt";
    std::ofstream log_file;

    PCB* cpu;
    PCB* IO_channel;

    Resource_State CPU_state;
    Resource_State IO_channel_state;

    std::vector<PCB*> PCB_queue;
    std::vector<PCB*> Finished_Queue;
    //std::queue<PCB*> Ready_Queue;
    std::queue<PCB*> IO_Queue;

    SJF_Simulation(double alpha);

    void schedule_event(Event * new_event);
    void init();
    void run();
    void cpu_pick_handler();
    void cpu_end_handler();
    void io_pick_handler();
    void io_end_handler();
    void event_handler(Event* event);
    void add_to_ready_queue(PCB* process);
    PCB* remove_from_ready_queue();
    void add_to_io_queue(PCB* process);
    PCB* remove_from_io_queue();


protected:
    std::priority_queue<Event*, std::vector<Event *, std::allocator<Event*> >, event_compare> Event_Queue;
    //sorted based on predicted burst time computes each time before process is added to queue
    std::priority_queue<PCB*, std::vector<PCB *, std::allocator<PCB*> >, pcb_compare> Ready_Queue;
};


#endif //SIMULATION_PROJECT_SJF_SIMULATION_H
