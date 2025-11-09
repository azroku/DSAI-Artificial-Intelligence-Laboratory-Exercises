#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "Container.h"
#include <iostream>
#include <string>

enum class ToxicityLevel { Low, Medium, High };

class OrdinaryContainer : public Container {
protected:
  bool is_fragile;

public:
  using Container::Container;
  void getInfo() override { std::cout << this->id << this->weight; }
  bool isFragile() const { return is_fragile; }
};
class FragileContainer : public Container {
public:
  using Container::Container;
  void getInfo() override { std::cout << this->id << this->weight; }
};

class ToxicContainer : public Container {
  ToxicityLevel toxicity_level;

public:
  ToxicContainer(std::string id, int weight, ToxicityLevel toxicity_level)
      : Container(id, weight), toxicity_level(toxicity_level) {}
  void getInfo() override {
    switch (toxicity_level) {
    case ToxicityLevel::Low:
      std::cout << this->id << this->weight << "Low";
      break;
    case ToxicityLevel::Medium:
      std::cout << this->id << this->weight << "Medium";
      break;
    case ToxicityLevel::High:
      std::cout << this->id << this->weight << "High";
      break;
    }
  }
};

class RefrigeratedContainer : public Container {
  int temperature;

public:
  RefrigeratedContainer(std::string id, int weight, int temperature)
      : Container(id, weight), temperature(temperature) {}
  void getInfo() override {
    std::cout << this->id << this->weight << temperature;
  }
};

#endif
