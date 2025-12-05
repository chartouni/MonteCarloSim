#include "montecarlo/Engine.h"
#include "montecarlo/Option.h"
#include "montecarlo/StochasticProcess.h"
#include <iostream>
#include <iomanip>

int main() {
    using namespace montecarlo;

    std::cout << "=== European Option Pricing with Monte Carlo ===" << std::endl;
    std::cout << std::endl;

    // Market parameters
    double S0 = 100.0;      // Current stock price
    double K = 100.0;       // Strike price
    double r = 0.05;        // Risk-free rate (5%)
    double sigma = 0.2;     // Volatility (20%)
    double T = 1.0;         // Time to maturity (1 year)

    // Simulation parameters
    size_t numPaths = 1000000;  // 1 million simulations

    std::cout << "Market Parameters:" << std::endl;
    std::cout << "  Spot Price (S0):    $" << S0 << std::endl;
    std::cout << "  Strike Price (K):   $" << K << std::endl;
    std::cout << "  Risk-free rate (r): " << (r * 100) << "%" << std::endl;
    std::cout << "  Volatility (σ):     " << (sigma * 100) << "%" << std::endl;
    std::cout << "  Time to Maturity:   " << T << " years" << std::endl;
    std::cout << std::endl;

    // Create Monte Carlo engine
    Engine engine;  // Uses all available CPU cores

    // Create stochastic process (Geometric Brownian Motion)
    GeometricBrownianMotion gbm(r, sigma);

    // Price European Call Option
    std::cout << "Pricing European Call Option..." << std::endl;
    EuropeanOption callOption(OptionType::Call, K);
    auto callResult = engine.priceOption(callOption, gbm, S0, r, T, numPaths);

    std::cout << "  Price:              $" << std::fixed << std::setprecision(4)
              << callResult.price << std::endl;
    std::cout << "  Standard Error:     $" << callResult.standardError << std::endl;
    std::cout << "  95% CI:             [$" << callResult.confidenceLower
              << ", $" << callResult.confidenceUpper << "]" << std::endl;
    std::cout << "  Simulations:        " << callResult.numSimulations << std::endl;
    std::cout << "  Time:               " << std::setprecision(3)
              << callResult.elapsedSeconds << " seconds" << std::endl;
    std::cout << std::endl;

    // Price European Put Option
    std::cout << "Pricing European Put Option..." << std::endl;
    EuropeanOption putOption(OptionType::Put, K);
    auto putResult = engine.priceOption(putOption, gbm, S0, r, T, numPaths);

    std::cout << "  Price:              $" << std::fixed << std::setprecision(4)
              << putResult.price << std::endl;
    std::cout << "  Standard Error:     $" << putResult.standardError << std::endl;
    std::cout << "  95% CI:             [$" << putResult.confidenceLower
              << ", $" << putResult.confidenceUpper << "]" << std::endl;
    std::cout << "  Simulations:        " << putResult.numSimulations << std::endl;
    std::cout << "  Time:               " << std::setprecision(3)
              << putResult.elapsedSeconds << " seconds" << std::endl;
    std::cout << std::endl;

    // Black-Scholes analytical solution for comparison
    auto blackScholesCall = [](double S, double K, double r, double sigma, double T) {
        double d1 = (std::log(S/K) + (r + 0.5*sigma*sigma)*T) / (sigma*std::sqrt(T));
        double d2 = d1 - sigma*std::sqrt(T);
        auto N = [](double x) {
            return 0.5 * std::erfc(-x * M_SQRT1_2);
        };
        return S * N(d1) - K * std::exp(-r*T) * N(d2);
    };

    double bsCallPrice = blackScholesCall(S0, K, r, sigma, T);
    std::cout << "Black-Scholes Call Price (analytical): $" << std::fixed
              << std::setprecision(4) << bsCallPrice << std::endl;
    std::cout << "Monte Carlo Error: $" << std::abs(callResult.price - bsCallPrice)
              << " (" << std::setprecision(2)
              << (std::abs(callResult.price - bsCallPrice) / bsCallPrice * 100)
              << "%)" << std::endl;

    return 0;
}
