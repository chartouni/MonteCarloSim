#pragma once

#include <random>
#include <memory>

namespace montecarlo {

class RandomGenerator {
public:
    explicit RandomGenerator(unsigned int seed = std::random_device{}())
        : generator_(seed), normalDist_(0.0, 1.0), uniformDist_(0.0, 1.0) {}

    // Generate standard normal random variable
    double getNormal() {
        return normalDist_(generator_);
    }

    // Generate uniform random variable in [0, 1)
    double getUniform() {
        return uniformDist_(generator_);
    }

    // Generate normal with custom mean and std dev
    double getNormal(double mean, double stddev) {
        return mean + stddev * getNormal();
    }

    void setSeed(unsigned int seed) {
        generator_.seed(seed);
    }

private:
    std::mt19937_64 generator_;
    std::normal_distribution<double> normalDist_;
    std::uniform_real_distribution<double> uniformDist_;
};

} // namespace montecarlo
