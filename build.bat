@echo off
REM Simple build script for Windows with MinGW

echo Building Monte Carlo Simulation System...
echo.

REM Compile estimate_pi
echo Compiling estimate_pi.exe...
g++ -std=c++17 -O3 -pthread -I./include src/Engine.cpp src/RandomGenerator.cpp src/Statistics.cpp examples/estimate_pi.cpp -o estimate_pi.exe
if %ERRORLEVEL% NEQ 0 (
    echo Error compiling estimate_pi
    exit /b 1
)
echo OK

REM Compile monte_carlo_integration
echo Compiling monte_carlo_integration.exe...
g++ -std=c++17 -O3 -pthread -I./include src/Engine.cpp src/RandomGenerator.cpp src/Statistics.cpp examples/monte_carlo_integration.cpp -o monte_carlo_integration.exe
if %ERRORLEVEL% NEQ 0 (
    echo Error compiling monte_carlo_integration
    exit /b 1
)
echo OK

echo.
echo Build complete! Run the examples:
echo   estimate_pi.exe
echo   monte_carlo_integration.exe
