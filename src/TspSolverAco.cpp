#include "TspSolverAco.hpp"
#include <random>
#include <functional>
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;
TspSolverAco::TspSolverAco(Tsp arg_tsp, AcoParameters arg_aco_params, size_t seed)
    :
    tsp(arg_tsp),
    params(arg_aco_params),
    distance(arg_tsp.Size(),std::vector<double>(arg_tsp.Size(),0)),
    pheromones(arg_tsp.Size(), std::vector<double>(arg_tsp.Size(), arg_aco_params.initialPheromones)),
    generator(seed),
    int_distribution(0, arg_tsp.Size() - 1),
    real_distribution(0,1)
{
    calculateDistance();
}

size_t TspSolverAco::getRandomIndex()
{
    return int_distribution(generator);
}

double TspSolverAco::getProbability()
{
    return real_distribution(generator);
}

void TspSolverAco::calculateDistance()
{
    for (size_t i = 0; i < distance.size(); i++) {
        for (size_t j = 0; j < i; j++) {
            distance[i][j] = distance[j][i] = Point2D::Dist(tsp[i], tsp[j]);
        }
    }
}

Solution TspSolverAco::CalculateSolution(size_t thread_size)
{
    using namespace std;

    Solution previousPath, bestPath;
    bestPath = previousPath = antsRun();
    size_t repetitions = 0;

    #pragma omp parallel for num_threads(thread_size)
    for (size_t iter = 1; iter < params.iterations; iter++) {
        Solution path = antsRun();
        //check circular equal
        #pragma omp critical
        {
            auto start_iter = find(begin(previousPath.path), end(previousPath.path), path.path[0]);
            rotate(begin(previousPath.path), start_iter, end(previousPath.path));

            if (previousPath.path == path.path) {
                repetitions++;

                if (repetitions == params.repetitionThreshold) {
                    for (auto& line : pheromones) {
                        for (auto& onePheromone : line) {
                            onePheromone = params.initialPheromones;
                        }
                    }
                } else {
                    for (auto& line : pheromones) {
                        for (auto& onePheromone : line) {
                            onePheromone *= (1 - params.evaporationRate);
                        }
                    }
                }
            } else {
                repetitions = 0;

                if (path.cost < bestPath.cost) {
                    bestPath = path;
                }

                for (auto& line : pheromones) {
                    for (auto& onePheromone : line) {
                        onePheromone *= (1 - params.evaporationRate);
                    }
                }
            }
        }
    }

    return bestPath;
}

Solution TspSolverAco::antsRun()
{
    using namespace std;

    vector<vector<size_t>> antPath(params.numAnts, vector<size_t>(tsp.Size(), 0));
    vector<double> antCost(params.numAnts, 0);


    for (size_t i = 0; i < params.numAnts; i++) {
        vector<bool> city_visited(tsp.Size(), false);
        auto city_intex = getRandomIndex();
        city_visited[city_intex] = true;
        antPath[i][0] = city_intex;

        for (size_t j = 0; j < tsp.Size(); j++) {
            vector<double> selectionValue(tsp.Size() - j, 0.0);
            vector<double> roulette(tsp.Size() - j, 0.0);
            vector<size_t> indices(tsp.Size() - j, 0);
            double totalSelectionValue = 0;
            size_t selectedCity;

            for (size_t z = 0, count = 0; z < tsp.Size() && count < tsp.Size() - j; z++) {
                if (!city_visited[z]) {
                    indices[count] = z;
                    double tau = pow(pheromones[city_intex][z], params.alpha);
                    double nu = pow(1/distance[city_intex][z], params.beta);
                    selectionValue[count] = tau*nu;
                    totalSelectionValue += selectionValue[count];
                    roulette[count] = (count == 0 ? selectionValue[count] : roulette[count - 1] + selectionValue[count]);
                    count++;
                }
            }

            if (getProbability() <= params.q) {
                auto selection = getProbability() * totalSelectionValue;
                auto selected_iter = find_if(begin(roulette), end(roulette), [selection](double value) {
                    return selection <= value;
                });
                selectedCity = indices[selected_iter - begin(roulette)];
            } else {
                auto selected_iter = max_element(begin(selectionValue), end(selectionValue));
                selectedCity = indices[selected_iter - begin(selectionValue)];
            }

            city_visited[selectedCity] = true;
            antPath[i][j] = selectedCity;
            antCost[i] += distance[city_intex][selectedCity];
            city_intex = selectedCity;
        }

        antCost[i] += distance[city_intex][antPath[i][0]];
    }

    for (size_t i = 0; i < params.numAnts; i++) {
        double pheromonesAmount = params.Q / antCost[i];

        for (size_t j = 0; j < tsp.Size() - 1; j++) {
            size_t a = antPath[i][j];
            size_t b = antPath[i][j + 1];
            double newPheromones = pheromones[a][b] + pheromonesAmount;
            pheromones[a][b] = pheromones[b][a] = newPheromones;
        }

        size_t a = antPath[i][0];
        size_t b = antPath[i][tsp.Size()-1];
        double newPheromones = pheromones[a][b] + pheromonesAmount;
        pheromones[a][b] = pheromones[b][a] = newPheromones;
    }

    auto min_iter = min_element(begin(antCost), end(antCost));
    auto best_index = min_iter - begin(antCost);

    Solution best;
    best.path = antPath[best_index];
    best.cost = antCost[best_index];

    return best;
}
