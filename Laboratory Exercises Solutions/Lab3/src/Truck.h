#ifndef TRUCK_H
#define TRUCK_H

#include <iostream>
#include <string>
#include <vector>

enum class TruckType { Standard, Refrigerated, ToxicWaste };
enum class TruckState { Loading, WaitingForInspection, InInspection, Departed };

class Truck {
private:
  int max_capacity;
  int max_containers;
  TruckType type;
  bool has_fragile_cargo_ = false;
  bool has_heavy_cargo_ = false;
  int current_weight;
  int current_container_count;
  std::vector<std::string> loaded_container_ids;
  TruckState state = TruckState::Loading;
  int inspection_time_left = 0;

public:
  Truck()
      : max_capacity(0), max_containers(0), type(TruckType::Standard),
        current_weight(0), current_container_count(0) {
    // Set the inspection time for different type of Trucks
    switch (type) {
    case TruckType::ToxicWaste:
      this->inspection_time_left = 10;
      break;
    case TruckType::Refrigerated:
      this->inspection_time_left = 5;
      break;
    case TruckType::Standard:
      this->inspection_time_left = 2;
      break;
    }
  }

  Truck(TruckType type, int max_capacity, int max_containers) {
    this->type = type;
    this->max_capacity = max_capacity;
    this->max_containers = max_containers;

    this->current_weight = 0;
    this->current_container_count = 0;
  }

  std::string get_type_str() const {
    switch (type) {
    case TruckType::Standard:
      return "Standard";
    case TruckType::Refrigerated:
      return "Refrigerated";
    case TruckType::ToxicWaste:
      return "ToxicWaste";
    default:
      return "Unknown";
    }
  }

  int get_current_weight() const { return current_weight; }

  int get_container_count() const { return current_container_count; }

  void print_loaded_ids() const {
    std::cout << " - Loaded Container IDs: ";
    if (loaded_container_ids.empty()) {
      std::cout << "None";
    } else {
      for (size_t i = 0; i < loaded_container_ids.size(); ++i) {
        std::cout << loaded_container_ids[i]
                  << (i == loaded_container_ids.size() - 1 ? "" : ", ");
      }
    }
    std::cout << std::endl;
  }

  void load(const std::string &container_id, int container_weight,
            bool is_fragile) {
    loaded_container_ids.push_back(container_id);
    current_weight += container_weight;
    current_container_count++;
    if (is_fragile) {
      has_fragile_cargo_ = true;
    }
    if (container_weight > 5000) {
      has_heavy_cargo_ = true;
    }
  }

  bool can_accept(int container_weight, bool is_fragile) const {
    if (current_container_count >= max_containers ||
        current_weight + container_weight > max_capacity) {
      return false;
    }
    if (is_fragile) {
      if (this->has_heavy_cargo_) {
        return false;
      }
    } else {
      if (this->has_fragile_cargo_ && container_weight > 5000) {
        return false;
      }
    }
    return true;
  }

  bool is_empty() const { return current_container_count == 0; }

  void set_state(TruckState new_state) { state = new_state; }

  int get_inspection_time_left() const { return inspection_time_left; }

  void work_on_inspection() {
    if (inspection_time_left > 0) {
      inspection_time_left--;
    }
  }
};

#endif
