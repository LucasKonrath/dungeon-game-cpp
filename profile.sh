#!/bin/bash

# Dungeon Game Profiling Script
# This script runs comprehensive profiling analysis

echo "=== DUNGEON GAME PROFILING SUITE ==="
echo

# Check if build is needed
if [ ! -f "profiling_tests" ]; then
    echo "Building profiling tests..."
    make profiling_tests
    echo
fi

echo "Choose profiling option:"
echo "1. Basic performance profiling"
echo "2. Optimized build profiling"
echo "3. Memory usage profiling"
echo "4. Instruments profiling (macOS)"
echo "5. Valgrind profiling (Linux/macOS)"
echo "6. All profiling methods"
echo "7. Quick benchmark"
echo

read -p "Enter your choice (1-7): " choice

case $choice in
    1)
        echo "Running basic performance profiling..."
        make profile
        ;;
    2)
        echo "Running optimized build profiling..."
        make profile-optimized
        ;;
    3)
        echo "Running memory usage profiling..."
        make profile-memory
        ;;
    4)
        echo "Running Instruments profiling (macOS)..."
        make profile-instruments
        ;;
    5)
        echo "Running Valgrind profiling..."
        make profile-valgrind
        ;;
    6)
        echo "Running all profiling methods..."
        echo
        echo "=== 1. Basic Profiling ==="
        make profile
        echo
        echo "=== 2. Optimized Profiling ==="
        make profile-optimized
        echo
        echo "=== 3. Memory Profiling ==="
        make profile-memory
        echo
        echo "=== 4. Instruments (if available) ==="
        make profile-instruments
        echo
        echo "=== 5. Valgrind (if available) ==="
        make profile-valgrind
        ;;
    7)
        echo "Running quick benchmark..."
        if [ ! -f "profiling_optimized" ]; then
            make profiling_optimized
        fi
        echo "Testing different grid sizes..."
        time ./profiling_optimized | grep -E "(Testing|Average time|Result)"
        ;;
    *)
        echo "Invalid choice. Please run the script again."
        exit 1
        ;;
esac

echo
echo "=== PROFILING COMPLETE ==="
echo

# Provide analysis recommendations
echo "Profiling files generated:"
ls -la | grep -E "(callgrind|instruments|\.prof|\.trace)"

echo
echo "Next steps for analysis:"
echo "- View callgrind output: kcachegrind callgrind.out (or qcachegrind)"
echo "- Open Instruments trace: open instruments_output.trace"
echo "- Compare performance across different optimizations"
echo "- Analyze memory usage patterns"
echo "- Identify bottlenecks in recursive calls"
