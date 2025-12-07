#pragma once

#include "RandomGenerator.h"
#include "Statistics.h"
#include <thread>
#include <vector>
#include <functional>

namespace montecarlo {

struct SimulationResult {
    double mean;
    double standardError;
    double confidenceLower;
    double confidenceUpper;
    size_t numSimulations;
    double elapsedSeconds;
};

// Type alias for simulation functions
// Function takes a RandomGenerator and returns a double result
using SimulationFunc = std::function<double(RandomGenerator&)>;

class Engine {
public:
    Engine(size_t numThreads = 0)
        : numThreads_(numThreads > 0 ? numThreads : std::thread::hardware_concurrency()) {}

    // Run a Monte Carlo simulation with a user-defined function
    SimulationResult run(
        SimulationFunc simulation,  // User's simulation function
        size_t numSamples          // Number of Monte Carlo samples
    );

    size_t getNumThreads() const { return numThreads_; }

private:
    size_t numThreads_;

    // Worker function for each thread
    void runSimulations(
        SimulationFunc simulation,
        size_t numSamples,
        unsigned int seed,
        Statistics& stats
    );
};

} // namespace montecarlo
