#include "montecarlo/Engine.h"
#include <chrono>
#include <mutex>

namespace montecarlo {

void Engine::runSimulations(
    const Option& option,
    const StochasticProcess& process,
    double S0,
    double r,
    double T,
    size_t numPaths,
    size_t numSteps,
    unsigned int seed,
    Statistics& stats
) {
    RandomGenerator rng(seed);

    for (size_t i = 0; i < numPaths; ++i) {
        double payoffValue;

        if (option.isPathDependent()) {
            // Generate full path for path-dependent options
            auto path = process.generatePath(S0, T, numSteps, rng);
            payoffValue = option.payoff(path);
        } else {
            // Generate only terminal value for European-style options
            double terminal = process.generateTerminal(S0, T, rng);
            payoffValue = option.payoff(terminal);
        }

        // Discount payoff to present value
        double discountedPayoff = payoffValue * std::exp(-r * T);
        stats.add(discountedPayoff);
    }
}

SimulationResult Engine::priceOption(
    const Option& option,
    const StochasticProcess& process,
    double S0,
    double r,
    double T,
    size_t numPaths,
    size_t numSteps
) {
    auto startTime = std::chrono::high_resolution_clock::now();

    // Divide work among threads
    size_t pathsPerThread = numPaths / numThreads_;
    size_t remainderPaths = numPaths % numThreads_;

    std::vector<std::thread> threads;
    std::vector<Statistics> threadStats(numThreads_);

    // Launch worker threads
    for (size_t i = 0; i < numThreads_; ++i) {
        size_t threadPaths = pathsPerThread + (i < remainderPaths ? 1 : 0);
        unsigned int seed = std::random_device{}() + i;

        threads.emplace_back(
            &Engine::runSimulations,
            this,
            std::ref(option),
            std::ref(process),
            S0, r, T,
            threadPaths,
            numSteps,
            seed,
            std::ref(threadStats[i])
        );
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Combine results from all threads
    Statistics combinedStats;
    for (const auto& stats : threadStats) {
        for (size_t i = 0; i < stats.count(); ++i) {
            // Note: This is a simplified combination. For exact stats,
            // we would need to track individual values or use a different approach.
            // For now, we'll merge the statistics properly.
        }
    }

    // Better approach: aggregate means weighted by counts
    double totalSum = 0.0;
    double totalSumSquared = 0.0;
    size_t totalCount = 0;

    for (const auto& stats : threadStats) {
        if (stats.count() > 0) {
            double mean = stats.mean();
            double var = stats.variance();
            size_t count = stats.count();

            totalSum += mean * count;
            totalSumSquared += (var + mean * mean) * count;
            totalCount += count;
        }
    }

    Statistics finalStats;
    double finalMean = totalSum / totalCount;
    double finalVariance = (totalSumSquared / totalCount) - (finalMean * finalMean);
    double finalStdDev = std::sqrt(finalVariance);
    double standardError = finalStdDev / std::sqrt(totalCount);
    double margin = 1.96 * standardError;

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    return SimulationResult{
        finalMean,
        standardError,
        finalMean - margin,
        finalMean + margin,
        totalCount,
        elapsed.count()
    };
}

} // namespace montecarlo
