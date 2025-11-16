#ifndef DEPARTURE_TERMINAL_H
#define DEPARTURE_TERMINAL_H

#include "Truck.h"
#include <queue>
#include <vector>
#include <iostream>

class DepartureTerminal {
private:
    int num_inspection_lanes;
    std::queue<Truck> waiting_queue;
    std::vector<Truck> inspection_lanes;
    std::vector<Truck> departed_trucks;

public:
    DepartureTerminal(int num_lanes) : num_inspection_lanes(num_lanes) {}

    void add_truck_to_queue(Truck truck) {
        truck.set_state(TruckState::WaitingForInspection);
        waiting_queue.push(truck);
    }

    void simulate_timestep() {
        for (int i = inspection_lanes.size() - 1; i >= 0; --i) {
            inspection_lanes[i].work_on_inspection();
            if (inspection_lanes[i].get_inspection_time_left() == 0) {
                inspection_lanes[i].set_state(TruckState::Departed);
                departed_trucks.push_back(inspection_lanes[i]);
                inspection_lanes.erase(inspection_lanes.begin() + i);
            }
        }

        while (inspection_lanes.size() < num_inspection_lanes && !waiting_queue.empty()) {
            Truck truck_to_inspect = waiting_queue.front();
            waiting_queue.pop();
            truck_to_inspect.set_state(TruckState::InInspection);
            inspection_lanes.push_back(truck_to_inspect);
        }
    }
    
    void print_status() const {
        std::cout << "--- Terminal Status ---" << std::endl;
        std::cout << "Trucks in Queue: " << waiting_queue.size() << std::endl;
        std::cout << "Inspection Lanes (" << inspection_lanes.size() << "/" << num_inspection_lanes << "):" << std::endl;
        if (inspection_lanes.empty()) {
            std::cout << "  - All lanes are empty." << std::endl;
        } else {
            for (const auto& truck : inspection_lanes) {
                std::cout << "  - Truck (" << truck.get_type_str() << ") with "
                          << truck.get_inspection_time_left() << " time units left." << std::endl;
            }
        }
        std::cout << "Trucks Departed: " << departed_trucks.size() << std::endl;
    }

    bool is_finished() const {
        return waiting_queue.empty() && inspection_lanes.empty();
    }
};

#endif
