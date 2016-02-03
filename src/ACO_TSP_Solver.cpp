// ACO_TSP_Solver.cpp : Defines the entry point for the console application.
//

#include "Tsp.hpp"
#include "TspSolverAco.hpp"

int main(int argc,char* argv[])
{
    auto tsp = Tsp::loadTspFile(argv[1]);
    auto params = AcoParameters();
    auto tsp_solver_aco = TspSolverAco(tsp, params);
    auto thread_size = (argc == 2 ? 1 : strtol(argv[2], nullptr, 0));
    auto best_solution = tsp_solver_aco.CalculateSolution(thread_size);
    return 0;
}

