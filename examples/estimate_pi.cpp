#include "montecarlo/Engine.h"
#include <iostream>
#include <iomanip>
#include <cmath>

int main() {
    using namespace montecarlo;

    std::cout << "=== Estimating π using Monte Carlo Method ===" << std::endl;
    std::cout << std::endl;

    std::cout << "Method: Generate random points in a unit square [0,1] x [0,1]" << std::endl;
    std::cout << "        Count how many fall inside the quarter circle (x² + y² ≤ 1)" << std::endl;
    std::cout << "        π ≈ 4 × (points inside circle / total points)" << std::endl;
    std::cout << std::endl;

    // Create Monte Carlo engine
    Engine engine;
    std::cout << "Using " << engine.getNumThreads() << " threads" << std::endl;
    std::cout << std::endl;

    // Define the simulation: randomly sample points and check if inside circle
    auto estimatePi = [](RandomGenerator& rng) -> double {
        double x = rng.getUniform();
        double y = rng.getUniform();
        // Return 1 if inside circle, 0 if outside
        return (x * x + y * y <= 1.0) ? 1.0 : 0.0;
    };

    // Run simulations with different sample sizes
    std::vector<size_t> sampleSizes = {10000, 100000, 1000000, 10000000};

    for (size_t numSamples : sampleSizes) {
        auto result = engine.run(estimatePi, numSamples);

        // Mean represents the ratio of points inside the circle
        // Multiply by 4 to get π estimate
        double piEstimate = result.mean * 4.0;
        double piError = 4.0 * result.standardError;
        double piLower = result.confidenceLower * 4.0;
        double piUpper = result.confidenceUpper * 4.0;

        double actualPi = M_PI;
        double absoluteError = std::abs(piEstimate - actualPi);
        double relativeError = (absoluteError / actualPi) * 100.0;

        std::cout << "Samples: " << std::setw(10) << numSamples << std::endl;
        std::cout << "  π estimate:     " << std::fixed << std::setprecision(6) << piEstimate << std::endl;
        std::cout << "  Actual π:       " << actualPi << std::endl;
        std::cout << "  Error:          " << absoluteError
                  << " (" << std::setprecision(4) << relativeError << "%)" << std::endl;
        std::cout << "  95% CI:         [" << std::setprecision(6) << piLower
                  << ", " << piUpper << "]" << std::endl;
        std::cout << "  Time:           " << std::setprecision(4)
                  << result.elapsedSeconds << " seconds" << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Note: Error decreases as O(1/√n) with more samples" << std::endl;
    std::cout << "      To halve the error, you need 4× more samples" << std::endl;

    return 0;
}
