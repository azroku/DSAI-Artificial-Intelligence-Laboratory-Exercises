#include <vector>
#include <random>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <iomanip>

struct Neuron {
    double output;                 //activation value (a)
    double gradient;               //error term (delta)
    std::vector<double> weights;   //weights connecting to the next layer
    double bias;                   //bias for this neuron
};

class NeuralNetwork {
private:
    //A network is a vector of layers, where each layer is a vector of Neurons
    std::vector<std::vector<Neuron>> layers;
    double learningRate;

    static double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    static double sigmoidDerivFromOutput(double out) {
        return out * (1.0 - out);
    }

public:
    //TASK 1    
    NeuralNetwork(const std::vector<int>& topology, double lr) {
        if (topology.size() < 2) {
            throw std::invalid_argument("Topology must have at least input and output layers.");
        }

        learningRate = lr;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(-1.0, 1.0);

        layers.resize(topology.size());

        for (size_t layerIndex = 0; layerIndex < topology.size(); ++layerIndex) {
            const int numNeurons = topology[layerIndex];
            layers[layerIndex].resize(numNeurons);

            const int numOutputs = (layerIndex == topology.size() - 1)
                                   ? 0
                                   : topology[layerIndex + 1];

            for (int n = 0; n < numNeurons; ++n) {
                Neuron neuron;
                neuron.output = 0.0;
                neuron.gradient = 0.0;

                //Random bias in [-1, 1] 
                neuron.bias = dist(gen);

                //Random outgoing weights in [-1, 1]
                neuron.weights.resize(numOutputs);
                for (int w = 0; w < numOutputs; ++w) {
                    neuron.weights[w] = dist(gen);
                }

                layers[layerIndex][n] = neuron;
            }
        }
    }

    //TASK 2
    void feedForward(const std::vector<double>& inputVals) {
        if (inputVals.size() != layers[0].size()) {
            throw std::invalid_argument("Input size does not match number of input neurons.");
        }

        for (size_t i = 0; i < inputVals.size(); ++i) {
            layers[0][i].output = inputVals[i];
        }

        //Propagate forward
        for (size_t layerIndex = 1; layerIndex < layers.size(); ++layerIndex) {
            auto& prevLayer = layers[layerIndex - 1];
            auto& currLayer = layers[layerIndex];

            for (size_t n = 0; n < currLayer.size(); ++n) {
                double z = 0.0;

                for (size_t p = 0; p < prevLayer.size(); ++p) {
                    z += prevLayer[p].output * prevLayer[p].weights[n];
                }
                z += currLayer[n].bias;
                currLayer[n].output = sigmoid(z);
            }
        }
    }

    //TASK 3    
    void backProp(const std::vector<double>& targetVals) {
        const size_t lastLayerIdx = layers.size() - 1;

        if (targetVals.size() != layers[lastLayerIdx].size()) {
            throw std::invalid_argument("Target size does not match number of output neurons.");
        }

        //1. Calculate Output Gradients
        auto& outputLayer = layers[lastLayerIdx];
        for (size_t n = 0; n < outputLayer.size(); ++n) {
            const double out = outputLayer[n].output;
            const double error = targetVals[n] - out;
            outputLayer[n].gradient = error * sigmoidDerivFromOutput(out);
        }

        //2. Calculate Hidden Gradients
        for (size_t layerIdx = lastLayerIdx - 1; layerIdx >= 1; --layerIdx) {
            auto& hiddenLayer = layers[layerIdx];
            auto& nextLayer = layers[layerIdx + 1];

            for (size_t i = 0; i < hiddenLayer.size(); ++i) {
                double sum = 0.0;

                for (size_t j = 0; j < nextLayer.size(); ++j) {
                    sum += nextLayer[j].gradient * hiddenLayer[i].weights[j];
                }

                const double out = hiddenLayer[i].output;
                hiddenLayer[i].gradient = sum * sigmoidDerivFromOutput(out);
            }

            if (layerIdx == 1) break;
        }

        //3. Update Weights
        for (size_t layerIdx = 1; layerIdx < layers.size(); ++layerIdx) {
            auto& prevLayer = layers[layerIdx - 1];
            auto& currLayer = layers[layerIdx];

            for (size_t i = 0; i < prevLayer.size(); ++i) {
                for (size_t j = 0; j < currLayer.size(); ++j) {
                    prevLayer[i].weights[j] += learningRate * currLayer[j].gradient * prevLayer[i].output;
                }
            }

            //Also update biases: 
            for (size_t j = 0; j < currLayer.size(); ++j) {
                currLayer[j].bias += learningRate * currLayer[j].gradient;
            }
        }
    }

    std::vector<double> getResults() const {
        std::vector<double> results;
        const auto& outLayer = layers.back();
        results.reserve(outLayer.size());
        for (const auto& n : outLayer) {
            results.push_back(n.output);
        }
        return results;
    }
};

int main() {
    NeuralNetwork net({2, 3, 1}, 0.5);

    //XOR
    std::vector<std::vector<double>> inputs = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 0.0},
        {1.0, 1.0}
    };

    std::vector<std::vector<double>> targets = {
        {0.0},
        {1.0},
        {1.0},
        {0.0}
    };

    //training
    const int epochs = 10000;
    for (int e = 0; e < epochs; ++e) {
        for (size_t i = 0; i < inputs.size(); ++i) {
            net.feedForward(inputs[i]);
            net.backProp(targets[i]);
        }
    }

    //testing
    std::cout << std::fixed << std::setprecision(4);
    for (size_t i = 0; i < inputs.size(); ++i) {
        net.feedForward(inputs[i]);
        auto out = net.getResults();
        std::cout << inputs[i][0] << " XOR " << inputs[i][1]
                  << " = " << out[0] << "\n";
    }

    return 0;
}