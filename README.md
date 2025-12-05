# Monte Carlo Simulation System for Financial Derivatives

A high-performance C++ library for pricing financial derivatives using Monte Carlo simulation with multithreading support.

## Features

- **Multi-threaded Monte Carlo Engine**: Automatically utilizes all available CPU cores
- **Stochastic Processes**: Geometric Brownian Motion (Black-Scholes model)
- **Financial Instruments**:
  - European Options (Call/Put)
  - Asian Options (Arithmetic Average)
- **Statistical Analysis**: Mean, variance, standard error, and 95% confidence intervals
- **High-Quality RNG**: Mersenne Twister (MT19937-64) with normal and uniform distributions

## Project Structure

```
MonteCarloSim/
├── include/montecarlo/     # Header files
│   ├── Engine.h           # Monte Carlo engine
│   ├── Option.h           # Option payoff definitions
│   ├── StochasticProcess.h # Stochastic processes (GBM)
│   ├── RandomGenerator.h   # Random number generation
│   └── Statistics.h        # Statistical calculations
├── src/                    # Implementation files
├── examples/               # Example programs
│   ├── european_option.cpp # European option pricing
│   └── asian_option.cpp    # Asian option pricing
└── CMakeLists.txt          # Build configuration
```

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14 or higher
- POSIX threads (included in most systems)

## Building

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build . -j

# The executables will be in the build directory
```

## Usage

### European Option Example

```cpp
#include "montecarlo/Engine.h"
#include "montecarlo/Option.h"
#include "montecarlo/StochasticProcess.h"

using namespace montecarlo;

// Market parameters
double S0 = 100.0;      // Spot price
double K = 100.0;       // Strike
double r = 0.05;        // Risk-free rate
double sigma = 0.2;     // Volatility
double T = 1.0;         // Time to maturity

// Create engine and process
Engine engine;
GeometricBrownianMotion gbm(r, sigma);

// Price a European call
EuropeanOption call(OptionType::Call, K);
auto result = engine.priceOption(call, gbm, S0, r, T, 1000000);

std::cout << "Option Price: $" << result.price << std::endl;
std::cout << "Standard Error: $" << result.standardError << std::endl;
```

### Running Examples

```bash
# European option pricing
./european_option

# Asian option pricing
./asian_option
```

## Performance

The system automatically scales to use all available CPU cores. Typical performance on a modern laptop:

- **European Options**: ~1M simulations in < 0.5 seconds
- **Asian Options**: ~500K simulations (252 steps) in ~2-3 seconds

Performance scales linearly with the number of CPU cores.

## Technical Details

### Monte Carlo Method

The system uses Monte Carlo simulation to price options by:
1. Simulating many random price paths using stochastic processes
2. Calculating the payoff for each path
3. Discounting payoffs to present value
4. Taking the average as the option price

### Variance Reduction

For European options, the system optimizes by generating only terminal values instead of full paths, significantly improving performance.

### Random Number Generation

- Uses Mersenne Twister (MT19937-64) for high-quality randomness
- Box-Muller transformation for normal distributions
- Each thread uses an independent RNG with unique seed

### Statistical Accuracy

Results include:
- Mean (option price)
- Standard error
- 95% confidence interval
- Number of simulations
- Computation time

## Extending the System

### Adding New Options

Inherit from the `Option` base class and implement:
```cpp
class MyOption : public Option {
    double payoff(const std::vector<double>& path) const override;
    double payoff(double terminal) const override;
    bool isPathDependent() const override;
};
```

### Adding New Stochastic Processes

Inherit from `StochasticProcess`:
```cpp
class MyProcess : public StochasticProcess {
    std::vector<double> generatePath(...) const override;
    double generateTerminal(...) const override;
};
```

## Limitations

- Currently supports only arithmetic average Asian options (not geometric)
- No variance reduction techniques (antithetic variates, control variates)
- No support for multi-asset options yet
- Risk-neutral pricing framework only

## Future Enhancements

- Additional option types (Barrier, Lookback, Digital)
- More stochastic processes (Heston, Mean-Reverting)
- Variance reduction techniques
- Jump-diffusion models
- Multi-asset correlation

## License

This is an educational/research project. Feel free to use and modify as needed.

## References

- Hull, J. C. (2018). *Options, Futures, and Other Derivatives*
- Glasserman, P. (2003). *Monte Carlo Methods in Financial Engineering*
