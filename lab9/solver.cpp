#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <cmath>

class Solver {
    int _populationSize;
    std::vector<double> _minX;
    std::vector<double> _maxX;
    std::vector<std::vector<double>> _population;
    std::function<double(std::vector<double>)> _fitness;

    std::mt19937 _rng;

    int _generations = 1200;
    int _tournamentSize = 3;
    double _crossoverRate = 0.85;
    double _mutationRate = 0.20;      //one gene probability
    double _mutationStrength = 0.10;  //fraction of range
    int _elitismCount = 1;

    static double clamp(double v, double lo, double hi) {
        return std::max(lo, std::min(v, hi));
    }

    double uniform01() {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(_rng);
    }

    double uniform(double lo, double hi) {
        std::uniform_real_distribution<double> dist(lo, hi);
        return dist(_rng);
    }

    double evalFitness(const std::vector<double>& x) const {
        if (!_fitness) throw std::runtime_error("Fitness function not set. Call setFitness(...) first.");
        return _fitness(x);
    }

public:
    Solver(int populationSize, std::vector<double> minX, std::vector<double> maxX)
        : _populationSize(populationSize), _minX(std::move(minX)), _maxX(std::move(maxX)) {

        if (_populationSize <= 0) throw std::invalid_argument("populationSize must be > 0");
        if (_minX.size() != _maxX.size()) throw std::invalid_argument("minX and maxX must have same dimension");
        if (_minX.empty()) throw std::invalid_argument("Dimension must be >= 1");

        for (size_t i = 0; i < _minX.size(); i++) {
            if (!(_minX[i] <= _maxX[i])) throw std::invalid_argument("Each minX[i] must be <= maxX[i]");
        }

        std::random_device rd;
        _rng = std::mt19937(rd());
    }

    void setFitness(std::function<double(std::vector<double>)> fitness) {
        _fitness = std::move(fitness);
    }

    void Initialize() { //initialization
        _population.clear();
        _population.reserve(_populationSize);

        const size_t dim = _minX.size();
        for (int i = 0; i < _populationSize; i++) {
            std::vector<double> indiv(dim);
            for (size_t d = 0; d < dim; d++) {
                indiv[d] = uniform(_minX[d], _maxX[d]);
            }
            _population.push_back(std::move(indiv));
        }
    }

    std::vector<double> Select() { //minimization
        if (_population.empty()) throw std::runtime_error("Population not initialized. Call Initialize().");

        std::uniform_int_distribution<int> idxDist(0, (int)_population.size() - 1);

        int bestIdx = idxDist(_rng);
        double bestFit = evalFitness(_population[bestIdx]);

        for (int k = 1; k < _tournamentSize; k++) {
            int candIdx = idxDist(_rng);
            double candFit = evalFitness(_population[candIdx]);
            if (candFit < bestFit) {
                bestFit = candFit;
                bestIdx = candIdx;
            }
        }
        return _population[bestIdx];
    }

    std::vector<double> Crossover(std::vector<double> a, std::vector<double> b) { //crossover
        const size_t dim = _minX.size();
        if (a.size() != dim || b.size() != dim) throw std::invalid_argument("Crossover parents have wrong dimension");

        if (uniform01() > _crossoverRate) {
            return (uniform01() < 0.5) ? a : b;
        }

        std::vector<double> child(dim);
        for (size_t d = 0; d < dim; d++) {
            double alpha = uniform01();
            double v = alpha * a[d] + (1.0 - alpha) * b[d];
            child[d] = clamp(v, _minX[d], _maxX[d]);
        }
        return child;
    }

    std::vector<double> Mutation(std::vector<double> c) { //mutation
        const size_t dim = _minX.size();
        if (c.size() != dim) throw std::invalid_argument("Mutation child has wrong dimension");

        std::normal_distribution<double> normal(0.0, 1.0);

        for (size_t d = 0; d < dim; d++) {
            if (uniform01() < _mutationRate) {
                double range = _maxX[d] - _minX[d];
                double step = normal(_rng) * (_mutationStrength * range);
                c[d] = clamp(c[d] + step, _minX[d], _maxX[d]);
            }
        }
        return c;
    }

    void solve(double &y, std::vector<double> &x) { //solve
        if (!_fitness) throw std::runtime_error("Fitness function not set. Call setFitness(...) first.");

        Initialize();
        y = evalFitness(_population[0]);
        x = _population[0];
        for (int i = 1; i < _populationSize; i++) {
            double fi = evalFitness(_population[i]);
            if (fi < y) {
                y = fi;
                x = _population[i];
            }
        }

        for (int gen = 0; gen < _generations; gen++) {
            std::vector<int> idx(_populationSize);
            for (int i = 0; i < _populationSize; i++) idx[i] = i;

            std::sort(idx.begin(), idx.end(), [&](int i, int j) {
                return evalFitness(_population[i]) < evalFitness(_population[j]);
            });

            std::vector<std::vector<double>> newPop;
            newPop.reserve(_populationSize);
            for (int e = 0; e < _elitismCount && e < _populationSize; e++) {
                newPop.push_back(_population[idx[e]]);
            }

            while ((int)newPop.size() < _populationSize) {
                std::vector<double> p1 = Select();
                std::vector<double> p2 = Select();

                std::vector<double> child = Crossover(p1, p2);
                child = Mutation(child);
                for (size_t d = 0; d < child.size(); d++) {
                    child[d] = clamp(child[d], _minX[d], _maxX[d]);
                }

                newPop.push_back(std::move(child));
            }

            _population = std::move(newPop);

            for (int i = 0; i < _populationSize; i++) {
                double fi = evalFitness(_population[i]);
                if (fi < y) {
                    y = fi;
                    x = _population[i];
                }
            }
        }
    }
};

static void printVec(const std::vector<double>& x) {
    std::cout << "[";
    for (size_t i = 0; i < x.size(); i++) {
        std::cout << x[i] << (i + 1 < x.size() ? ", " : "");
    }
    std::cout << "]";
}

int main() {
    std::cout.setf(std::ios::fixed);
    std::cout.precision(8);
    //1. Find the minimum of x^2+(y − 2)^2 in range x∈(−10, 10) and y∈(−10, 10)
    //answer: x=0, y=2, f=0
    {
        Solver s(200, {-10.0, -10.0}, {10.0, 10.0});
        s.setFitness([](std::vector<double> v) {
            double x = v[0], y = v[1];
            return x*x + (y - 2.0)*(y - 2.0);
        });

        double bestY;
        std::vector<double> bestX;
        s.solve(bestY, bestX);

        std::cout << "1. (min x^2 + (y-2)^2)\n";
        std::cout << "  best y = " << bestY << "\n";
        std::cout << "  best x = "; printVec(bestX); std::cout << "\n";
    }

    //2. Find the maximum of e^(−x^2) in range x∈(−15, 15)
    //answer: x=0, max=1
    {
        Solver s(150, {-15.0}, {15.0});
        s.setFitness([](std::vector<double> v) {
            double x = v[0];
            return -std::exp(-(x*x));
        });

        double bestNeg;
        std::vector<double> bestX;
        s.solve(bestNeg, bestX);

        double bestMax = -bestNeg;
        std::cout << "2. (max exp(-x^2))\n";
        std::cout << "  best max value = " << bestMax << "\n";
        std::cout << "  best x = "; printVec(bestX); std::cout << "\n";
    }

    //3. Find the minimum of the sum {i=1..5} (x_i - i)^2 in range xi∈(−10, 10)
    //answer: [1,2,3,4,5], min=0
    {
        Solver s(300,
                 {-10.0, -10.0, -10.0, -10.0, -10.0},
                 { 10.0,  10.0,  10.0,  10.0,  10.0});

        s.setFitness([](std::vector<double> v) {
            double sum = 0.0;
            for (int i = 1; i <= 5; i++) {
                double diff = v[i - 1] - (double)i;
                sum += diff * diff;
            }
            return sum;
        });

        double bestY;
        std::vector<double> bestX;
        s.solve(bestY, bestX);

        std::cout << "3. (min sum_{i=1..5} (x_i - i)^2)\n";
        std::cout << "  best y = " << bestY << "\n";
        std::cout << "  best x = "; printVec(bestX); std::cout << "\n";
    }

    return 0;
}