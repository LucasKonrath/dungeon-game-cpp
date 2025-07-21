# Dungeon Game Tests

This repository contains unit tests for the DungeonGame C++ implementation.

The visual callgraph shows the tree structure with:
- 🟢 **Green**: Entry point (`calculateMinimumHP`)
- 🟡 **Yellow**: Recursive calls exploring valid cells
- 🟠 **Orange**: Princess room (destination)
- 🔴 **Red**: Out-of-bounds calls (pruned)

=== DUNGEON GAME: 2D vs 1D vs In-Place DP COMPARISON ===
## Algorithm Comparison: 2D vs 1D vs In-Place DP

| Test Case | 2D DP | 1D DP | In-Place | Match |
|-----------|-------|-------|----------|-------|
| Case 1    | 4     | 4     | 4        | ✓     |
| Case 2    | 6     | 6     | 6        | ✓     |
| Case 3    | 1     | 1     | 1        | ✓     |
| Case 4    | 22    | 22    | 22       | ✓     |
| Case 5    | 22    | 22    | 22       | ✓     |     

## Performance Comparison

| Size   | 2D DP (μs) | 1D DP (μs) | In-Place (μs) | 1D Speedup | In-Place Speedup |
|--------|------------|------------|---------------|------------|------------------|
| 10x10  | 19.6       | 8.4        | 8.4           | 2.34x      | 2.34x           |
| 25x25  | 69.9       | 30.3       | 30.1          | 2.31x      | 2.32x           |
| 50x50  | 180.0      | 72.7       | 75.1          | 2.47x      | 2.40x           |
| 100x100| 467.4      | 191.2      | 207.5         | 2.44x      | 2.25x           |
| 200x200| 1615.0     | 736.1      | 816.9         | 2.19x      | 1.98x           |

## Memory Complexity Analysis

| Grid Size | 2D DP Memory | 1D DP Memory | In-Place Memory | 1D Reduction | In-Place Reduction |
|-----------|--------------|--------------|-----------------|--------------|-------------------|
| 10x10     | 400 B        | 40 B         | 0 B             | 90%          | 100%             |
| 50x50     | 9 KB         | 200 B        | 0 B             | 98%          | 100%             |
| 100x100   | 39 KB        | 400 B        | 0 B             | 99%          | 100%             |
| 500x500   | 976 KB       | 1 KB         | 0 B             | 99%          | 100%             |
| 1000x1000 | 3 MB         | 3 KB         | 0 B             | 99%          | 100%             |           

## Algorithm Analysis

| Aspect          | 2D DP (Original) | 1D DP (Optimized) | In-Place DP     |
|-----------------|------------------|--------------------|-----------------|
| Time Complexity | O(rows × cols)   | O(rows × cols)     | O(rows × cols)  |
| Space Complexity| O(rows × cols)   | O(cols)            | O(1)            |
| Approach        | Top-down (rec.)  | Bottom-up (iter)   | Bottom-up (iter)|
| Memory Access   | Random           | Sequential         | Sequential      |
| Cache Locality  | Poor             | Excellent          | Excellent       |
| Stack Usage     | O(rows + cols)   | O(1)               | O(1)            |
| Input Modified  | No               | No                 | Yes             |
| Readability     | High             | Medium             | Medium          |
| Performance     | Baseline         | ~2x faster         | ~2x faster, slower than 1D dp for larger arrays    |
| Memory Usage    | Baseline         | ~90-99% less       | 100% less       |

Key Optimizations by Implementation:

1. 2D DP (Original):
   • Intuitive recursive approach with memoization
   • Easy to understand and debug
   • Good for small to medium grids

2. 1D DP (Space-Optimized):
   • Reduced space from O(mn) to O(n)
   • Better cache locality with sequential access
   • No recursion overhead, iterative approach
   • Preserves original input data

3. In-Place DP (Ultimate Optimization):
   • O(1) extra space complexity
   • Fastest execution due to minimal memory allocation for small arrays, slower for larger arrays due to memory access patterns
   • Best cache performance
   • Modifies input array (may not be suitable for all use cases)

## Conclusion

### Three distinct optimization levels:

**1. 2D DP (Original):**
- **Best for**: Learning, debugging, small grids
- **Pros**: Clear logic, preserves input, intuitive
- **Cons**: High memory usage, slower performance

**2. 1D DP (Space-Optimized):**
- **Best for**: Large grids, production systems
- **Pros**: 90-99% memory reduction, 2x faster, preserves input
- **Cons**: More complex logic

**3. In-Place DP (Ultimate):**
- **Best for**: Memory-critical, embedded systems
- **Pros**: O(1) space, fastest execution for small samples, minimal allocation
- **Cons**: Modifies input, not suitable when original data needed

**Recommendation**: Use 1D DP for most production scenarios!

## Performance Profiling

Comprehensive profiling tools are available to analyze algorithm performance, memory usage, and optimization opportunities:

### Quick Profiling
```bash
# Run basic performance analysis
make profile

# Run optimized build profiling
make profile-optimized

# Interactive profiling menu
./profile.sh
```

### Advanced Profiling Tools

#### Instruments (macOS)
```bash
# Generate detailed performance trace
make profile-instruments
open instruments_output.trace
```

#### Valgrind (Linux/macOS)
```bash
# Call graph analysis
make profile-valgrind
kcachegrind callgrind.out

# Memory leak detection
make profile-memory
```

#### Manual Profiling
```bash
# With gprof
g++ -pg -o profiling_gprof profiling_tests.cpp
./profiling_gprof
gprof profiling_gprof gmon.out > analysis.txt

# With perf (Linux)
perf record ./profiling_optimized
perf report
```

### Profiling Results Analysis

The profiling suite provides insights into:

- ⏱️ **Execution Time**: Microsecond-level timing analysis
- 🔄 **Recursive Calls**: Count and patterns of function calls
- 💾 **Memoization Efficiency**: Hit rates and cache effectiveness
- 🧠 **Memory Usage**: Memory consumption by grid size
- 📊 **Scalability**: Performance characteristics as problem size grows

**Sample Output:**
```
=== PROFILING RESULTS ===
Recursive calls: 1799
Memoization hits: 840
Boundary checks: 1797
Total execution time: 4897917 nanoseconds
Average time per call: 2722 ns
Memoization hit rate: 46.69%
```

![callgraph](image.png)

## CI/CD Status

[![Build Status](https://github.com/YOUR_USERNAME/dungeon-game-cpp/workflows/Build%20Status/badge.svg)](https://github.com/YOUR_USERNAME/dungeon-game-cpp/actions)
[![C++ Tests](https://github.com/YOUR_USERNAME/dungeon-game-cpp/workflows/C%2B%2B%20Tests/badge.svg)](https://github.com/YOUR_USERNAME/dungeon-game-cpp/actions)

This project uses GitHub Actions for continuous integration. The CI pipeline automatically:
- ✅ Builds the project on Ubuntu and macOS
- ✅ Runs all unit tests
- ✅ Generates callgraph traces
- ✅ Tests with both GCC and Clang compilers
- ✅ Validates CMake build system
- ✅ Uploads test artifacts

## Files

- `dungeon-game.cpp` - Original dungeon game implementation
- `simple_tests.cpp` - Self-contained unit tests with custom test framework
- `callgraph_generator.cpp` - Generates call traces and visual callgraphs
- `profiling_tests.cpp` - Comprehensive performance profiling suite
- `dungeon_game_1d_dp.cpp` - Space-optimized 1D DP implementation
- `comparison_2d_vs_1d.cpp` - Performance comparison between implementations
- `profile.sh` - Interactive profiling script
- `Makefile` - Build automation
- `CMakeLists.txt` - CMake build configuration

## Running Tests

### Simple Tests (No external dependencies)

The simple tests use a custom lightweight test framework and don't require any external libraries:

```bash
# Compile and run simple tests
make test
```

Or manually:
```bash
clang++ -std=c++11 -Wall -Wextra -g -o simple_tests simple_tests.cpp
./simple_tests
```

### Using CMake
```bash
mkdir build
cd build
cmake ..
make
./simple_tests
```

## Space-Optimized 1D DP Implementation

An optimized version using O(cols) space complexity instead of O(rows × cols):

### Running 1D DP Tests
```bash
# Test the 1D DP implementation
make test-1d-dp

# Compare 2D vs 1D performance
make compare
```

### Key Optimizations

**1D DP Implementation Features:**
- 🚀 **Space Complexity**: O(cols) instead of O(rows × cols)
- ⚡ **Performance**: ~2x faster due to better cache locality  
- 💾 **Memory Savings**: Up to 99% reduction for large grids
- 🔄 **No Recursion**: Iterative bottom-up approach
- ✅ **Same Results**: Identical correctness to original algorithm

**Three Implementation Variants:**
1. **Bottom-up 1D DP**: Single array, processes right-to-left
2. **Alternative 1D DP**: Two arrays alternating between rows  
3. **In-place DP**: Modifies input directly, O(1) extra space

**Performance Comparison:**

| Grid Size | 2D DP Memory | 1D DP Memory | Performance | Memory Saved |
|-----------|--------------|--------------|-------------|--------------|
| 10x10     | 400 B        | 40 B         | 2.03x       | 90%          |
| 100x100   | 39 KB        | 400 B        | 2.32x       | 99%          |
| 1000x1000 | 3 MB         | 3 KB         | ~2.3x       | 99%          |

## Generating Callgraphs

You can generate detailed call traces and visual callgraphs to understand how the algorithm executes:

### Generate Call Trace
```bash
# Generate text trace of function calls
make callgraph
cat callgraph_trace.txt
```

### Generate Visual Callgraph
First install Graphviz (for visualization):
```bash
# On macOS with Homebrew
brew install graphviz

# On Ubuntu/Debian
sudo apt-get install graphviz
```

Then generate the visual callgraph:
```bash
# Generate PNG and SVG callgraph images
make callgraph-viz

# Or manually:
dot -Tpng callgraph.dot -o callgraph.png
dot -Tsvg callgraph.dot -o callgraph.svg
```

### Example Callgraph Output

For a 2x2 dungeon:
```
  [-3,  5]
  [ 1, -4]
```

The call trace shows:
1. `calculateMinimumHP` starts the process
2. `recurse(0,0)` explores from top-left
3. Explores both right `(0,1)` and down `(1,0)` paths
4. Eventually reaches princess room `(1,1)` 
5. Uses memoization to avoid recalculation
6. Returns minimum HP needed: **4**

## Test Cases Covered

The tests cover the following scenarios:

1. **Basic functionality** - Standard dungeon traversal
2. **Edge cases** - Single cell dungeons with positive/negative/zero values
3. **Boundary conditions** - Single row/column dungeons
4. **All positive values** - Should return minimum health of 1
5. **All negative values** - Tests worst-case scenario
6. **Mixed values** - Realistic dungeon configurations
7. **Princess room scenarios** - Large positive/negative values in destination
8. **Optimal path planning** - Cases where path choice matters
9. **Performance** - Large grids to verify memoization efficiency
10. **Result validation** - Ensures minimum health is always positive

## Algorithm Description

The DungeonGame class implements a dynamic programming solution to find the minimum initial health points needed to reach the princess. The algorithm:

1. Uses memoization to avoid recalculating subproblems
2. Works backwards from the destination
3. At each cell, calculates the minimum health needed considering both possible moves (right and down)
4. Ensures the knight never has health ≤ 0 during the journey

## Expected Test Results

When all tests pass, you should see output similar to:
```
✓ PASS: Basic example (expected: 3, got: 3)
✓ PASS: Single cell negative (expected: 6, got: 6)
✓ PASS: Single cell positive (expected: 1, got: 1)
...
=== Test Summary ===
Passed: 14
Failed: 0
Total: 14
All tests passed! ✓
```
