#pragma once

#include "RandomGenerator.h"
#include <vector>
#include <cmath>

namespace montecarlo {

// Base class for stochastic processes
class StochasticProcess {
public:
    virtual ~StochasticProcess() = default;

    // Generate a single path
    virtual std::vector<double> generatePath(
        double S0,          // Initial value
        double T,           // Time to maturity
        size_t steps,       // Number of time steps
        RandomGenerator& rng
    ) const = 0;

    // Generate terminal value only (more efficient for European options)
    virtual double generateTerminal(
        double S0,
        double T,
        RandomGenerator& rng
    ) const = 0;
};

// Geometric Brownian Motion (Black-Scholes model)
class GeometricBrownianMotion : public StochasticProcess {
public:
    GeometricBrownianMotion(double mu, double sigma)
        : mu_(mu), sigma_(sigma) {}

    std::vector<double> generatePath(
        double S0,
        double T,
        size_t steps,
        RandomGenerator& rng
    ) const override {
        std::vector<double> path(steps + 1);
        path[0] = S0;

        double dt = T / steps;
        double drift = (mu_ - 0.5 * sigma_ * sigma_) * dt;
        double diffusion = sigma_ * std::sqrt(dt);

        for (size_t i = 1; i <= steps; ++i) {
            double Z = rng.getNormal();
            path[i] = path[i-1] * std::exp(drift + diffusion * Z);
        }

        return path;
    }

    double generateTerminal(
        double S0,
        double T,
        RandomGenerator& rng
    ) const override {
        double Z = rng.getNormal();
        double drift = (mu_ - 0.5 * sigma_ * sigma_) * T;
        double diffusion = sigma_ * std::sqrt(T);
        return S0 * std::exp(drift + diffusion * Z);
    }

private:
    double mu_;     // Drift (risk-free rate for risk-neutral pricing)
    double sigma_;  // Volatility
};

} // namespace montecarlo
