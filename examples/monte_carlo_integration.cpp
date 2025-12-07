#include "montecarlo/Engine.h"
#include <iostream>
#include <iomanip>
#include <cmath>

// Example functions to integrate
namespace functions {
    // f(x) = x² on [0, 1], integral = 1/3
    double square(double x) {
        return x * x;
    }

    // f(x) = sin(x) on [0, π], integral = 2
    double sine(double x) {
        return std::sin(x);
    }

    // f(x) = e^(-x²) on [0, 1], integral ≈ 0.746824
    double gaussian(double x) {
        return std::exp(-x * x);
    }

    // Multidimensional: f(x,y) = x² + y² on [0,1]x[0,1], integral = 2/3
    double multivariate(double x, double y) {
        return x * x + y * y;
    }
}

int main() {
    using namespace montecarlo;

    std::cout << "=== Monte Carlo Integration ===" << std::endl;
    std::cout << std::endl;

    Engine engine;
    std::cout << "Using " << engine.getNumThreads() << " threads" << std::endl;
    std::cout << std::endl;

    size_t numSamples = 1000000;

    // Example 1: Integrate x² from 0 to 1
    std::cout << "Example 1: ∫₀¹ x² dx" << std::endl;
    std::cout << "Expected result: 1/3 ≈ 0.333333" << std::endl;
    {
        double a = 0.0, b = 1.0;  // Integration bounds
        auto integrand = [a, b](RandomGenerator& rng) -> double {
            double x = rng.getUniform() * (b - a) + a;
            return functions::square(x) * (b - a);  // Scale by interval width
        };

        auto result = engine.run(integrand, numSamples);
        double expected = 1.0 / 3.0;
        double error = std::abs(result.mean - expected);

        std::cout << "  Estimate:       " << std::fixed << std::setprecision(6) << result.mean << std::endl;
        std::cout << "  Error:          " << error
                  << " (" << std::setprecision(4) << (error/expected*100) << "%)" << std::endl;
        std::cout << "  95% CI:         [" << std::setprecision(6)
                  << result.confidenceLower << ", " << result.confidenceUpper << "]" << std::endl;
        std::cout << "  Time:           " << std::setprecision(4)
                  << result.elapsedSeconds << " seconds" << std::endl;
    }
    std::cout << std::endl;

    // Example 2: Integrate sin(x) from 0 to π
    std::cout << "Example 2: ∫₀^π sin(x) dx" << std::endl;
    std::cout << "Expected result: 2.0" << std::endl;
    {
        double a = 0.0, b = M_PI;
        auto integrand = [a, b](RandomGenerator& rng) -> double {
            double x = rng.getUniform() * (b - a) + a;
            return functions::sine(x) * (b - a);
        };

        auto result = engine.run(integrand, numSamples);
        double expected = 2.0;
        double error = std::abs(result.mean - expected);

        std::cout << "  Estimate:       " << std::fixed << std::setprecision(6) << result.mean << std::endl;
        std::cout << "  Error:          " << error
                  << " (" << std::setprecision(4) << (error/expected*100) << "%)" << std::endl;
        std::cout << "  95% CI:         [" << std::setprecision(6)
                  << result.confidenceLower << ", " << result.confidenceUpper << "]" << std::endl;
        std::cout << "  Time:           " << std::setprecision(4)
                  << result.elapsedSeconds << " seconds" << std::endl;
    }
    std::cout << std::endl;

    // Example 3: Integrate e^(-x²) from 0 to 1
    std::cout << "Example 3: ∫₀¹ e^(-x²) dx" << std::endl;
    std::cout << "Expected result: ≈ 0.746824" << std::endl;
    {
        double a = 0.0, b = 1.0;
        auto integrand = [a, b](RandomGenerator& rng) -> double {
            double x = rng.getUniform() * (b - a) + a;
            return functions::gaussian(x) * (b - a);
        };

        auto result = engine.run(integrand, numSamples);
        double expected = 0.746824;  // Numerical value
        double error = std::abs(result.mean - expected);

        std::cout << "  Estimate:       " << std::fixed << std::setprecision(6) << result.mean << std::endl;
        std::cout << "  Error:          " << error
                  << " (" << std::setprecision(4) << (error/expected*100) << "%)" << std::endl;
        std::cout << "  95% CI:         [" << std::setprecision(6)
                  << result.confidenceLower << ", " << result.confidenceUpper << "]" << std::endl;
        std::cout << "  Time:           " << std::setprecision(4)
                  << result.elapsedSeconds << " seconds" << std::endl;
    }
    std::cout << std::endl;

    // Example 4: 2D integration - ∫∫ (x² + y²) dx dy over [0,1]×[0,1]
    std::cout << "Example 4: ∫₀¹ ∫₀¹ (x² + y²) dx dy" << std::endl;
    std::cout << "Expected result: 2/3 ≈ 0.666667" << std::endl;
    {
        auto integrand = [](RandomGenerator& rng) -> double {
            double x = rng.getUniform();
            double y = rng.getUniform();
            return functions::multivariate(x, y);  // Area is 1×1 = 1
        };

        auto result = engine.run(integrand, numSamples);
        double expected = 2.0 / 3.0;
        double error = std::abs(result.mean - expected);

        std::cout << "  Estimate:       " << std::fixed << std::setprecision(6) << result.mean << std::endl;
        std::cout << "  Error:          " << error
                  << " (" << std::setprecision(4) << (error/expected*100) << "%)" << std::endl;
        std::cout << "  95% CI:         [" << std::setprecision(6)
                  << result.confidenceLower << ", " << result.confidenceUpper << "]" << std::endl;
        std::cout << "  Time:           " << std::setprecision(4)
                  << result.elapsedSeconds << " seconds" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Note: Monte Carlo integration shines for high-dimensional integrals" << std::endl;
    std::cout << "      where traditional methods become computationally expensive." << std::endl;

    return 0;
}
