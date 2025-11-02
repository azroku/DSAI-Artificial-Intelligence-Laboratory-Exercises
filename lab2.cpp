#include <iostream>
#include <vector>
#include <string>
using namespace std;

//TASK1
//toxicity levels
enum class ToxicityLevel { Low, Medium, High };

//abstract base class
class Container {
protected:
    string id;
    int weight; 

public:
    Container(const string& id, int weight) : id(id), weight(weight) {}
    virtual ~Container() {}
    virtual void getInfo() const = 0;

    //getters ship
    int getWeight() const { return weight; }
    string getId() const { return id; }
};

//ordinary 
class OrdinaryContainer : public Container {
public:
    OrdinaryContainer(const string& id, int weight) : Container(id, weight) {}
    void getInfo() const override {
        cout << "Ordinary Container - ID: " << id
             << ", Weight: " << weight << " kg\n";
    }
};

//refrigerated 
class RefrigeratedContainer : public Container {
private:
    double temperature;
public:
    RefrigeratedContainer(const string& id, int weight, double temperature)
        : Container(id, weight), temperature(temperature) {}
    void getInfo() const override {
        cout << "Refrigerated Container - ID: " << id
             << ", Weight: " << weight
             << " kg, Temp: " << temperature << " °C\n";
    }
};

//toxic 
class ToxicContainer : public Container {
private:
    ToxicityLevel toxicity;
public:
    ToxicContainer(const string& id, int weight, ToxicityLevel toxicity)
        : Container(id, weight), toxicity(toxicity) {}
    void getInfo() const override {
        cout << "Toxic Container - ID: " << id
             << ", Weight: " << weight
             << " kg, Toxicity: ";
        switch (toxicity) {
            case ToxicityLevel::Low: cout << "Low"; break;
            case ToxicityLevel::Medium: cout << "Medium"; break;
            case ToxicityLevel::High: cout << "High"; break;
        }
        cout << "\n";
    }
};

//fragile 
class FragileContainer : public Container {
public:
    FragileContainer(const string& id, int weight) : Container(id, weight) {}
    void getInfo() const override {
        cout << "Fragile Container - ID: " << id
             << ", Weight: " << weight
             << " kg, Handle with care!\n";
    }
};

//TASK2
class Ship {
private:
    string name;
    int maxLoadWeight;
    int currentLoad;

    vector<OrdinaryContainer> ordinaryCargo;
    vector<RefrigeratedContainer> refrigeratedCargo;
    vector<ToxicContainer> toxicCargo;
    vector<FragileContainer> fragileCargo;

public:
    Ship(const string& name, int maxLoadWeight)
        : name(name), maxLoadWeight(maxLoadWeight), currentLoad(0) {}

    bool addContainer(const OrdinaryContainer& c) {
        if (currentLoad + c.getWeight() > maxLoadWeight) {
            cout << "Cannot add OrdinaryContainer " << c.getId() << ". Exceeds max load!\n";
            return false;
        }
        ordinaryCargo.push_back(c);
        currentLoad += c.getWeight();
        return true;
    }

    bool addContainer(const RefrigeratedContainer& c) {
        if (currentLoad + c.getWeight() > maxLoadWeight) {
            cout << "Cannot add RefrigeratedContainer " << c.getId() << ". Exceeds max load!\n";
            return false;
        }
        refrigeratedCargo.push_back(c);
        currentLoad += c.getWeight();
        return true;
    }

    bool addContainer(const ToxicContainer& c) {
        if (currentLoad + c.getWeight() > maxLoadWeight) {
            cout << "Cannot add ToxicContainer " << c.getId() << ". Exceeds max load!\n";
            return false;
        }
        toxicCargo.push_back(c);
        currentLoad += c.getWeight();
        return true;
    }

    bool addContainer(const FragileContainer& c) {
        if (currentLoad + c.getWeight() > maxLoadWeight) {
            cout << "Cannot add FragileContainer " << c.getId() << ". Exceeds max load!\n";
            return false;
        }
        fragileCargo.push_back(c);
        currentLoad += c.getWeight();
        return true;
    }

    void printManifest() const {
        cout << "\nShip: " << name
             << "\nMax Load: " << maxLoadWeight
             << " kg, Current Load: " << currentLoad << " kg\n";
        cout << "============================\n";

        cout << "--- Ordinary Containers ---\n";
        for (const auto& c : ordinaryCargo) c.getInfo();

        cout << "--- Refrigerated Containers ---\n";
        for (const auto& c : refrigeratedCargo) c.getInfo();

        cout << "--- Toxic Containers ---\n";
        for (const auto& c : toxicCargo) c.getInfo();

        cout << "--- Fragile Containers ---\n";
        for (const auto& c : fragileCargo) c.getInfo();

        cout << "============================\n";
    }
};


int main() {
    //containers
    OrdinaryContainer o1("O-001", 1200);
    RefrigeratedContainer r1("R-101", 1500, -5.5);
    ToxicContainer t1("T-777", 1800, ToxicityLevel::High);
    FragileContainer f1("F-303", 900);

    //ship
    Ship ship("Titanic II", 5000);

    ship.addContainer(o1);
    ship.addContainer(r1);
    ship.addContainer(t1);
    ship.addContainer(f1);

    ship.printManifest();

    return 0;
}
