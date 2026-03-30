```
#include <chrono>
#include "pugixml.hpp"
#include <utility>
#include <vector>
#include <cmath>
#include <iostream>

std::vector<float> CORRECT_ANSWERS = {
    0,0,
    0,1,
    1,0,
    1,1
};
std::vector<float> Answers = {};

struct Neuron {
    float bias;
    std::vector<float> weights;

    Neuron(float b, std::vector<float> w) {
        bias = b;
        weights = w;
    }

    float Forward(std::vector<float> input) {
        float sum = 0;
        for (int i = 0; i < weights.size(); i++) {
            sum = sum + (input[i] * weights[i]);
        }
        sum = sum + bias;
        float output = 1 / (1 + exp(-sum));
        return output;
    }
};

struct Layer {
    std::vector<Neuron> neurons;

    Layer(std::vector<Neuron> n) {
        neurons = n;
    }

    std::vector<float> Forward(std::vector<float> input) {
        std::vector<float> outputs;

        for (int i = 0; i < neurons.size(); i++) {
            float result = neurons[i].Forward(input);
            outputs.push_back(result);
        }
        return outputs;
    }
};

struct Backprop {
    std::vector<Layer> layers;
    float loss = 0;

    Backprop(std::vector<Layer> l) {
        layers = l;
    }

private:
        float Sigmoid_Derivative(float output) {
            float Deriv = output * (1 - output);
            return Deriv;
        }
        float CalculateOutputGradients(float output, float loss) {
            float Deriv = Sigmoid_Derivative(output);
            float Grad = loss * Deriv;
            return Grad;
        }
    public:
        float CalculateLoss(std::vector<float> predicted, std::vector<float> expected) {
            loss = 0;
            for (int i = 0; i < predicted.size(); i++) {
                float Diff = predicted[i] - expected[i];
                loss = loss + pow(Diff, 2);
            }
            return loss;
        }
        std::vector<float> CalculateGradients(float loss, std::vector<float> outputs) {
            std::vector<float> gradients;
            for (int i = 0; i < outputs.size(); i++) {
                float grad = CalculateOutputGradients(outputs[i], loss);
                gradients.push_back(grad);
            }
            return gradients;
        }
        std::vector<float> ChangeWeights(std::vector<float> outputs, float LR) {
            std::vector<float> gradients = CalculateGradients(CalculateLoss(Answers, CORRECT_ANSWERS), outputs);
            for (int i = layers.size() - 1; i > 0; i--) {
                for (int Neurons = layers[i].neurons.size() - 1; Neurons > 0; Neurons--) {
                    for (int w = 0; w < layers[i].neurons[Neurons].weights.size(); w++) {
                        layers[i].neurons[Neurons].weights[w] = layers[i].neurons[Neurons].weights[w] - (LR * gradients[Neurons]);
                    }
                }
            }
        };
};

struct Model {
    std::vector<Layer> layers;

    Model(std::vector<Layer> l) {
        layers = l;
    }

    Model(const Layer & layer, const Layer & output_layer);;

    std::vector<float> Forward(std::vector<float> input) {
        std::vector<float> outputs = input;

        for (int i = 0; i < layers.size(); i++) {
            std::vector<float> result = layers[i].Forward(outputs);
            outputs = result;
            Backprop bp(layers);
            Answers = result;
        }
        return outputs;
    }
};