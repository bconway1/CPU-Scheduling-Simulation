//
// Created by bvcon_000 on 11/3/2019.
//

#ifndef SIMULATION_PROJECT_SIMULATION_H
#define SIMULATION_PROJECT_SIMULATION_H

#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Event.h"
#include "PCB.h"

//enum Resource_State{BUSY, FREE};

//struct event_compare {
//    bool operator() (const Event * left, const Event * right) const {
//        return left->timestamp > right->timestamp;
//    }
//};



class FCFS_Simulation {
public:
    double time;
    //stats var's
    double throughput;
    //for whe the cpu isnt used for cpu util.
    double time_idle;
    double time_set_idle;

    std::string log_file_name = "D:/Classes/CS520/Simulation_Project/FCFS_log.txt";
    std::ofstream log_file;

    PCB* cpu;
    PCB* IO_channel;

    Resource_State CPU_state;
    Resource_State IO_channel_state;

    std::vector<PCB*> PCB_queue;
    std::vector<PCB*> Finished_Queue;
    std::queue<PCB*> Ready_Queue;
    std::queue<PCB*> IO_Queue;

    FCFS_Simulation();

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

};


#endif //SIMULATION_PROJECT_SIMULATION_H
