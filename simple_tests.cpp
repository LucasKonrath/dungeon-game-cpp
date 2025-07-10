#include <iostream>
#include <vector>
#include <cassert>
#include <climits>
#include <string>
#include <chrono>
#include <chrono>

using std::vector;
using std::max;
using std::min;
using std::cout;
using std::endl;
using std::string;

// Copy the DungeonGame class here for testing
class DungeonGame {
public:
    int recurse(int row, int col, const vector<vector<int>>& dungeon, 
    vector<vector<int>>& memo){
        int rows = dungeon.size();
        int cols = dungeon[0].size();

        if(row == rows - 1 && col == cols - 1){
            return max(1, 1 - dungeon[row][col]);
        }

        if(row >= rows || col >= cols){
            return INT_MAX;
        }

        if(memo[row][col] != INT_MIN){
            return memo[row][col];
        } 

        int goRight = recurse(row, col + 1, dungeon, memo);
        int goDown = recurse(row + 1, col, dungeon, memo);

        int minimumHealth = min(goRight, goDown) - dungeon[row][col];
        memo[row][col] = max(1, minimumHealth);

        return memo[row][col];
    }

    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        vector<vector<int>> memo(rows, vector<int>(cols, INT_MIN));

        return recurse(0, 0, dungeon, memo);
    }
};

// Simple test framework
class TestRunner {
private:
    int passed = 0;
    int failed = 0;
    
public:
    void expect_eq(int actual, int expected, const string& test_name) {
        if (actual == expected) {
            cout << "✓ PASS: " << test_name << " (expected: " << expected << ", got: " << actual << ")" << endl;
            passed++;
        } else {
            cout << "✗ FAIL: " << test_name << " (expected: " << expected << ", got: " << actual << ")" << endl;
            failed++;
        }
    }
    
    void print_summary() {
        cout << "\n=== Test Summary ===" << endl;
        cout << "Passed: " << passed << endl;
        cout << "Failed: " << failed << endl;
        cout << "Total: " << (passed + failed) << endl;
        if (failed == 0) {
            cout << "All tests passed! ✓" << endl;
        }
    }
    
    bool all_passed() {
        return failed == 0;
    }
};

void runTests() {
    DungeonGame game;
    TestRunner runner;
    
    cout << "Running Dungeon Game Tests...\n" << endl;
    
    // Test 1: Basic example
    {
        vector<vector<int>> dungeon = {
            {-3, 5},
            {1, -4}
        };
        runner.expect_eq(game.calculateMinimumHP(dungeon), 4, "Basic example");
    }
    
    // Test 2: Single cell with negative value
    {
        vector<vector<int>> dungeon = {{-5}};
        runner.expect_eq(game.calculateMinimumHP(dungeon), 6, "Single cell negative");
    }
    
    // Test 3: Single cell with positive value
    {
        vector<vector<int>> dungeon = {{5}};
        runner.expect_eq(game.calculateMinimumHP(dungeon), 1, "Single cell positive");
    }
    
    // Test 4: Single cell with zero
    {
        vector<vector<int>> dungeon = {{0}};
        runner.expect_eq(game.calculateMinimumHP(dungeon), 1, "Single cell zero");
    }
    
    // Test 5: All positive values
    {
        vector<vector<int>> dungeon = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };
        runner.expect_eq(game.calculateMinimumHP(dungeon), 1, "All positive values");
    }
    
    // Test 6: All negative values
    {
        vector<vector<int>> dungeon = {
            {-1, -2, -3},
            {-4, -5, -6},
            {-7, -8, -9}
        };
        runner.expect_eq(game.calculateMinimumHP(dungeon), 22, "All negative values");
    }
    
    // Test 7: Single row
    {
        vector<vector<int>> dungeon = {{-3, 5, -2, 4}};
        runner.expect_eq(game.calculateMinimumHP(dungeon), 4, "Single row");
    }
    
    // Test 8: Single column
    {
        vector<vector<int>> dungeon = {
            {-3},
            {5},
            {-2},
            {4}
        };
        runner.expect_eq(game.calculateMinimumHP(dungeon), 4, "Single column");
    }
    
    // Test 9: Princess room has large negative value
    {
        vector<vector<int>> dungeon = {
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, -100}
        };
        runner.expect_eq(game.calculateMinimumHP(dungeon), 97, "Princess room large negative");
    }
    
    // Test 10: Princess room has large positive value
    {
        vector<vector<int>> dungeon = {
            {-10, -10, -10},
            {-10, -10, -10},
            {-10, -10, 100}
        };
        runner.expect_eq(game.calculateMinimumHP(dungeon), 41, "Princess room large positive");
    }
    
    // Test 11: Optimal path requires careful planning
    {
        vector<vector<int>> dungeon = {
            {-5, 1, -2},
            {1, -100, 1},
            {1, 1, 1}
        };
        runner.expect_eq(game.calculateMinimumHP(dungeon), 6, "Optimal path planning");
    }
    
    // Test 12: 2x2 grid
    {
        vector<vector<int>> dungeon = {
            {-4, -2},
            {-3, -1}
        };
        runner.expect_eq(game.calculateMinimumHP(dungeon), 8, "2x2 grid");
    }
    
    // Test 13: Edge case - minimal health always 1
    {
        vector<vector<int>> dungeon = {
            {100, 100},
            {100, 100}
        };
        runner.expect_eq(game.calculateMinimumHP(dungeon), 1, "Minimal health is 1");
    }
    
    // Test 14: Mixed large values
    {
        vector<vector<int>> dungeon = {
            {-200, 100, -50},
            {50, -100, 200},
            {-50, 50, -10}
        };
        int result = game.calculateMinimumHP(dungeon);
        cout << "Mixed large values result: " << result << " (should be > 0)" << endl;
        runner.expect_eq(result > 0 ? 1 : 0, 1, "Mixed large values (positive result)");
    }
    
    // Test 15: Performance test - large grid
    {
        vector<vector<int>> dungeon(20, vector<int>(20, -1));
        dungeon[19][19] = 0; // Princess room
        
        auto start = std::chrono::high_resolution_clock::now();
        int result = game.calculateMinimumHP(dungeon);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cout << "Performance test (20x20 grid): " << result << " in " << duration.count() << "ms" << endl;
        runner.expect_eq(result > 0 ? 1 : 0, 1, "Performance test (positive result)");
    }
    
    runner.print_summary();
}

int main() {
    runTests();
    return 0;
}
