//
// Created by bvcon_000 on 11/9/2019.
//

#include "SJF_Simulation.h"
#include <iostream>
#include <fstream>
#include <random>

#define SJF_IO_TIME 0.06
//min and max execution time
double sjf_min = 2 * 60;
double sjf_max = 4 * 60;
//uniform dist. for exec time
std::random_device sjf_rd;
std::mt19937_64 sjf_rng(sjf_rd());
std::uniform_real_distribution<double> sjf_uni_gen(sjf_min,sjf_max);


SJF_Simulation::SJF_Simulation(double alpha) : time (0.0), alpha(alpha), CPU_state(FREE), IO_channel_state(FREE), Event_Queue() {
    throughput = 0;
    time_idle = 0;
    time_set_idle = 0;
}

void SJF_Simulation::schedule_event(Event * new_event){
    Event_Queue.push(new_event);
}

void SJF_Simulation::init(){
    //intialize sim with processes and first events
    double inter_arrival_time = 0.03;
    for(int i = 0; i < 10; i++){

        PCB_queue.push_back(new PCB(i, sjf_uni_gen(sjf_rng), inter_arrival_time, 0.0));
        inter_arrival_time = inter_arrival_time + 0.005;
    }
    for(unsigned int j = 0; j < PCB_queue.size(); j++){
        schedule_event(new Event(0.0, CPU_ENQUE, FCFS, PCB_queue[j]));
        PCB_queue[j]->set_alpha(alpha);
    }
    log_file.open(log_file_name);
    log_file << "processes listed of the form 'process#: predicted time(seconds);'" << std::endl << std::endl;
}

void SJF_Simulation::add_to_ready_queue(PCB* process){
    //add process to ready queue vector and print to log file
    process->predicted = process->compute_predited();
    Ready_Queue.push(process);
    std::priority_queue<PCB*, std::vector<PCB *, std::allocator<PCB*> >, pcb_compare> temp = Ready_Queue;
    log_file << "process: " << process->pid << " added to ready queue" << std::endl;
    log_file << "Ready Queue: ";
    for(int i = 0; i < temp.size(); i++){
        PCB* p = temp.top();
        log_file << p->pid << ": " << p->predicted << "; ";
        temp.pop();
    }
    log_file << std::endl;
    process->set_waiting(time);

}

PCB* SJF_Simulation::remove_from_ready_queue(){
//remove proces from ready queue and print to log file
    PCB* process = Ready_Queue.top();
    Ready_Queue.pop();
    std::priority_queue<PCB*, std::vector<PCB *, std::allocator<PCB*> >, pcb_compare> temp = Ready_Queue;
    log_file << "process: " << process->pid << " removed from ready queue " << std::endl;
    log_file << "Ready Queue: ";
    for(int i = 0; i < temp.size(); i++){
        PCB* p = temp.top();
        log_file << p->pid << " ";
        temp.pop();
    }
    log_file << std::endl;
    process->resolve_waiting(time);

    return process;
}


void SJF_Simulation::add_to_io_queue(PCB* process){

    IO_Queue.push(process);
    std::queue<PCB*> temp = IO_Queue;
    log_file << "process: " << process->pid << " added to io queue" << std::endl;
    log_file << "IO Queue: ";
    for(int i = 0; i < temp.size(); i++){
        PCB* p = temp.front();
        log_file << p->pid << " ";
        temp.pop();
    }
    log_file << std::endl;

}

PCB* SJF_Simulation::remove_from_io_queue(){
    PCB* process = IO_Queue.front();
    IO_Queue.pop();
    std::queue<PCB*> temp = IO_Queue;
    log_file << "process: " << process->pid << " removed from IO queue " << std::endl;
    log_file << "IO Queue: ";
    for(int i = 0; i < temp.size(); i++){
        PCB* p = temp.front();
        log_file << p->pid << " ";
        temp.pop();
    }
    log_file << std::endl;
    return process;
}


void SJF_Simulation::cpu_pick_handler(){
    //take sprocess form ready queue and calculates the predicted burst time and inserts it into predicted burst piority queue
    if(Ready_Queue.empty() != true){
        if(CPU_state == FREE && time != 0.0){
            time_idle = time_idle + (time - time_set_idle);
        }
        CPU_state = BUSY;
//        cpu = Ready_Queue.front();
//        cpu->resolve_waiting(time);
//        Ready_Queue.pop();
        cpu = remove_from_ready_queue();
        double burst_length = cpu->get_burst_time();
        cpu->past_bursts.push_back(burst_length);
        cpu->cpu_served_time =  cpu->cpu_served_time + burst_length;
        if(burst_length > (cpu->process_burst_time - cpu->cpu_served_time)){
            schedule_event(new Event((time + (cpu->process_burst_time - cpu->cpu_served_time)),CPU_END, FCFS, cpu));
        }
        else{
            schedule_event(new Event((time + burst_length),CPU_END, FCFS, cpu));
        }
    }
    else{
        CPU_state = FREE;
        time_set_idle = time;
    }
}
void SJF_Simulation::cpu_end_handler(){
    //schedul;es io event
    if((cpu->cpu_served_time) >= (cpu->process_burst_time)){
        cpu->turnaround_time = time;
        Finished_Queue.push_back(cpu);
        cpu = nullptr;
    }
    else{
        schedule_event(new Event(time, IO_ENQUE, FCFS, cpu));
        cpu = nullptr;

    }
    if(Ready_Queue.empty()){
        CPU_state = FREE;
        time_set_idle = time;
    }
    else{
        cpu_pick_handler();
    }
}

void SJF_Simulation::io_pick_handler(){
    //schedules io end event
    if(IO_Queue.empty() != true){
        IO_channel_state = BUSY;
//
        IO_channel = remove_from_io_queue();
        schedule_event(new Event((time + SJF_IO_TIME),IO_END, FCFS, IO_channel));
    }
    else{
        CPU_state = FREE;
    }
}

void SJF_Simulation::io_end_handler(){
    //schedules cpu enque event
    schedule_event(new Event(time, CPU_ENQUE, FCFS, IO_channel));
    IO_channel = nullptr;
    if(IO_Queue.empty()){
        IO_channel_state = FREE;
    }
    else{
        io_pick_handler();
    }
}


void SJF_Simulation::event_handler(Event* event){
    //runs relevant function based on event type
    if(event->event_type == CPU_ENQUE){
        if(CPU_state == FREE){
            add_to_ready_queue(event->process);
//            Ready_Queue.push(event->process);
//            event->process->set_waiting(time);
            cpu_pick_handler();
        }
        else{
//            Ready_Queue.push(event->process);
//            event->process->set_waiting(time);
            add_to_ready_queue(event->process);
        }
    }
    else if(event->event_type == CPU_END){
        cpu_end_handler();
    }
    else if(event->event_type == IO_ENQUE){
        if(IO_channel_state == FREE){
//            IO_Queue.push(event->process);
            add_to_io_queue(event->process);
            io_pick_handler();
        }
        else{
//            IO_Queue.push(event->process);
            add_to_io_queue(event->process);
        }
    }
    else if(event->event_type == IO_END){
        io_end_handler();
    }
    else{
        //error
        std::cout << "Event type error" << std::endl;
    }

}

void SJF_Simulation::run(){
    //run simulation until no more events
    while (! Event_Queue.empty() ) {
        //get the next event on queue
        Event * current_event = Event_Queue.top();
        Event_Queue.pop();
        //update time
        time = current_event->timestamp;
        //execute event then delete it
        //current_event->process_event();
        event_handler(current_event);
        delete current_event;
    }
    for(int i = 0; i < Finished_Queue.size(); i++){
        std::cout << Finished_Queue[i]->pid << std::endl;
    }
//print out final stats
    throughput = double(10)/time;
    log_file << std::endl;
    log_file << "Throughput: " << throughput << "processes/sec" << std::endl;
    log_file << "CPU Utilization: " << double(100) - ((time_idle/time) * double(100)) << "%" << std::endl;
    double total_waiting_time;
    for(int p = 0; p < Finished_Queue.size(); p++){
        log_file << "Process " << Finished_Queue[p]->pid << ":" << std::endl;
        log_file << "   Waiting time: " << Finished_Queue[p]->waiting_time << " seconds" << std::endl;
        log_file << "   Turnaround time: " << Finished_Queue[p]->turnaround_time << " seconds" << std::endl;
        total_waiting_time = total_waiting_time + Finished_Queue[p]->waiting_time;
    }
    log_file << "System Average Waiting Time: " << total_waiting_time/10 << " seconds";
    log_file.close();

}