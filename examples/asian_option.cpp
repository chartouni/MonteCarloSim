#include "montecarlo/Engine.h"
#include "montecarlo/Option.h"
#include "montecarlo/StochasticProcess.h"
#include <iostream>
#include <iomanip>

int main() {
    using namespace montecarlo;

    std::cout << "=== Asian Option Pricing with Monte Carlo ===" << std::endl;
    std::cout << std::endl;

    // Market parameters
    double S0 = 100.0;      // Current stock price
    double K = 100.0;       // Strike price
    double r = 0.05;        // Risk-free rate (5%)
    double sigma = 0.3;     // Volatility (30%)
    double T = 1.0;         // Time to maturity (1 year)

    // Simulation parameters
    size_t numPaths = 500000;   // 500k simulations
    size_t numSteps = 252;      // Daily observations (trading days per year)

    std::cout << "Market Parameters:" << std::endl;
    std::cout << "  Spot Price (S0):    $" << S0 << std::endl;
    std::cout << "  Strike Price (K):   $" << K << std::endl;
    std::cout << "  Risk-free rate (r): " << (r * 100) << "%" << std::endl;
    std::cout << "  Volatility (σ):     " << (sigma * 100) << "%" << std::endl;
    std::cout << "  Time to Maturity:   " << T << " years" << std::endl;
    std::cout << "  Observation Steps:  " << numSteps << " (daily)" << std::endl;
    std::cout << std::endl;

    // Create Monte Carlo engine
    Engine engine;

    // Create stochastic process (Geometric Brownian Motion)
    GeometricBrownianMotion gbm(r, sigma);

    // Price Asian Call Option (arithmetic average)
    std::cout << "Pricing Asian Call Option (Arithmetic Average)..." << std::endl;
    AsianOption asianCall(OptionType::Call, K);
    auto callResult = engine.priceOption(asianCall, gbm, S0, r, T, numPaths, numSteps);

    std::cout << "  Price:              $" << std::fixed << std::setprecision(4)
              << callResult.price << std::endl;
    std::cout << "  Standard Error:     $" << callResult.standardError << std::endl;
    std::cout << "  95% CI:             [$" << callResult.confidenceLower
              << ", $" << callResult.confidenceUpper << "]" << std::endl;
    std::cout << "  Simulations:        " << callResult.numSimulations << std::endl;
    std::cout << "  Time:               " << std::setprecision(3)
              << callResult.elapsedSeconds << " seconds" << std::endl;
    std::cout << std::endl;

    // Price Asian Put Option (arithmetic average)
    std::cout << "Pricing Asian Put Option (Arithmetic Average)..." << std::endl;
    AsianOption asianPut(OptionType::Put, K);
    auto putResult = engine.priceOption(asianPut, gbm, S0, r, T, numPaths, numSteps);

    std::cout << "  Price:              $" << std::fixed << std::setprecision(4)
              << putResult.price << std::endl;
    std::cout << "  Standard Error:     $" << putResult.standardError << std::endl;
    std::cout << "  95% CI:             [$" << putResult.confidenceLower
              << ", $" << putResult.confidenceUpper << "]" << std::endl;
    std::cout << "  Simulations:        " << putResult.numSimulations << std::endl;
    std::cout << "  Time:               " << std::setprecision(3)
              << putResult.elapsedSeconds << " seconds" << std::endl;
    std::cout << std::endl;

    std::cout << "Note: Asian options are path-dependent and average the stock price" << std::endl;
    std::cout << "      over the option's lifetime, making them less volatile than" << std::endl;
    std::cout << "      European options and typically cheaper." << std::endl;

    return 0;
}
