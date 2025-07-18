#include <iostream>
#include <vector>
#include <climits>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>

using std::vector;
using std::max;
using std::min;
using std::cout;
using std::endl;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

// Original DungeonGame class with profiling hooks
class DungeonGame {
private:
    mutable size_t recursiveCalls = 0;
    mutable size_t memoHits = 0;
    mutable size_t boundaryChecks = 0;
    mutable std::chrono::nanoseconds totalTime{0};
    
public:
    void resetProfileData() const {
        recursiveCalls = 0;
        memoHits = 0;
        boundaryChecks = 0;
        totalTime = std::chrono::nanoseconds{0};
    }
    
    void printProfileData() const {
        cout << "\n=== PROFILING RESULTS ===" << endl;
        cout << "Recursive calls: " << recursiveCalls << endl;
        cout << "Memoization hits: " << memoHits << endl;
        cout << "Boundary checks: " << boundaryChecks << endl;
        cout << "Total execution time: " << totalTime.count() << " nanoseconds" << endl;
        cout << "Average time per call: " << (recursiveCalls > 0 ? totalTime.count() / recursiveCalls : 0) << " ns" << endl;
        cout << "Memoization hit rate: " << std::fixed << std::setprecision(2) 
             << (recursiveCalls > 0 ? (double)memoHits / recursiveCalls * 100 : 0) << "%" << endl;
    }

    int recurse(int row, int col, const vector<vector<int>>& dungeon, 
                vector<vector<int>>& memo) const {
        
        auto start = high_resolution_clock::now();
        recursiveCalls++;
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();

        // Base case: reached princess
        if(row == rows - 1 && col == cols - 1) {
            auto end = high_resolution_clock::now();
            totalTime += duration_cast<nanoseconds>(end - start);
            return max(1, 1 - dungeon[row][col]);
        }

        // Out of bounds check
        boundaryChecks++;
        if(row >= rows || col >= cols) {
            auto end = high_resolution_clock::now();
            totalTime += duration_cast<nanoseconds>(end - start);
            return INT_MAX;
        }

        // Memoization check
        if(memo[row][col] != INT_MIN) {
            memoHits++;
            auto end = high_resolution_clock::now();
            totalTime += duration_cast<nanoseconds>(end - start);
            return memo[row][col];
        }

        // Recursive calls
        int goRight = recurse(row, col + 1, dungeon, memo);
        int goDown = recurse(row + 1, col, dungeon, memo);

        int minimumHealth = min(goRight, goDown) - dungeon[row][col];
        memo[row][col] = max(1, minimumHealth);
        
        auto end = high_resolution_clock::now();
        totalTime += duration_cast<nanoseconds>(end - start);
        
        return memo[row][col];
    }

    int calculateMinimumHP(vector<vector<int>>& dungeon) const {
        resetProfileData();
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        vector<vector<int>> memo(rows, vector<int>(cols, INT_MIN));
        
        auto start = high_resolution_clock::now();
        int result = recurse(0, 0, dungeon, memo);
        auto end = high_resolution_clock::now();
        
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Total algorithm time: " << duration.count() << " microseconds" << endl;
        
        return result;
    }
};

// Performance benchmarking
class PerformanceBenchmark {
private:
    DungeonGame game;
    
public:
    void benchmarkDifferentSizes() {
        cout << "\n=== PERFORMANCE BENCHMARK ===" << endl;
        
        vector<int> sizes = {5, 10, 15, 20, 25, 30};
        
        for (int size : sizes) {
            cout << "\n--- Testing " << size << "x" << size << " grid ---" << endl;
            
            // Create random dungeon
            vector<vector<int>> dungeon = generateRandomDungeon(size, size);
            
            // Benchmark multiple runs
            const int runs = 10;
            auto totalStart = high_resolution_clock::now();
            
            for (int i = 0; i < runs; i++) {
                vector<vector<int>> testDungeon = dungeon; // Copy for each run
                game.calculateMinimumHP(testDungeon);
            }
            
            auto totalEnd = high_resolution_clock::now();
            auto avgTime = duration_cast<microseconds>(totalEnd - totalStart).count() / runs;
            
            cout << "Average time over " << runs << " runs: " << avgTime << " μs" << endl;
            
            // Single run with detailed profiling
            vector<vector<int>> testDungeon = dungeon;
            int result = game.calculateMinimumHP(testDungeon);
            cout << "Result: " << result << endl;
            game.printProfileData();
        }
    }
    
    void benchmarkWorstCase() {
        cout << "\n=== WORST CASE SCENARIO ===" << endl;
        
        // All negative values - forces exploration of all paths
        vector<vector<int>> worstCase = {
            {-10, -5, -2, -1},
            {-8, -6, -3, -1},
            {-7, -4, -2, -1},
            {-5, -3, -1, -1}
        };
        
        cout << "Worst case dungeon (all negative):" << endl;
        printDungeon(worstCase);
        
        int result = game.calculateMinimumHP(worstCase);
        cout << "Result: " << result << endl;
        game.printProfileData();
    }
    
    void benchmarkBestCase() {
        cout << "\n=== BEST CASE SCENARIO ===" << endl;
        
        // All positive values - optimal path is straightforward
        vector<vector<int>> bestCase = {
            {5, 10, 15, 20},
            {8, 12, 18, 25},
            {10, 15, 20, 30},
            {12, 18, 25, 35}
        };
        
        cout << "Best case dungeon (all positive):" << endl;
        printDungeon(bestCase);
        
        int result = game.calculateMinimumHP(bestCase);
        cout << "Result: " << result << endl;
        game.printProfileData();
    }
    
    void memoryUsageAnalysis() {
        cout << "\n=== MEMORY USAGE ANALYSIS ===" << endl;
        
        vector<int> sizes = {10, 20, 30, 40, 50};
        
        for (int size : sizes) {
            // Calculate memory usage for memoization table
            size_t memoSize = size * size * sizeof(int);
            size_t dungeonSize = size * size * sizeof(int);
            size_t totalMemory = memoSize + dungeonSize;
            
            cout << size << "x" << size << " grid:" << endl;
            cout << "  Memoization table: " << memoSize << " bytes" << endl;
            cout << "  Dungeon grid: " << dungeonSize << " bytes" << endl;
            cout << "  Total memory: " << totalMemory << " bytes (" 
                 << std::fixed << std::setprecision(2) << totalMemory / 1024.0 << " KB)" << endl;
            
            // Test actual performance
            vector<vector<int>> dungeon = generateRandomDungeon(size, size);
            auto start = high_resolution_clock::now();
            game.calculateMinimumHP(dungeon);
            auto end = high_resolution_clock::now();
            
            auto time = duration_cast<microseconds>(end - start).count();
            cout << "  Execution time: " << time << " μs" << endl;
            cout << "  Time per cell: " << std::fixed << std::setprecision(3) 
                 << (double)time / (size * size) << " μs" << endl << endl;
        }
    }
    
private:
    vector<vector<int>> generateRandomDungeon(int rows, int cols) {
        vector<vector<int>> dungeon(rows, vector<int>(cols));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-20, 10);
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                dungeon[i][j] = dis(gen);
            }
        }
        
        return dungeon;
    }
    
    void printDungeon(const vector<vector<int>>& dungeon) {
        for (const auto& row : dungeon) {
            cout << "  [";
            for (size_t i = 0; i < row.size(); i++) {
                cout << std::setw(3) << row[i];
                if (i < row.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
    }
};

int main() {
    cout << "=== DUNGEON GAME PROFILING SUITE ===" << endl;
    
    PerformanceBenchmark benchmark;
    
    // Run different benchmark scenarios
    benchmark.benchmarkBestCase();
    benchmark.benchmarkWorstCase();
    benchmark.benchmarkDifferentSizes();
    benchmark.memoryUsageAnalysis();
    
    cout << "\n=== PROFILING COMPLETE ===" << endl;
    cout << "For more detailed profiling, use external tools:" << endl;
    cout << "- Valgrind: valgrind --tool=callgrind ./profiling_tests" << endl;
    cout << "- Instruments (macOS): instruments -t 'Time Profiler' ./profiling_tests" << endl;
    cout << "- perf (Linux): perf record ./profiling_tests && perf report" << endl;
    
    return 0;
}
