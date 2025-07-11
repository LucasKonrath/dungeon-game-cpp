#include <gtest/gtest.h>
#include <vector>
#include <climits>

using std::vector;
using std::max;
using std::min;

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

// Test fixture class
class DungeonGameTest : public ::testing::Test {
protected:
    DungeonGame game;
};

// Test case 1: Basic example from LeetCode problem
TEST_F(DungeonGameTest, BasicExample) {
    vector<vector<int>> dungeon = {
        {-3, 5},
        {1, -4}
    };
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 4);
}

// Test case 2: Single cell with negative value
TEST_F(DungeonGameTest, SingleCellNegative) {
    vector<vector<int>> dungeon = {{-5}};
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 6);
}

// Test case 3: Single cell with positive value
TEST_F(DungeonGameTest, SingleCellPositive) {
    vector<vector<int>> dungeon = {{5}};
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 1);
}

// Test case 4: Single cell with zero
TEST_F(DungeonGameTest, SingleCellZero) {
    vector<vector<int>> dungeon = {{0}};
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 1);
}

// Test case 5: All positive values
TEST_F(DungeonGameTest, AllPositiveValues) {
    vector<vector<int>> dungeon = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 1);
}

// Test case 6: All negative values
TEST_F(DungeonGameTest, AllNegativeValues) {
    vector<vector<int>> dungeon = {
        {-1, -2, -3},
        {-4, -5, -6},
        {-7, -8, -9}
    };
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 22);
}

// Test case 7: Mixed values - large dungeon
TEST_F(DungeonGameTest, LargeDungeonMixed) {
    vector<vector<int>> dungeon = {
        {-3, 5, -2, 4},
        {1, -4, 2, -5},
        {-2, 3, -1, 2},
        {4, -2, 1, -3}
    };
    int result = game.calculateMinimumHP(dungeon);
    EXPECT_GT(result, 0); // Should be positive
}

// Test case 8: Single row
TEST_F(DungeonGameTest, SingleRow) {
    vector<vector<int>> dungeon = {{-3, 5, -2, 4}};
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 4);
}

// Test case 9: Single column
TEST_F(DungeonGameTest, SingleColumn) {
    vector<vector<int>> dungeon = {
        {-3},
        {5},
        {-2},
        {4}
    };
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 4);
}

// Test case 10: Princess room has large negative value
TEST_F(DungeonGameTest, PrincessRoomLargeNegative) {
    vector<vector<int>> dungeon = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, -100}
    };
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 97);
}

// Test case 11: Princess room has large positive value
TEST_F(DungeonGameTest, PrincessRoomLargePositive) {
    vector<vector<int>> dungeon = {
        {-10, -10, -10},
        {-10, -10, -10},
        {-10, -10, 100}
    };
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 41);
}

// Test case 12: Optimal path requires careful planning
TEST_F(DungeonGameTest, OptimalPathPlanning) {
    vector<vector<int>> dungeon = {
        {-5, 1, -2},
        {1, -100, 1},
        {1, 1, 1}
    };
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 6);
}

// Test case 13: 2x2 grid - comprehensive test
TEST_F(DungeonGameTest, TwoByTwoGrid) {
    vector<vector<int>> dungeon = {
        {-4, -2},
        {-3, -1}
    };
    EXPECT_EQ(game.calculateMinimumHP(dungeon), 8);
}

// Test case 14: Verify memoization works (indirect test through performance)
TEST_F(DungeonGameTest, LargeGridPerformance) {
    // Create a 10x10 grid to test memoization efficiency
    vector<vector<int>> dungeon(10, vector<int>(10, -1));
    dungeon[9][9] = 0; // Princess room
    
    auto start = std::chrono::high_resolution_clock::now();
    int result = game.calculateMinimumHP(dungeon);
    auto end = std::chrono::high_resolution_clock::now();
    
    EXPECT_GT(result, 0);
    // Should complete quickly due to memoization
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count(), 100); // Should take less than 100ms
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
