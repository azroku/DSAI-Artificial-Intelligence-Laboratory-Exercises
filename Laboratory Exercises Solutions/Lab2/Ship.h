#ifndef SHIP_H
#define SHIP_H

#include "Containers.h"
#include <stdexcept>
#include <string>
#include <vector>

class Ship {
  std::string name;
  int max_overload_weight;
  std::vector<OrdinaryContainer> ordinary_cargo;
  std::vector<FragileContainer> fragile_cargo;
  std::vector<ToxicContainer> toxic_cargo;
  std::vector<RefrigeratedContainer> refrigerated_cargo;

private:
  // Here we will use c++ templates to create a generic function that will
  // calculate the current sum for all types of containers - this could have
  // also been done by function overloading (creating a separate function for
  // each container type) to learn more about templates:
  // https://www.geeksforgeeks.org/cpp/templates-cpp/

  template <typename T>
  int getSum(const std::vector<T> &container_vector) const {
    int total_weight = 0;
    for (const auto &container : container_vector) {
      total_weight += container.getWeight();
    }
    return total_weight;
  }
  // Again using templates to print cargo details for each type of contanier
  template <typename T>
  void print_cargo_details(const std::string &cargo_type,
                           const std::vector<T> &cargo_vector) const {
    std::cout << "  " << cargo_type << " (" << cargo_vector.size()
              << "):" << std::endl;
    if (cargo_vector.empty()) {
      std::cout << "    - None" << std::endl;
    } else {
      for (const auto &container : cargo_vector) {
        // Assumes container classes have getID() and getWeight() methods
        std::cout << "    - ID: " << container.getId()
                  << ", Weight: " << container.getWeight() << " kg"
                  << std::endl;
      }
    }
  }

public:
  Ship(const std::string &name, int max_weight)
      : name(name), max_overload_weight(max_weight) {}

  // Again, here we will use function overloading to show how it works - this
  // could have been done using templates (probably more appropriate, but for
  // instructional purposes we are using overloading)
  void add_container(OrdinaryContainer container) {
    int ordinary_cargo_sum = getSum<OrdinaryContainer>(ordinary_cargo);
    int fragile_cargo_sum = getSum<FragileContainer>(fragile_cargo);
    int toxic_cargo_sum = getSum<ToxicContainer>(toxic_cargo);
    int refrigerated_cargo_sum =
        getSum<RefrigeratedContainer>(refrigerated_cargo);

    if (ordinary_cargo_sum + fragile_cargo_sum + toxic_cargo_sum +
            refrigerated_cargo_sum + container.getWeight() >
        max_overload_weight) {
      throw std::overflow_error("Exceeded max ship weight");
    }
    ordinary_cargo.push_back(container);
  }
  void add_container(FragileContainer container) {
    int ordinary_cargo_sum = getSum<OrdinaryContainer>(ordinary_cargo);
    int fragile_cargo_sum = getSum<FragileContainer>(fragile_cargo);
    int toxic_cargo_sum = getSum<ToxicContainer>(toxic_cargo);
    int refrigerated_cargo_sum =
        getSum<RefrigeratedContainer>(refrigerated_cargo);

    if (ordinary_cargo_sum + fragile_cargo_sum + toxic_cargo_sum +
            refrigerated_cargo_sum + container.getWeight() >
        max_overload_weight) {
      throw std::overflow_error("Exceeded max ship weight");
    }
    fragile_cargo.push_back(container);
  }
  void add_container(ToxicContainer container) {
    int ordinary_cargo_sum = getSum<OrdinaryContainer>(ordinary_cargo);
    int fragile_cargo_sum = getSum<FragileContainer>(fragile_cargo);
    int toxic_cargo_sum = getSum<ToxicContainer>(toxic_cargo);
    int refrigerated_cargo_sum =
        getSum<RefrigeratedContainer>(refrigerated_cargo);

    if (ordinary_cargo_sum + fragile_cargo_sum + toxic_cargo_sum +
            refrigerated_cargo_sum + container.getWeight() >
        max_overload_weight) {
      throw std::overflow_error("Exceeded max ship weight");
    }
    toxic_cargo.push_back(container);
  }
  void add_container(RefrigeratedContainer container) {
    int ordinary_cargo_sum = getSum<OrdinaryContainer>(ordinary_cargo);
    int fragile_cargo_sum = getSum<FragileContainer>(fragile_cargo);
    int toxic_cargo_sum = getSum<ToxicContainer>(toxic_cargo);
    int refrigerated_cargo_sum =
        getSum<RefrigeratedContainer>(refrigerated_cargo);

    if (ordinary_cargo_sum + fragile_cargo_sum + toxic_cargo_sum +
            refrigerated_cargo_sum + container.getWeight() >
        max_overload_weight) {
      throw std::overflow_error("Exceeded max ship weight");
    }
    refrigerated_cargo.push_back(container);
  }

  const std::vector<OrdinaryContainer> &get_ordinary_cargo() const {
    return ordinary_cargo;
  }
  const std::vector<FragileContainer> &get_fragile_cargo() const {
    return fragile_cargo;
  }
  const std::vector<ToxicContainer> &get_toxic_cargo() const {
    return toxic_cargo;
  }
  const std::vector<RefrigeratedContainer> &get_refrigerated_cargo() const {
    return refrigerated_cargo;
  }

  void print_manifest() const {
    std::cout << "========================================" << std::endl;
    std::cout << "Ship Manifest for: " << this->name << std::endl;
    std::cout << "========================================" << std::endl;

    print_cargo_details("Ordinary Cargo", ordinary_cargo);
    print_cargo_details("Fragile Cargo", fragile_cargo);
    print_cargo_details("Toxic Cargo", toxic_cargo);
    print_cargo_details("Refrigerated Cargo", refrigerated_cargo);

    int total_weight = getSum<OrdinaryContainer>(ordinary_cargo) +
                       getSum<FragileContainer>(fragile_cargo) +
                       getSum<ToxicContainer>(toxic_cargo) +
                       getSum<RefrigeratedContainer>(refrigerated_cargo);

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total Cargo Weight: " << total_weight << " kg" << std::endl;
    std::cout << "Max Overload Weight: " << this->max_overload_weight << " kg"
              << std::endl;
    std::cout << "========================================" << std::endl;
  }
};

#endif
