#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using std::vector;
using std::max;
using std::min;
using std::cout;
using std::endl;

/**
 * Space-optimized DungeonGame implementation using 1D DP array
 * 
 * Key insight: We only need the current row and the row below it to compute
 * the minimum health required. This allows us to use a single array and
 * process the dungeon bottom-up, right-to-left.
 * 
 * Space complexity: O(cols) instead of O(rows × cols)
 * Time complexity: O(rows × cols) - same as 2D approach
 */
class DungeonGameOptimized {
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // Single array to store minimum health needed for current row
        // dp[j] = minimum health needed at position (i, j)
        vector<int> dp(cols, INT_MAX);
        
        // Start from bottom-right (princess room) and work backwards
        // For the last row, we can compute directly
        dp[cols - 1] = max(1, 1 - dungeon[rows - 1][cols - 1]);
        
        // Fill the last row from right to left
        for (int j = cols - 2; j >= 0; j--) {
            dp[j] = max(1, dp[j + 1] - dungeon[rows - 1][j]);
        }
        
        // Process remaining rows from bottom to top
        for (int i = rows - 2; i >= 0; i--) {
            // For the rightmost column, we can only come from below
            dp[cols - 1] = max(1, dp[cols - 1] - dungeon[i][cols - 1]);
            
            // For other columns, we can come from right or below
            for (int j = cols - 2; j >= 0; j--) {
                int rightPath = dp[j + 1];  // Coming from right
                int downPath = dp[j];       // Coming from below (already computed)
                
                int minHealthNeeded = min(rightPath, downPath) - dungeon[i][j];
                dp[j] = max(1, minHealthNeeded);
            }
        }
        
        return dp[0];  // Minimum health needed at starting position (0, 0)
    }
    
    /**
     * Alternative implementation that processes left-to-right, top-to-bottom
     * This requires storing the entire last row, but demonstrates the concept
     */
    int calculateMinimumHPAlternative(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // Use two arrays to alternate between current and next row
        vector<int> curr(cols, 0);
        vector<int> next(cols, 0);
        
        // Initialize the princess room (bottom-right corner)
        next[cols - 1] = max(1, 1 - dungeon[rows - 1][cols - 1]);
        
        // Fill the last row
        for (int j = cols - 2; j >= 0; j--) {
            next[j] = max(1, next[j + 1] - dungeon[rows - 1][j]);
        }
        
        // Process from second-to-last row upwards
        for (int i = rows - 2; i >= 0; i--) {
            curr = vector<int>(cols, INT_MAX);
            
            // Rightmost column - can only come from below
            curr[cols - 1] = max(1, next[cols - 1] - dungeon[i][cols - 1]);
            
            // Other columns - can come from right or below
            for (int j = cols - 2; j >= 0; j--) {
                int fromRight = curr[j + 1];
                int fromBelow = next[j];
                curr[j] = max(1, min(fromRight, fromBelow) - dungeon[i][j]);
            }
            
            next = curr;  // Move to next iteration
        }
        
        return next[0];
    }
    
    /**
     * In-place version that modifies the dungeon array directly
     * This achieves O(1) space complexity (not counting input)
     */
    int calculateMinimumHPInPlace(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // Convert dungeon values to minimum health needed
        // Start from princess room
        dungeon[rows - 1][cols - 1] = max(1, 1 - dungeon[rows - 1][cols - 1]);
        
        // Fill last row
        for (int j = cols - 2; j >= 0; j--) {
            dungeon[rows - 1][j] = max(1, dungeon[rows - 1][j + 1] - dungeon[rows - 1][j]);
        }
        
        // Fill last column
        for (int i = rows - 2; i >= 0; i--) {
            dungeon[i][cols - 1] = max(1, dungeon[i + 1][cols - 1] - dungeon[i][cols - 1]);
        }
        
        // Fill remaining cells
        for (int i = rows - 2; i >= 0; i--) {
            for (int j = cols - 2; j >= 0; j--) {
                int cellValue = dungeon[i][j];
                int rightPath = dungeon[i][j + 1];
                int downPath = dungeon[i + 1][j];
                
                dungeon[i][j] = max(1, min(rightPath, downPath) - cellValue);
            }
        }
        
        return dungeon[0][0];
    }
};

// Test class to compare different implementations
class DungeonGameComparison {
private:
    DungeonGameOptimized optimized;
    
public:
    void compareImplementations() {
        cout << "=== DUNGEON GAME IMPLEMENTATION COMPARISON ===" << endl;
        
        vector<vector<vector<int>>> testCases = {
            {{-3, 5}, {1, -4}},  // Basic test case
            {{-5}},               // Single cell
            {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},  // All positive
            {{-1, -2, -3}, {-4, -5, -6}, {-7, -8, -9}},  // All negative
            {{-10, -5, -2, -1}, {-8, -6, -3, -1}, {-7, -4, -2, -1}, {-5, -3, -1, -1}}  // 4x4 worst case
        };
        
        for (size_t i = 0; i < testCases.size(); i++) {
            cout << "\n--- Test Case " << (i + 1) << " ---" << endl;
            printDungeon(testCases[i]);
            
            // Test all three implementations
            vector<vector<int>> dungeon1 = testCases[i];
            vector<vector<int>> dungeon2 = testCases[i];
            vector<vector<int>> dungeon3 = testCases[i];
            
            int result1 = optimized.calculateMinimumHP(dungeon1);
            int result2 = optimized.calculateMinimumHPAlternative(dungeon2);
            int result3 = optimized.calculateMinimumHPInPlace(dungeon3);
            
            cout << "1D DP (bottom-up): " << result1 << endl;
            cout << "1D DP (alternative): " << result2 << endl;
            cout << "In-place DP: " << result3 << endl;
            
            // Verify all implementations give same result
            if (result1 == result2 && result2 == result3) {
                cout << "✓ All implementations agree!" << endl;
            } else {
                cout << "✗ Results differ!" << endl;
            }
        }
    }
    
    void performanceBenchmark() {
        cout << "\n=== PERFORMANCE BENCHMARK ===" << endl;
        
        vector<int> sizes = {10, 20, 50, 100};
        
        for (int size : sizes) {
            cout << "\n--- " << size << "x" << size << " grid ---" << endl;
            
            // Generate random test dungeon
            vector<vector<int>> dungeon = generateRandomDungeon(size, size);
            
            const int iterations = 1000;
            
            // Benchmark 1D DP
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < iterations; i++) {
                vector<vector<int>> testDungeon = dungeon;
                optimized.calculateMinimumHP(testDungeon);
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto time1D = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            // Benchmark in-place DP
            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < iterations; i++) {
                vector<vector<int>> testDungeon = dungeon;
                optimized.calculateMinimumHPInPlace(testDungeon);
            }
            end = std::chrono::high_resolution_clock::now();
            auto timeInPlace = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            cout << "1D DP: " << time1D.count() / iterations << " μs/iteration" << endl;
            cout << "In-place DP: " << timeInPlace.count() / iterations << " μs/iteration" << endl;
            cout << "Memory saved: " << (size * size * sizeof(int)) << " bytes" << endl;
        }
    }
    
private:
    void printDungeon(const vector<vector<int>>& dungeon) {
        for (const auto& row : dungeon) {
            cout << "  [";
            for (size_t i = 0; i < row.size(); i++) {
                cout << row[i];
                if (i < row.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
    }
    
    vector<vector<int>> generateRandomDungeon(int rows, int cols) {
        vector<vector<int>> dungeon(rows, vector<int>(cols));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                dungeon[i][j] = (rand() % 21) - 10;  // Random values from -10 to 10
            }
        }
        return dungeon;
    }
};

int main() {
    cout << "=== SPACE-OPTIMIZED DUNGEON GAME ===" << endl;
    cout << "Comparing 1D DP implementations with O(cols) space complexity" << endl;
    
    DungeonGameComparison comparison;
    comparison.compareImplementations();
    comparison.performanceBenchmark();
    
    cout << "\n=== IMPLEMENTATION DETAILS ===" << endl;
    cout << "1. 1D DP (bottom-up): Uses single array, processes bottom-up, right-to-left" << endl;
    cout << "2. 1D DP (alternative): Uses two arrays alternating between rows" << endl;
    cout << "3. In-place DP: Modifies input array directly, O(1) extra space" << endl;
    cout << "\nSpace complexity improvements:" << endl;
    cout << "- Original 2D: O(rows × cols)" << endl;
    cout << "- 1D DP: O(cols)" << endl;
    cout << "- In-place: O(1)" << endl;
    
    return 0;
}
