#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

namespace montecarlo {

enum class OptionType {
    Call,
    Put
};

// Base class for option payoffs
class Option {
public:
    virtual ~Option() = default;

    // Calculate payoff from a price path
    virtual double payoff(const std::vector<double>& path) const = 0;

    // Calculate payoff from terminal price (for path-independent options)
    virtual double payoff(double terminal) const = 0;

    virtual bool isPathDependent() const = 0;
};

// European option (path-independent)
class EuropeanOption : public Option {
public:
    EuropeanOption(OptionType type, double strike)
        : type_(type), strike_(strike) {}

    double payoff(const std::vector<double>& path) const override {
        return payoff(path.back());
    }

    double payoff(double terminal) const override {
        if (type_ == OptionType::Call) {
            return std::max(terminal - strike_, 0.0);
        } else {
            return std::max(strike_ - terminal, 0.0);
        }
    }

    bool isPathDependent() const override { return false; }

private:
    OptionType type_;
    double strike_;
};

// Asian option (path-dependent, uses arithmetic average)
class AsianOption : public Option {
public:
    AsianOption(OptionType type, double strike)
        : type_(type), strike_(strike) {}

    double payoff(const std::vector<double>& path) const override {
        double average = std::accumulate(path.begin(), path.end(), 0.0) / path.size();

        if (type_ == OptionType::Call) {
            return std::max(average - strike_, 0.0);
        } else {
            return std::max(strike_ - average, 0.0);
        }
    }

    double payoff(double terminal) const override {
        // Cannot calculate Asian option payoff from terminal value only
        return payoff(std::vector<double>{terminal});
    }

    bool isPathDependent() const override { return true; }

private:
    OptionType type_;
    double strike_;
};

} // namespace montecarlo
