#include <iostream>
#include <vector>
#include <stack>
#include <climits>
#include <algorithm>

using std::vector;
using std::stack;
using std::pair;
using std::max;
using std::min;
using std::cout;
using std::endl;

struct DFSState {
    int row, col, health;
    
    DFSState(int r, int c, int h) : row(r), col(c), health(h) {}
};

class DungeonGameDFS {
private:
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        // Binary search on the minimum starting health
        int left = 1, right = 1000000;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (canReachPrincessDFS(dungeon, mid)) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left;
    }
    
private:
    bool canReachPrincessDFS(vector<vector<int>>& dungeon, int startHealth) {
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // DFS using stack to check if we can reach princess
        stack<DFSState> stk;
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        
        stk.push(DFSState(0, 0, startHealth));
        
        while (!stk.empty()) {
            DFSState current = stk.top();
            stk.pop();
            
            // Skip if already visited (optimization)
            if (visited[current.row][current.col]) {
                continue;
            }
            visited[current.row][current.col] = true;
            
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
            
            // Explore neighbors (add to stack in reverse order for consistent traversal)
            for (int i = directions.size() - 1; i >= 0; i--) {
                int newRow = current.row + directions[i].first;
                int newCol = current.col + directions[i].second;
                
                if (newRow < rows && newCol < cols && !visited[newRow][newCol]) {
                    stk.push(DFSState(newRow, newCol, currentHealth));
                }
            }
        }
        
        return false;
    }
};

// Alternative recursive DFS implementation
class DungeonGameDFSRecursive {
private:
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        // Binary search on the minimum starting health
        int left = 1, right = 1000000;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            vector<vector<bool>> visited(dungeon.size(), vector<bool>(dungeon[0].size(), false));
            if (dfsRecursive(dungeon, 0, 0, mid, visited)) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left;
    }
    
private:
    bool dfsRecursive(vector<vector<int>>& dungeon, int row, int col, int health, 
                     vector<vector<bool>>& visited) {
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // Boundary check
        if (row >= rows || col >= cols || visited[row][col]) {
            return false;
        }
        
        // Mark as visited
        visited[row][col] = true;
        
        // Check health after entering this cell
        int currentHealth = health + dungeon[row][col];
        
        // Must have positive health
        if (currentHealth <= 0) {
            visited[row][col] = false; // Backtrack
            return false;
        }
        
        // Reached princess
        if (row == rows - 1 && col == cols - 1) {
            visited[row][col] = false; // Backtrack for other paths
            return true;
        }
        
        // Try all directions
        for (auto& dir : directions) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;
            
            if (dfsRecursive(dungeon, newRow, newCol, currentHealth, visited)) {
                visited[row][col] = false; // Backtrack
                return true;
            }
        }
        
        // Backtrack
        visited[row][col] = false;
        return false;
    }
};

// Test function
void testDFS() {
    cout << "=== DFS Implementation Test ===" << endl;
    
    DungeonGameDFS solver;
    DungeonGameDFSRecursive recursiveSolver;
    
    // Test case 1: Basic example
    vector<vector<int>> dungeon1 = {{-3, 5}, {1, -4}};
    int result1 = solver.calculateMinimumHP(dungeon1);
    int result1_rec = recursiveSolver.calculateMinimumHP(dungeon1);
    cout << "Test 1 - Expected: 7, Got (iterative): " << result1 
         << ", Got (recursive): " << result1_rec << endl;
    
    // Test case 2: Single cell negative
    vector<vector<int>> dungeon2 = {{-5}};
    int result2 = solver.calculateMinimumHP(dungeon2);
    int result2_rec = recursiveSolver.calculateMinimumHP(dungeon2);
    cout << "Test 2 - Expected: 6, Got (iterative): " << result2 
         << ", Got (recursive): " << result2_rec << endl;
    
    // Test case 3: Single cell positive
    vector<vector<int>> dungeon3 = {{5}};
    int result3 = solver.calculateMinimumHP(dungeon3);
    int result3_rec = recursiveSolver.calculateMinimumHP(dungeon3);
    cout << "Test 3 - Expected: 1, Got (iterative): " << result3 
         << ", Got (recursive): " << result3_rec << endl;
    
    // Test case 4: All positive path exists
    vector<vector<int>> dungeon4 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int result4 = solver.calculateMinimumHP(dungeon4);
    int result4_rec = recursiveSolver.calculateMinimumHP(dungeon4);
    cout << "Test 4 - Expected: 1, Got (iterative): " << result4 
         << ", Got (recursive): " << result4_rec << endl;
    
    cout << "DFS Implementation completed!" << endl;
}

int main() {
    testDFS();
    return 0;
}
