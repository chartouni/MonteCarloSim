#include "montecarlo/Engine.h"
#include <chrono>

namespace montecarlo {

void Engine::runSimulations(
    SimulationFunc simulation,
    size_t numSamples,
    unsigned int seed,
    Statistics& stats
) {
    RandomGenerator rng(seed);

    for (size_t i = 0; i < numSamples; ++i) {
        double result = simulation(rng);
        stats.add(result);
    }
}

SimulationResult Engine::run(
    SimulationFunc simulation,
    size_t numSamples
) {
    auto startTime = std::chrono::high_resolution_clock::now();

    // Divide work among threads
    size_t samplesPerThread = numSamples / numThreads_;
    size_t remainderSamples = numSamples % numThreads_;

    std::vector<std::thread> threads;
    std::vector<Statistics> threadStats(numThreads_);

    // Launch worker threads
    for (size_t i = 0; i < numThreads_; ++i) {
        size_t threadSamples = samplesPerThread + (i < remainderSamples ? 1 : 0);
        unsigned int seed = std::random_device{}() + i;

        threads.emplace_back(
            &Engine::runSimulations,
            this,
            simulation,
            threadSamples,
            seed,
            std::ref(threadStats[i])
        );
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Combine results from all threads
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
