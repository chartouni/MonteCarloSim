#pragma once

#include "Option.h"
#include "StochasticProcess.h"
#include "RandomGenerator.h"
#include "Statistics.h"
#include <thread>
#include <vector>
#include <future>
#include <functional>

namespace montecarlo {

struct SimulationResult {
    double price;
    double standardError;
    double confidenceLower;
    double confidenceUpper;
    size_t numSimulations;
    double elapsedSeconds;
};

class Engine {
public:
    Engine(size_t numThreads = 0)
        : numThreads_(numThreads > 0 ? numThreads : std::thread::hardware_concurrency()) {}

    // Price an option using Monte Carlo simulation
    SimulationResult priceOption(
        const Option& option,
        const StochasticProcess& process,
        double S0,              // Initial stock price
        double r,               // Risk-free rate
        double T,               // Time to maturity
        size_t numPaths,        // Number of Monte Carlo paths
        size_t numSteps = 100   // Time steps per path (for path-dependent options)
    );

private:
    size_t numThreads_;

    // Worker function for each thread
    void runSimulations(
        const Option& option,
        const StochasticProcess& process,
        double S0,
        double r,
        double T,
        size_t numPaths,
        size_t numSteps,
        unsigned int seed,
        Statistics& stats
    );
};

} // namespace montecarlo
