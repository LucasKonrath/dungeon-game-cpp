#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using std::vector;
using std::priority_queue;
using std::pair;
using std::max;
using std::min;
using std::cout;
using std::endl;

struct DijkstraState {
    int row, col, minHealthNeeded;
    
    DijkstraState(int r, int c, int h) : row(r), col(c), minHealthNeeded(h) {}
    
    // For priority queue (min-heap based on minimum health needed)
    bool operator>(const DijkstraState& other) const {
        return minHealthNeeded > other.minHealthNeeded;
    }
};

class DungeonGameDijkstra {
private:
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // Dijkstra's algorithm to find minimum health needed
        // We'll work backwards: find minimum health needed to reach princess from each cell
        vector<vector<int>> minHealth(rows, vector<int>(cols, INT_MAX));
        priority_queue<DijkstraState, vector<DijkstraState>, std::greater<DijkstraState>> pq;
        
        // Start from princess room - minimum health needed there
        int princessHealth = max(1, 1 - dungeon[rows-1][cols-1]);
        minHealth[rows-1][cols-1] = princessHealth;
        pq.push(DijkstraState(rows-1, cols-1, princessHealth));
        
        // Reverse directions (left, up) since we're working backwards
        vector<pair<int, int>> reverseDirections = {{0, -1}, {-1, 0}};
        
        while (!pq.empty()) {
            DijkstraState current = pq.top();
            pq.pop();
            
            // Skip if we've already found a better path to this cell
            if (current.minHealthNeeded > minHealth[current.row][current.col]) {
                continue;
            }
            
            // Explore neighbors (cells that can reach current cell)
            for (auto& dir : reverseDirections) {
                int newRow = current.row + dir.first;
                int newCol = current.col + dir.second;
                
                // Check bounds
                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                    // Calculate minimum health needed at (newRow, newCol) to reach princess
                    int healthNeeded = max(1, current.minHealthNeeded - dungeon[newRow][newCol]);
                    
                    // If we found a better path to this cell
                    if (healthNeeded < minHealth[newRow][newCol]) {
                        minHealth[newRow][newCol] = healthNeeded;
                        pq.push(DijkstraState(newRow, newCol, healthNeeded));
                    }
                }
            }
        }
        
        return minHealth[0][0];
    }
};

// Alternative Dijkstra implementation working forward
class DungeonGameDijkstraForward {
private:
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
    struct ForwardState {
        int row, col, healthWhenEntering;
        
        ForwardState(int r, int c, int h) : row(r), col(c), healthWhenEntering(h) {}
        
        // For priority queue (max-heap based on health when entering)
        bool operator<(const ForwardState& other) const {
            return healthWhenEntering < other.healthWhenEntering;
        }
    };
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        // Binary search on the starting health
        int left = 1, right = 1000000;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (canReachWithStartingHealth(dungeon, mid)) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left;
    }
    
private:
    bool canReachWithStartingHealth(vector<vector<int>>& dungeon, int startHealth) {
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // Use Dijkstra to find the path that maximizes health when reaching princess
        vector<vector<int>> maxHealthReached(rows, vector<int>(cols, -1));
        priority_queue<ForwardState> pq;
        
        int initialHealth = startHealth + dungeon[0][0];
        if (initialHealth <= 0) return false;
        
        maxHealthReached[0][0] = initialHealth;
        pq.push(ForwardState(0, 0, initialHealth));
        
        while (!pq.empty()) {
            ForwardState current = pq.top();
            pq.pop();
            
            // Skip if we've already found a better path to this cell
            if (current.healthWhenEntering < maxHealthReached[current.row][current.col]) {
                continue;
            }
            
            // Check if reached princess
            if (current.row == rows - 1 && current.col == cols - 1) {
                return true;
            }
            
            // Explore neighbors
            for (auto& dir : directions) {
                int newRow = current.row + dir.first;
                int newCol = current.col + dir.second;
                
                if (newRow < rows && newCol < cols) {
                    int newHealth = current.healthWhenEntering + dungeon[newRow][newCol];
                    
                    // Must have positive health
                    if (newHealth > 0 && newHealth > maxHealthReached[newRow][newCol]) {
                        maxHealthReached[newRow][newCol] = newHealth;
                        pq.push(ForwardState(newRow, newCol, newHealth));
                    }
                }
            }
        }
        
        return false;
    }
};

// Test function
void testDijkstra() {
    cout << "=== Dijkstra Implementation Test ===" << endl;
    
    DungeonGameDijkstra solver;
    DungeonGameDijkstraForward forwardSolver;
    
    // Test case 1: Basic example
    vector<vector<int>> dungeon1 = {{-3, 5}, {1, -4}};
    int result1 = solver.calculateMinimumHP(dungeon1);
    int result1_fwd = forwardSolver.calculateMinimumHP(dungeon1);
    cout << "Test 1 - Expected: 7, Got (backward): " << result1 
         << ", Got (forward): " << result1_fwd << endl;
    
    // Test case 2: Single cell negative
    vector<vector<int>> dungeon2 = {{-5}};
    int result2 = solver.calculateMinimumHP(dungeon2);
    int result2_fwd = forwardSolver.calculateMinimumHP(dungeon2);
    cout << "Test 2 - Expected: 6, Got (backward): " << result2 
         << ", Got (forward): " << result2_fwd << endl;
    
    // Test case 3: Single cell positive
    vector<vector<int>> dungeon3 = {{5}};
    int result3 = solver.calculateMinimumHP(dungeon3);
    int result3_fwd = forwardSolver.calculateMinimumHP(dungeon3);
    cout << "Test 3 - Expected: 1, Got (backward): " << result3 
         << ", Got (forward): " << result3_fwd << endl;
    
    // Test case 4: All positive path exists
    vector<vector<int>> dungeon4 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int result4 = solver.calculateMinimumHP(dungeon4);
    int result4_fwd = forwardSolver.calculateMinimumHP(dungeon4);
    cout << "Test 4 - Expected: 1, Got (backward): " << result4 
         << ", Got (forward): " << result4_fwd << endl;
    
    cout << "Dijkstra Implementation completed!" << endl;
    
    cout << "\n=== Algorithm Analysis ===" << endl;
    cout << "Backward Dijkstra: Works from princess to start, directly calculates minimum health" << endl;
    cout << "Forward Dijkstra: Uses binary search + forward traversal, finds maximum health paths" << endl;
}

int main() {
    testDijkstra();
    return 0;
}
