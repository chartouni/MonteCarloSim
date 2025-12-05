#pragma once

#include <vector>
#include <cmath>

namespace montecarlo {

class Statistics {
public:
    Statistics() : sum_(0.0), sumSquared_(0.0), count_(0) {}

    void add(double value) {
        sum_ += value;
        sumSquared_ += value * value;
        ++count_;
    }

    double mean() const {
        return count_ > 0 ? sum_ / count_ : 0.0;
    }

    double variance() const {
        if (count_ < 2) return 0.0;
        double m = mean();
        return (sumSquared_ / count_) - (m * m);
    }

    double stddev() const {
        return std::sqrt(variance());
    }

    double standardError() const {
        return count_ > 0 ? stddev() / std::sqrt(count_) : 0.0;
    }

    // 95% confidence interval
    std::pair<double, double> confidenceInterval() const {
        double m = mean();
        double se = standardError();
        double margin = 1.96 * se;  // 95% CI
        return {m - margin, m + margin};
    }

    size_t count() const { return count_; }

    void reset() {
        sum_ = 0.0;
        sumSquared_ = 0.0;
        count_ = 0;
    }

private:
    double sum_;
    double sumSquared_;
    size_t count_;
};

} // namespace montecarlo
