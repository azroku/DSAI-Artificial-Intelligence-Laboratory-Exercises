#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>

class Container {
protected:
  std::string id;
  int weight;

public:
  Container(std::string id, int weight) {
    this->id = id;
    this->weight = weight;
  }
  virtual void getInfo(){};
  int getWeight() const { return weight; }
  std::string getId() const { return id; }
};

#endif
