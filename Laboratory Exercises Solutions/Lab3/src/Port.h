#ifndef PORT_H
#define PORT_H

#include "Ship.h"
#include "Truck.h"
#include <stack>
#include <vector>
#include "DepartureTerminal.h"

class Port {
  std::vector<std::stack<OrdinaryContainer>> sorting_yard;

public:
  std::vector<Truck> create_unloading_plan(Ship &ship) {
    std::vector<Truck> filled_trucks;

    Truck current_toxic_truck;
    bool truck_is_active = false;

    for (const auto &container : ship.get_toxic_cargo()) {
      if (!truck_is_active ||
          !current_toxic_truck.can_accept(container.getWeight(), false)) {
        if (truck_is_active) {
          filled_trucks.push_back(current_toxic_truck);
        }
        current_toxic_truck = Truck(TruckType::ToxicWaste, 15000, 2);
        truck_is_active = true;
      }
      current_toxic_truck.load(container.getId(), container.getWeight(), false);
    }

    if (truck_is_active && !current_toxic_truck.is_empty()) {
      filled_trucks.push_back(current_toxic_truck);
    }

    Truck current_ref_truck;
    truck_is_active = false;

    for (const auto &container : ship.get_refrigerated_cargo()) {
      if (!truck_is_active ||
          !current_ref_truck.can_accept(container.getWeight(), false)) {
        if (truck_is_active) {
          filled_trucks.push_back(current_ref_truck);
        }
        current_ref_truck = Truck(TruckType::Refrigerated, 18000, 5);
        truck_is_active = true;
      }
      current_ref_truck.load(container.getId(), container.getWeight(), false);
    }

    if (truck_is_active && !current_ref_truck.is_empty()) {
      filled_trucks.push_back(current_ref_truck);
    }

    Truck current_std_truck;
    truck_is_active = false;

    for (const auto &container : ship.get_ordinary_cargo()) {
      if (!truck_is_active ||
          !current_std_truck.can_accept(container.getWeight(), false)) {
        if (truck_is_active) {
          filled_trucks.push_back(current_std_truck);
        }
        current_std_truck = Truck(TruckType::Standard, 20000, 10);
        truck_is_active = true;
      }
      current_std_truck.load(container.getId(), container.getWeight(), false);
    }

    for (const auto &container : ship.get_fragile_cargo()) {
      if (!truck_is_active ||
          !current_std_truck.can_accept(container.getWeight(), true)) {
        if (truck_is_active) {
          filled_trucks.push_back(current_std_truck);
        }
        current_std_truck = Truck(TruckType::Standard, 20000, 10);
        truck_is_active = true;
      }
      current_std_truck.load(container.getId(), container.getWeight(), true);
    }

    if (truck_is_active && !current_std_truck.is_empty()) {
      filled_trucks.push_back(current_std_truck);
    }

    return filled_trucks;
  }
  std::vector<Truck> createUnloadingPlan2(Ship &ship) {
    // This part stays the same as in the previous task/loading plan
    std::vector<Truck> filled_trucks;

    Truck current_toxic_truck;
    bool truck_is_active = false;

    for (const auto &container : ship.get_toxic_cargo()) {
      if (!truck_is_active ||
          !current_toxic_truck.can_accept(container.getWeight(), false)) {
        if (truck_is_active) {
          filled_trucks.push_back(current_toxic_truck);
        }
        current_toxic_truck = Truck(TruckType::ToxicWaste, 15000, 2);
        truck_is_active = true;
      }
      current_toxic_truck.load(container.getId(), container.getWeight(), false);
    }

    if (truck_is_active && !current_toxic_truck.is_empty()) {
      filled_trucks.push_back(current_toxic_truck);
    }

    Truck current_ref_truck;
    truck_is_active = false;

    for (const auto &container : ship.get_refrigerated_cargo()) {
      if (!truck_is_active ||
          !current_ref_truck.can_accept(container.getWeight(), false)) {
        if (truck_is_active) {
          filled_trucks.push_back(current_ref_truck);
        }
        current_ref_truck = Truck(TruckType::Refrigerated, 18000, 5);
        truck_is_active = true;
      }
      current_ref_truck.load(container.getId(), container.getWeight(), false);
    }

    if (truck_is_active && !current_ref_truck.is_empty()) {
      filled_trucks.push_back(current_ref_truck);
    }

    Truck current_std_truck;
    truck_is_active = false;
    // Here we first load all containers from the ship to the sorting yard
    for (const auto &container : ship.get_ordinary_cargo()) {
      bool container_placed = false;
      for (std::stack<OrdinaryContainer> &current_stack : sorting_yard) {
        if (container.getWeight() <= current_stack.top().getWeight()) {
          current_stack.push(container);
          container_placed = true;
          break;
        }
      }
      if (!container_placed) {
        std::stack<OrdinaryContainer> new_stack;
        new_stack.push(container);
        sorting_yard.push_back(new_stack);
      }
    }

    current_std_truck = Truck(TruckType::Standard, 20000, 10);
    truck_is_active = true;

    while (!sorting_yard.empty()) {
      int best_stack_idx = -1;
      int max_weight = -1;

      // Find the heaviest container available at the top of any stack
      for (int i = 0; i < sorting_yard.size(); ++i) {
        if (!sorting_yard[i].empty()) {
          if (sorting_yard[i].top().getWeight() > max_weight) {
            max_weight = sorting_yard[i].top().getWeight();
            best_stack_idx = i;
          }
        }
      }

      // Cautionary check to see if the sorting yard is empty
      if (best_stack_idx == -1)
        break;

      const OrdinaryContainer &container_to_load =
          sorting_yard[best_stack_idx].top();

      if (!current_std_truck.can_accept(container_to_load.getWeight(),
                                        container_to_load.isFragile())) {
        if (!current_std_truck.is_empty()) {
          filled_trucks.push_back(current_std_truck);
        }
        current_std_truck = Truck(TruckType::Standard, 20000, 10);
      }

      current_std_truck.load(container_to_load.getId(),
                             container_to_load.getWeight(),
                             container_to_load.isFragile());
      sorting_yard[best_stack_idx].pop();

      // If a stack becomes empty, remove it from the yard
      if (sorting_yard[best_stack_idx].empty()) {
        sorting_yard.erase(sorting_yard.begin() + best_stack_idx);
      }
    }

    // Add the last active truck to the list if it is not empty
    if (truck_is_active && !current_std_truck.is_empty()) {
      filled_trucks.push_back(current_std_truck);
    }

    return filled_trucks;
  }

  void create_unloading_plan3(Ship &ship, DepartureTerminal &terminal) {
    Truck current_toxic_truck;
    bool truck_is_active = false;

    for (const auto &container : ship.get_toxic_cargo()) {
      if (!truck_is_active ||
          !current_toxic_truck.can_accept(container.getWeight(), false)) {
        if (truck_is_active && !current_toxic_truck.is_empty()) {
          terminal.add_truck_to_queue(current_toxic_truck);
        }
        current_toxic_truck = Truck(TruckType::ToxicWaste, 15000, 2);
        truck_is_active = true;
      }
      current_toxic_truck.load(container.getId(), container.getWeight(), false);
    }

    if (truck_is_active && !current_toxic_truck.is_empty()) {
      terminal.add_truck_to_queue(current_toxic_truck);
    }

    Truck current_ref_truck;
    truck_is_active = false;

    for (const auto &container : ship.get_refrigerated_cargo()) {
      if (!truck_is_active ||
          !current_ref_truck.can_accept(container.getWeight(), false)) {
        if (truck_is_active && !current_ref_truck.is_empty()) {
          terminal.add_truck_to_queue(current_ref_truck);
        }
        current_ref_truck = Truck(TruckType::Refrigerated, 18000, 5);
        truck_is_active = true;
      }
      current_ref_truck.load(container.getId(), container.getWeight(), false);
    }

    if (truck_is_active && !current_ref_truck.is_empty()) {
      terminal.add_truck_to_queue(current_ref_truck);
    }

    Truck current_std_truck;
    truck_is_active = false;

    for (const auto &container : ship.get_ordinary_cargo()) {
      if (!truck_is_active ||
          !current_std_truck.can_accept(container.getWeight(), false)) {
        if (truck_is_active && !current_std_truck.is_empty()) {
          terminal.add_truck_to_queue(current_std_truck);
        }
        current_std_truck = Truck(TruckType::Standard, 20000, 10);
        truck_is_active = true;
      }
      current_std_truck.load(container.getId(), container.getWeight(), false);
    }

    for (const auto &container : ship.get_fragile_cargo()) {
      if (!truck_is_active ||
          !current_std_truck.can_accept(container.getWeight(), true)) {
        if (truck_is_active && !current_std_truck.is_empty()) {
          terminal.add_truck_to_queue(current_std_truck);
        }
        current_std_truck = Truck(TruckType::Standard, 20000, 10);
        truck_is_active = true;
      }
      current_std_truck.load(container.getId(), container.getWeight(), true);
    }

    if (truck_is_active && !current_std_truck.is_empty()) {
      terminal.add_truck_to_queue(current_std_truck);
    }
  }
};

#endif
