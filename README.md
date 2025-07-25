# Dungeon Game Tests

This repository contains unit tests for the DungeonGame C++ implementation.

The visual callgraph shows the tree structure with:
- 🟢 **Green**: Entry point (`calculateMinimumHP`)
- 🟡 **Yellow**: Recursive calls exploring valid cells
- 🟠 **Orange**: Princess room (destination)
- 🔴 **Red**: Out-of-bounds calls (pruned)

## Comprehensive Performance Comparison

### DP Algorithms Performance:

| Size  | 2D DP (μs) | 1D DP (μs) | InPlace (μs) |
|-------|------------|------------|--------------|
| 10x10 | 17.7       | 8.7        | 8.7          |
| 25x25 | 78.6       | 36.3       | 35.9         |
| 50x50 | 231.2      | 113.6      | 124.6        |

### Graph Algorithms Performance:

| Size  | BFS (μs) | DFS (μs) | Dijkstra (μs) | Bellman (μs) | A* (μs) |
|-------|----------|----------|---------------|--------------|---------|
| 10x10 | 446.4    | 232.3    | 46.1          | 74.5         | 29.1    |
| 25x25 | 1938.9   | 470.0    | 250.1         | 788.8        | 57.2    |

### Speedup Comparison (relative to 2D DP):

| Algorithm   | Speedup     |
|-------------|-------------|
| 1D DP       | 2.28x faster |
| In-Place DP | 2.09x faster |
| BFS         | 0.03x slower |
| DFS         | 0.10x slower |
| Dijkstra    | 0.20x slower |

## Memory Complexity Analysis

| Grid Size | 2D DP Memory | 1D DP Memory | In-Place Memory | 1D Reduction | In-Place Reduction |
|-----------|--------------|--------------|-----------------|--------------|-------------------|
| 10x10     | 400 B        | 40 B         | 0 B             | 90%          | 100%             |
| 50x50     | 9 KB         | 200 B        | 0 B             | 98%          | 100%             |
| 100x100   | 39 KB        | 400 B        | 0 B             | 99%          | 100%             |
| 500x500   | 976 KB       | 1 KB         | 0 B             | 99%          | 100%             |
| 1000x1000 | 3 MB         | 3 KB         | 0 B             | 99%          | 100%             |           

## Comprehensive Algorithm Analysis

### 📊 Dynamic Programming Algorithms:

| Aspect          | 2D DP (Original) | 1D DP (Optimized) | In-Place DP     |
|-----------------|------------------|--------------------|-----------------|
| Time Complexity | O(m×n)           | O(m×n)             | O(m×n)          |
| Space Complexity| O(m×n)           | O(n)               | O(1)            |
| Approach        | Top-down (rec.)  | Bottom-up (iter)   | Bottom-up (iter)|
| Memory Access   | Random           | Sequential         | Sequential      |
| Cache Locality  | Poor             | Excellent          | Excellent       |
| Stack Usage     | O(m+n)           | O(1)               | O(1)            |
| Input Modified  | No               | No                 | Yes             |
| Performance     | Baseline         | ~2x faster         | ~2.5x faster    |

### 🔍 Graph Algorithm Approaches:

| Algorithm    | Time Complexity  | Space Complexity | Characteristics  |
|--------------|------------------|------------------|------------------|
| BFS          | O(m×n×log(max))  | O(m×n)           | Level-order      |
| DFS          | O(m×n×log(max))  | O(m×n)           | Depth-first      |
| Dijkstra     | O(m×n×log(m×n))  | O(m×n)           | Shortest path    |
| Bellman-Ford | O((m×n)²)        | O(m×n)           | General graphs   |
| A*           | O(b^d)           | O(b^d)           | Heuristic search |

🏆 Algorithm Recommendations:

**For Production Systems:**
   ✅ Winner: 1D DP
   • Best balance of performance and memory
   • 90-99% memory reduction vs 2D DP
   • Preserves input data

**For Memory-Critical Systems:**
   ✅ Winner: In-Place DP
   • O(1) extra space complexity
   • Fastest execution
   • Warning: Modifies input array

**For Educational Purposes:**
   ✅ Winner: 2D DP + Graph Algorithms
   • 2D DP: Most intuitive approach
   • Graph algorithms: Show versatility
   • Demonstrate different paradigms

**For Extensible/Research Systems:**
   ✅ Winner: Dijkstra or A*
   • Easy to modify for different constraints
   • Can handle obstacles, weighted paths
   • A* provides good performance with heuristics

📈 Performance Insights:
• DP algorithms are optimal for this specific problem
• Graph algorithms add O(log) factor due to binary search
• Bellman-Ford is slowest but most general
• A* performs well with good heuristics
• Cache locality is crucial for performance

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
## Profiling Results
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

### Core Implementations
- `dungeon-game.cpp` - Original 2D DP implementation
- `dungeon_game_1d_dp.cpp` - Space-optimized 1D DP and in-place implementations
- `comparison_2d_vs_1d.cpp` - Performance comparison between DP implementations

### Graph Algorithm Implementations
- `dungeon_game_bfs.cpp` - Breadth-First Search approach
- `dungeon_game_dfs.cpp` - Depth-First Search approach (iterative and recursive)
- `dungeon_game_dijkstra.cpp` - Dijkstra's shortest path algorithm
- `dungeon_game_bellman_ford.cpp` - Bellman-Ford algorithm (multiple variants)
- `dungeon_game_astar.cpp` - A* heuristic search algorithm

### Testing and Analysis Tools
- `simple_tests.cpp` - Self-contained unit tests with custom test framework
- `callgraph_generator.cpp` - Generates call traces and visual callgraphs
- `profiling_tests.cpp` - Comprehensive performance profiling suite
- `comprehensive_algorithm_analysis.cpp` - Detailed comparison of all algorithms
- `profile.sh` - Interactive profiling script

### Build and Configuration
- `Makefile` - Build automation for all implementations
- `CMakeLists.txt` - CMake build configuration
- `.gitignore` - Git ignore rules for build artifacts

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
## Test Summary
Passed: 14
Failed: 0
Total: 14
All tests passed! ✓
```

## Graph Algorithm Implementations

In addition to the Dynamic Programming approaches, this project includes implementations using classic graph algorithms to demonstrate different algorithmic paradigms:

### Available Graph Algorithms

1. **BFS (Breadth-First Search)** - `dungeon_game_bfs.cpp`
2. **DFS (Depth-First Search)** - `dungeon_game_dfs.cpp`
3. **Dijkstra's Algorithm** - `dungeon_game_dijkstra.cpp`
4. **Bellman-Ford Algorithm** - `dungeon_game_bellman_ford.cpp`
5. **A* Search Algorithm** - `dungeon_game_astar.cpp`

### Running Graph Algorithm Tests

```bash
# Test all graph algorithm implementations
make test-graph-algorithms

# Test individual algorithms
make dungeon_game_bfs && ./dungeon_game_bfs
make dungeon_game_dfs && ./dungeon_game_dfs
make dungeon_game_dijkstra && ./dungeon_game_dijkstra
make dungeon_game_bellman_ford && ./dungeon_game_bellman_ford
make dungeon_game_astar && ./dungeon_game_astar
```

### Algorithm Comparison Summary

| Algorithm     | Time Complexity  | Space Complexity | Best Use Case |
|---------------|------------------|------------------|---------------|
| **2D DP**     | O(m×n)          | O(m×n)          | Learning, debugging |
| **1D DP**     | O(m×n)          | O(n)            | **Production systems** |
| **In-Place DP** | O(m×n)        | O(1)            | Memory-critical |
| **BFS**       | O(m×n×log(max)) | O(m×n)          | Educational |
| **DFS**       | O(m×n×log(max)) | O(m×n)          | Educational |
| **Dijkstra**  | O(m×n×log(m×n)) | O(m×n)          | Extensible systems |
| **Bellman-Ford** | O((m×n)²)    | O(m×n)          | General graphs |
| **A***        | O(b^d)          | O(b^d)          | Heuristic search |

