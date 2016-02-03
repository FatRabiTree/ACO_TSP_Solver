#pragma once
#include "Tsp.hpp"
#include <random>
#include <functional>

struct Solution {
    std::vector<size_t> path;
    double cost;
};

struct AcoParameters {
    size_t iterations = 30;
    size_t numAnts = 300;
    double alpha = 3;
    double beta = 9;
    double evaporationRate = 0.3;
    double initialPheromones = 10;
    unsigned repetitionThreshold = 50;
    double q = 0.6;
    double Q = 3;
};

class TspSolverAco
{
public:
    TspSolverAco(Tsp arg_tsp, AcoParameters arg_aco_params, size_t seed = std::random_device {}());
    TspSolverAco(TspSolverAco&& other) = default;
    TspSolverAco(const TspSolverAco& other) = default;
    TspSolverAco& operator=(TspSolverAco&& other) = default;
    TspSolverAco& operator=(const TspSolverAco& other) = default;
    Solution CalculateSolution(size_t thread_size);
private:
    Tsp tsp;
    AcoParameters params;
    std::vector< std::vector<double> > distance;
    std::vector< std::vector<double> > pheromones;
    std::mt19937_64 generator;
    std::uniform_int_distribution<size_t> int_distribution;
    std::uniform_real_distribution<double> real_distribution;
    size_t getRandomIndex();
    double getProbability();
    void calculateDistance();
    Solution antsRun();
};

