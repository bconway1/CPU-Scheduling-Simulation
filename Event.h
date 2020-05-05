//
// Created by bvcon_000 on 11/3/2019.
//

#ifndef SIMULATION_PROJECT_EVENT_H
#define SIMULATION_PROJECT_EVENT_H

#include "PCB.h"


enum Event_Type{CPU_ENQUE, CPU_END, IO_ENQUE, IO_END};
enum Scheduler_Type{FCFS, RR};
enum Resource_State{BUSY, FREE};



class Event {
public:
    const double timestamp;
    const Event_Type event_type;
    const Scheduler_Type scheduler_type;
    PCB* process;
    Event (double t, Event_Type type, Scheduler_Type schedule, PCB* process) :
            timestamp(t), event_type(type), scheduler_type(schedule), process(process){}
};

//used for event queue
struct event_compare {
    bool operator() (const Event * left, const Event * right) const {
        return left->timestamp > right->timestamp;
    }
};

#endif //SIMULATION_PROJECT_EVENT_H
