# Monte Carlo Simulation System

A high-performance, general-purpose C++ library for Monte Carlo simulations with multithreading support.

## Features

- **Multi-threaded Engine**: Automatically utilizes all available CPU cores
- **General-Purpose Framework**: Solve any problem with a user-defined simulation function
- **High-Quality RNG**: Mersenne Twister (MT19937-64) with normal and uniform distributions
- **Statistical Analysis**: Mean, variance, standard error, and 95% confidence intervals
- **Simple API**: Define your simulation as a lambda or function

## What is Monte Carlo Simulation?

Monte Carlo methods use random sampling to obtain numerical results for problems that might be deterministic in principle. They're especially useful for:

- **Numerical Integration**: Estimate definite integrals, especially in high dimensions
- **Probability Estimation**: Calculate probabilities of complex events
- **Optimization**: Find approximate solutions to optimization problems
- **Physical Simulations**: Model systems with random behavior

## Project Structure

```
MonteCarloSim/
├── include/montecarlo/     # Header files
│   ├── Engine.h           # Monte Carlo engine
│   ├── RandomGenerator.h   # Random number generation
│   └── Statistics.h        # Statistical calculations
├── src/                    # Implementation files
├── examples/               # Example programs
│   ├── estimate_pi.cpp     # Estimate π using random sampling
│   └── monte_carlo_integration.cpp  # Numerical integration
└── CMakeLists.txt          # Build configuration
```

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+, MinGW)
- CMake 3.14 or higher (optional - see manual compilation)
- POSIX threads (included in most systems)

## Building

### With CMake

```bash
mkdir build && cd build
cmake ..
cmake --build . -j
```

### Manual Compilation (MinGW/GCC)

```bash
# Estimate π example
g++ -std=c++17 -O3 -pthread -I./include src/*.cpp examples/estimate_pi.cpp -o estimate_pi

# Integration example
g++ -std=c++17 -O3 -pthread -I./include src/*.cpp examples/monte_carlo_integration.cpp -o monte_carlo_integration
```

### Windows (PowerShell + MinGW)

```powershell
# Estimate π example
g++ -std=c++17 -O3 -pthread -I./include src/*.cpp examples/estimate_pi.cpp -o estimate_pi.exe

# Integration example
g++ -std=c++17 -O3 -pthread -I./include src/*.cpp examples/monte_carlo_integration.cpp -o monte_carlo_integration.exe
```

## Quick Start

### Example 1: Estimating π

```cpp
#include "montecarlo/Engine.h"

using namespace montecarlo;

int main() {
    Engine engine;

    // Define simulation: random points in unit square
    auto estimatePi = [](RandomGenerator& rng) -> double {
        double x = rng.getUniform();
        double y = rng.getUniform();
        return (x * x + y * y <= 1.0) ? 1.0 : 0.0;
    };

    // Run 1 million samples
    auto result = engine.run(estimatePi, 1000000);

    // π ≈ 4 × (points inside circle / total points)
    double pi = result.mean * 4.0;
    std::cout << "π ≈ " << pi << std::endl;

    return 0;
}
```

### Example 2: Monte Carlo Integration

Integrate f(x) = x² from 0 to 1:

```cpp
#include "montecarlo/Engine.h"

using namespace montecarlo;

int main() {
    Engine engine;

    double a = 0.0, b = 1.0;  // Integration bounds

    auto integrand = [a, b](RandomGenerator& rng) -> double {
        double x = rng.getUniform() * (b - a) + a;
        return x * x * (b - a);  // Scale by interval width
    };

    auto result = engine.run(integrand, 1000000);

    std::cout << "∫₀¹ x² dx ≈ " << result.mean << std::endl;
    std::cout << "Expected: 0.333333" << std::endl;

    return 0;
}
```

## Running Examples

```bash
# Estimate π with increasing sample sizes
./estimate_pi

# Numerical integration examples (1D and 2D)
./monte_carlo_integration
```

## Performance

The system automatically scales to use all available CPU cores. Typical performance on a modern laptop:

- **π Estimation**: 10M samples in ~0.1 seconds
- **Integration**: 1M samples in ~0.02 seconds

Performance scales linearly with the number of CPU cores.

## API Reference

### Engine

```cpp
Engine(size_t numThreads = 0)  // 0 = auto-detect CPU cores
```

Main method:
```cpp
SimulationResult run(SimulationFunc simulation, size_t numSamples)
```

- `simulation`: A function that takes `RandomGenerator&` and returns `double`
- `numSamples`: Number of Monte Carlo samples to generate

### SimulationResult

```cpp
struct SimulationResult {
    double mean;              // Average result
    double standardError;     // Standard error
    double confidenceLower;   // Lower 95% CI bound
    double confidenceUpper;   // Upper 95% CI bound
    size_t numSimulations;    // Total samples run
    double elapsedSeconds;    // Computation time
};
```

### RandomGenerator

```cpp
double getUniform()                    // Uniform in [0, 1)
double getNormal()                     // Standard normal (μ=0, σ=1)
double getNormal(double mean, double stddev)  // Custom normal
```

## Advanced Usage

### Multi-dimensional Integration

```cpp
auto integrand = [](RandomGenerator& rng) -> double {
    double x = rng.getUniform();
    double y = rng.getUniform();
    double z = rng.getUniform();
    // Your function here
    return x*x + y*y + z*z;
};
```

### Custom Distributions

```cpp
auto simulation = [](RandomGenerator& rng) -> double {
    // Exponential distribution
    double lambda = 2.0;
    double u = rng.getUniform();
    double x = -std::log(u) / lambda;

    // Your logic here
    return x;
};
```

### Optimization Problems

Use Monte Carlo for global optimization:

```cpp
auto findMinimum = [](RandomGenerator& rng) -> double {
    // Random search in parameter space
    double x = rng.getUniform() * 10.0 - 5.0;  // [-5, 5]
    double y = rng.getUniform() * 10.0 - 5.0;

    // Function to minimize: f(x,y) = x² + y²
    double value = x*x + y*y;

    // Track minimum (requires shared state in practice)
    return value;
};
```

## Why Monte Carlo?

**Advantages:**
- Works for complex, high-dimensional problems
- Easy to implement and parallelize
- No need for analytical solutions
- Error bounds are straightforward (O(1/√n))

**When to use:**
- High-dimensional integration
- Complex probability calculations
- No closed-form solution available
- Embarrassingly parallel problems

**Trade-offs:**
- Slower convergence than deterministic methods for low dimensions
- Requires many samples for high accuracy
- Inherently approximate (probabilistic)

## Technical Details

### Parallelization

The engine divides work evenly among threads, each with an independent RNG seeded differently to ensure statistical independence.

### Random Number Generation

- Uses Mersenne Twister (MT19937-64) for high-quality random numbers
- Period: 2^19937 - 1
- Each thread maintains its own RNG state (no locking overhead)

### Statistical Accuracy

Results include:
- **Standard Error**: σ/√n, where σ is sample std dev
- **95% Confidence Interval**: mean ± 1.96 × SE
- To halve the error, you need 4× more samples (O(1/√n) convergence)

## Common Applications

1. **Finance**: Option pricing, risk analysis (use appropriate libraries)
2. **Physics**: Particle simulations, radiation transport
3. **Statistics**: Bayesian inference, bootstrap methods
4. **Engineering**: Reliability analysis, sensitivity studies
5. **Mathematics**: Numerical integration, probability estimation

## Extending the System

The framework is designed to be extended. Simply write a function that:
1. Takes a `RandomGenerator&` parameter
2. Uses it to generate random values
3. Returns a `double` result

The engine handles the rest: parallelization, statistics, timing.

## Limitations

- Single result per sample (no vector outputs yet)
- No built-in variance reduction techniques
- No adaptive sampling
- Statistics assume independent samples

## References

- Metropolis, N., & Ulam, S. (1949). "The Monte Carlo Method"
- Kalos, M. H., & Whitlock, P. A. (2008). *Monte Carlo Methods*
- Press, W. H., et al. (2007). *Numerical Recipes*

## License

This is an educational/research project. Feel free to use and modify as needed.
