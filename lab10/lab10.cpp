#include <algorithm>
#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

enum class TestFunction {
    Sphere,
    ShiftedSphere,
    Rosenbrock
};

struct Particle {
    std::vector<double> position;      //x, y
    std::vector<double> velocity;      //vx, vy
    std::vector<double> bestPosition;  //PBest coordinates
    double bestFitness;                //Fitness at PBest
    double currentFitness;             //Fitness at current position

    Particle(int dimensions, double minBound, double maxBound) {
        static thread_local std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<double> distPos(minBound, maxBound);

        const double vMax = (maxBound - minBound) * 0.1;
        std::uniform_real_distribution<double> distVel(-vMax, vMax);

        position.resize(dimensions);
        velocity.resize(dimensions);
        bestPosition.resize(dimensions);

        for (int d = 0; d < dimensions; ++d) {
            position[d] = distPos(rng);
            velocity[d] = distVel(rng);
            bestPosition[d] = position[d];
        }

        bestFitness = std::numeric_limits<double>::infinity();
        currentFitness = std::numeric_limits<double>::infinity();
    }
};

class Swarm {
private:
    std::vector<Particle> particles;

    std::vector<double> globalBestPosition; //GBest
    double globalBestFitness;               //Value of GBest

    //PSO hyperparameters
    double w  = 0.7; //inertia
    double c1 = 1.4; //cognitive
    double c2 = 1.4; //social

    int dimensions;
    double minB, maxB;
    TestFunction whichFunc;

    //RNG for r1,r2 in [0,1]
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist01;

public:
    Swarm(int numParticles, int dimensions_, double minB_, double maxB_, TestFunction f)
        : globalBestFitness(std::numeric_limits<double>::infinity()),
          dimensions(dimensions_),
          minB(minB_),
          maxB(maxB_),
          whichFunc(f),
          rng(std::random_device{}()),
          dist01(0.0, 1.0)
    {
        particles.reserve(numParticles);
        for (int i = 0; i < numParticles; ++i) {
            particles.emplace_back(dimensions, minB, maxB);
        }
        globalBestPosition.assign(dimensions, 0.0);
    }

    double fitnessFunction(const std::vector<double>& pos) {
        if (pos.size() < 2) {
            throw std::runtime_error("fitnessFunction requires at least 2 dimensions (x,y).");
        }

        const double x = pos[0];
        const double y = pos[1];

        switch (whichFunc) {
            case TestFunction::Sphere:
                //f(x,y) = x^2 + y^2
                return x * x + y * y;

            case TestFunction::ShiftedSphere:
                //f(x,y) = (x-10)^2 + (y+10)^2
                return (x - 10.0) * (x - 10.0) + (y + 10.0) * (y + 10.0);

            case TestFunction::Rosenbrock:
                //f(x,y) = (1-x)^2 + 100*(y - x^2)^2
                return (1.0 - x) * (1.0 - x) + 100.0 * (y - x * x) * (y - x * x);
        }
        return std::numeric_limits<double>::infinity();
    }

    void runIteration() {
        for (auto& p : particles) {
            p.currentFitness = fitnessFunction(p.position);

            if (p.currentFitness < p.bestFitness) {
                p.bestFitness = p.currentFitness;
                p.bestPosition = p.position;
            }

            if (p.currentFitness < globalBestFitness) {
                globalBestFitness = p.currentFitness;
                globalBestPosition = p.position;
            }
        }

        for (auto& p : particles) {
            for (int d = 0; d < dimensions; ++d) {
                const double r1 = dist01(rng);
                const double r2 = dist01(rng);

                //v = w*v + c1*r1*(pBest - x) + c2*r2*(gBest - x)
                p.velocity[d] =
                    w * p.velocity[d]
                    + c1 * r1 * (p.bestPosition[d] - p.position[d])
                    + c2 * r2 * (globalBestPosition[d] - p.position[d]);

                //x = x + v
                p.position[d] += p.velocity[d];

                p.position[d] = std::clamp(p.position[d], minB, maxB);
            }
        }
    }

    std::vector<double> getGBest() const { return globalBestPosition; }
    double getGBestFitness() const { return globalBestFitness; }
};

static std::string funcName(TestFunction f) {
    switch (f) {
        case TestFunction::Sphere:        return "Sphere";
        case TestFunction::ShiftedSphere: return "Shifted Sphere";
        case TestFunction::Rosenbrock:    return "Rosenbrock";
    }
    return "Unknown";
}

int main() {
    try {
        const int dims = 2;
        const int numParticles = 30;
        const int iterations = 2000;

        auto runTest = [&](TestFunction f, double minB, double maxB, int iters) {
            Swarm swarm(numParticles, dims, minB, maxB, f);

            const int printEvery = 400;

            for (int i = 1; i <= iters; ++i) {
                swarm.runIteration();
                if (i % printEvery == 0) {
                    std::cout << funcName(f)
                              << " iter " << i
                              << " | best fitness: " << std::setprecision(10)
                              << swarm.getGBestFitness() << "\n";
                }
            }

            const auto best = swarm.getGBest();
            std::cout << "\n=== " << funcName(f) << " Result ===\n";
            std::cout << "Best position: ("
                      << std::fixed << std::setprecision(6)
                      << best[0] << ", " << best[1] << ")\n";
            std::cout << "Best fitness : " << std::setprecision(10)
                      << swarm.getGBestFitness() << "\n\n";
        };

        //1. Sphere: bounds [-10,10], global min at (0,0)
        runTest(TestFunction::Sphere, -10.0, 10.0, iterations);

        //2. Shifted Sphere: bounds [-20,20], global min at (10,-10)
        runTest(TestFunction::ShiftedSphere, -20.0, 20.0, iterations);

        //3. Rosenbrock: bounds [-5,5], global min at (1,1)
        runTest(TestFunction::Rosenbrock, -5.0, 5.0, iterations);

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
