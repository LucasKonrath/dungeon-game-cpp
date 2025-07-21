#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using std::vector;
using std::queue;
using std::pair;
using std::max;
using std::min;
using std::cout;
using std::endl;

struct State {
    int row, col, health;
    
    State(int r, int c, int h) : row(r), col(c), health(h) {}
};

class DungeonGameBFS {
private:
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // BFS approach: try different starting health values
        // Binary search on the answer
        int left = 1, right = 1000000;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (canReachPrincess(dungeon, mid)) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left;
    }
    
private:
    bool canReachPrincess(vector<vector<int>>& dungeon, int startHealth) {
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // BFS to check if we can reach princess with given starting health
        queue<State> q;
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        
        q.push(State(0, 0, startHealth));
        visited[0][0] = true;
        
        while (!q.empty()) {
            State current = q.front();
            q.pop();
            
            // Check current health after entering this cell
            int currentHealth = current.health + dungeon[current.row][current.col];
            
            // Must have positive health
            if (currentHealth <= 0) {
                continue;
            }
            
            // Reached princess
            if (current.row == rows - 1 && current.col == cols - 1) {
                return true;
            }
            
            // Explore neighbors
            for (auto& dir : directions) {
                int newRow = current.row + dir.first;
                int newCol = current.col + dir.second;
                
                if (newRow < rows && newCol < cols && !visited[newRow][newCol]) {
                    visited[newRow][newCol] = true;
                    q.push(State(newRow, newCol, currentHealth));
                }
            }
        }
        
        return false;
    }
};

// Test function
void testBFS() {
    cout << "=== BFS Implementation Test ===" << endl;
    
    DungeonGameBFS solver;
    
    // Test case 1: Basic example
    vector<vector<int>> dungeon1 = {{-3, 5}, {1, -4}};
    int result1 = solver.calculateMinimumHP(dungeon1);
    cout << "Test 1 - Expected: 7, Got: " << result1 << endl;
    
    // Test case 2: Single cell negative
    vector<vector<int>> dungeon2 = {{-5}};
    int result2 = solver.calculateMinimumHP(dungeon2);
    cout << "Test 2 - Expected: 6, Got: " << result2 << endl;
    
    // Test case 3: Single cell positive
    vector<vector<int>> dungeon3 = {{5}};
    int result3 = solver.calculateMinimumHP(dungeon3);
    cout << "Test 3 - Expected: 1, Got: " << result3 << endl;
    
    // Test case 4: All positive path exists
    vector<vector<int>> dungeon4 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int result4 = solver.calculateMinimumHP(dungeon4);
    cout << "Test 4 - Expected: 1, Got: " << result4 << endl;
    
    cout << "BFS Implementation completed!" << endl;
}

int main() {
    testBFS();
    return 0;
}
