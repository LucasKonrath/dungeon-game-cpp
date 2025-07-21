#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <cmath>

using std::vector;
using std::priority_queue;
using std::pair;
using std::max;
using std::min;
using std::cout;
using std::endl;

struct AStarState {
    int row, col, minHealthNeeded;
    double fScore; // g(n) + h(n)
    
    AStarState(int r, int c, int h, double f) 
        : row(r), col(c), minHealthNeeded(h), fScore(f) {}
    
    // For priority queue (min-heap based on f-score)
    bool operator>(const AStarState& other) const {
        return fScore > other.fScore;
    }
};

class DungeonGameAStar {
private:
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // A* working backwards from princess to start
        vector<vector<int>> minHealth(rows, vector<int>(cols, INT_MAX));
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        priority_queue<AStarState, vector<AStarState>, std::greater<AStarState>> pq;
        
        // Start from princess room
        int princessHealth = max(1, 1 - dungeon[rows-1][cols-1]);
        minHealth[rows-1][cols-1] = princessHealth;
        
        double heuristic = manhattanDistance(rows-1, cols-1, 0, 0);
        pq.push(AStarState(rows-1, cols-1, princessHealth, princessHealth + heuristic));
        
        // Reverse directions for backward search
        vector<pair<int, int>> reverseDirections = {{0, -1}, {-1, 0}};
        
        while (!pq.empty()) {
            AStarState current = pq.top();
            pq.pop();
            
            // Skip if already visited
            if (visited[current.row][current.col]) {
                continue;
            }
            visited[current.row][current.col] = true;
            
            // Found the start position
            if (current.row == 0 && current.col == 0) {
                return current.minHealthNeeded;
            }
            
            // Explore neighbors
            for (auto& dir : reverseDirections) {
                int newRow = current.row + dir.first;
                int newCol = current.col + dir.second;
                
                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols 
                    && !visited[newRow][newCol]) {
                    
                    // Calculate health needed at (newRow, newCol)
                    int healthNeeded = max(1, current.minHealthNeeded - dungeon[newRow][newCol]);
                    
                    if (healthNeeded < minHealth[newRow][newCol]) {
                        minHealth[newRow][newCol] = healthNeeded;
                        
                        // Calculate f-score = g-score + heuristic
                        double h = manhattanDistance(newRow, newCol, 0, 0);
                        double fScore = healthNeeded + h;
                        
                        pq.push(AStarState(newRow, newCol, healthNeeded, fScore));
                    }
                }
            }
        }
        
        return minHealth[0][0];
    }
    
private:
    double manhattanDistance(int row1, int col1, int row2, int col2) {
        return abs(row1 - row2) + abs(col1 - col2);
    }
};

// Forward A* with binary search
class DungeonGameAStarForward {
private:
    struct ForwardState {
        int row, col, health;
        double fScore;
        
        ForwardState(int r, int c, int h, double f) 
            : row(r), col(c), health(h), fScore(f) {}
        
        // For priority queue (min-heap based on f-score, but we want max health)
        bool operator>(const ForwardState& other) const {
            if (fScore != other.fScore) {
                return fScore > other.fScore;
            }
            return health < other.health; // Prefer higher health for tie-breaking
        }
    };
    
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        // Binary search on starting health
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
        
        vector<vector<int>> maxHealthReached(rows, vector<int>(cols, -1));
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        priority_queue<ForwardState, vector<ForwardState>, std::greater<ForwardState>> pq;
        
        int initialHealth = startHealth + dungeon[0][0];
        if (initialHealth <= 0) return false;
        
        maxHealthReached[0][0] = initialHealth;
        double heuristic = manhattanDistance(0, 0, rows-1, cols-1);
        pq.push(ForwardState(0, 0, initialHealth, -initialHealth + heuristic));
        
        while (!pq.empty()) {
            ForwardState current = pq.top();
            pq.pop();
            
            // Skip if already visited or found better path
            if (visited[current.row][current.col] || 
                current.health < maxHealthReached[current.row][current.col]) {
                continue;
            }
            visited[current.row][current.col] = true;
            
            // Reached princess
            if (current.row == rows - 1 && current.col == cols - 1) {
                return true;
            }
            
            // Explore neighbors
            for (auto& dir : directions) {
                int newRow = current.row + dir.first;
                int newCol = current.col + dir.second;
                
                if (newRow < rows && newCol < cols && !visited[newRow][newCol]) {
                    int newHealth = current.health + dungeon[newRow][newCol];
                    
                    if (newHealth > 0 && newHealth > maxHealthReached[newRow][newCol]) {
                        maxHealthReached[newRow][newCol] = newHealth;
                        
                        double h = manhattanDistance(newRow, newCol, rows-1, cols-1);
                        double fScore = -newHealth + h; // Minimize negative health + distance
                        
                        pq.push(ForwardState(newRow, newCol, newHealth, fScore));
                    }
                }
            }
        }
        
        return false;
    }
    
    double manhattanDistance(int row1, int col1, int row2, int col2) {
        return abs(row1 - row2) + abs(col1 - col2);
    }
};

// A* with more sophisticated heuristics
class DungeonGameAStarAdvanced {
private:
    struct AdvancedState {
        int row, col, minHealthNeeded;
        double fScore;
        
        AdvancedState(int r, int c, int h, double f) 
            : row(r), col(c), minHealthNeeded(h), fScore(f) {}
        
        bool operator>(const AdvancedState& other) const {
            return fScore > other.fScore;
        }
    };
    
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        vector<vector<int>> minHealth(rows, vector<int>(cols, INT_MAX));
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        priority_queue<AdvancedState, vector<AdvancedState>, std::greater<AdvancedState>> pq;
        
        // Start from princess room
        int princessHealth = max(1, 1 - dungeon[rows-1][cols-1]);
        minHealth[rows-1][cols-1] = princessHealth;
        
        double heuristic = advancedHeuristic(dungeon, rows-1, cols-1, 0, 0);
        pq.push(AdvancedState(rows-1, cols-1, princessHealth, princessHealth + heuristic));
        
        // Reverse directions for backward search
        vector<pair<int, int>> reverseDirections = {{0, -1}, {-1, 0}};
        
        while (!pq.empty()) {
            AdvancedState current = pq.top();
            pq.pop();
            
            if (visited[current.row][current.col]) {
                continue;
            }
            visited[current.row][current.col] = true;
            
            if (current.row == 0 && current.col == 0) {
                return current.minHealthNeeded;
            }
            
            for (auto& dir : reverseDirections) {
                int newRow = current.row + dir.first;
                int newCol = current.col + dir.second;
                
                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols 
                    && !visited[newRow][newCol]) {
                    
                    int healthNeeded = max(1, current.minHealthNeeded - dungeon[newRow][newCol]);
                    
                    if (healthNeeded < minHealth[newRow][newCol]) {
                        minHealth[newRow][newCol] = healthNeeded;
                        
                        double h = advancedHeuristic(dungeon, newRow, newCol, 0, 0);
                        double fScore = healthNeeded + h;
                        
                        pq.push(AdvancedState(newRow, newCol, healthNeeded, fScore));
                    }
                }
            }
        }
        
        return minHealth[0][0];
    }
    
private:
    double advancedHeuristic(vector<vector<int>>& dungeon, int row1, int col1, 
                           int row2, int col2) {
        // Combine Manhattan distance with path difficulty estimation
        double distance = abs(row1 - row2) + abs(col1 - col2);
        
        // Estimate minimum damage along the optimal path
        double minDamageEstimate = 0;
        int steps = 0;
        
        // Sample some cells along the direct path for damage estimation
        int dr = (row2 > row1) ? 1 : (row2 < row1) ? -1 : 0;
        int dc = (col2 > col1) ? 1 : (col2 < col1) ? -1 : 0;
        
        int r = row1, c = col1;
        while (r != row2 || c != col2) {
            if (r >= 0 && r < dungeon.size() && c >= 0 && c < dungeon[0].size()) {
                if (dungeon[r][c] < 0) {
                    minDamageEstimate += abs(dungeon[r][c]);
                }
                steps++;
            }
            
            if (r != row2) r += dr;
            else if (c != col2) c += dc;
            
            if (steps > distance * 2) break; // Prevent infinite loop
        }
        
        // Weight the heuristic
        return distance + minDamageEstimate * 0.1;
    }
};

// Test function
void testAStar() {
    cout << "=== A* Implementation Test ===" << endl;
    
    DungeonGameAStar solver;
    DungeonGameAStarForward forwardSolver;
    DungeonGameAStarAdvanced advancedSolver;
    
    // Test case 1: Basic example
    vector<vector<int>> dungeon1 = {{-3, 5}, {1, -4}};
    int result1 = solver.calculateMinimumHP(dungeon1);
    int result1_fwd = forwardSolver.calculateMinimumHP(dungeon1);
    int result1_adv = advancedSolver.calculateMinimumHP(dungeon1);
    cout << "Test 1 - Expected: 7, Got (backward): " << result1 
         << ", Got (forward): " << result1_fwd 
         << ", Got (advanced): " << result1_adv << endl;
    
    // Test case 2: Single cell negative
    vector<vector<int>> dungeon2 = {{-5}};
    int result2 = solver.calculateMinimumHP(dungeon2);
    int result2_fwd = forwardSolver.calculateMinimumHP(dungeon2);
    int result2_adv = advancedSolver.calculateMinimumHP(dungeon2);
    cout << "Test 2 - Expected: 6, Got (backward): " << result2 
         << ", Got (forward): " << result2_fwd 
         << ", Got (advanced): " << result2_adv << endl;
    
    // Test case 3: Single cell positive
    vector<vector<int>> dungeon3 = {{5}};
    int result3 = solver.calculateMinimumHP(dungeon3);
    int result3_fwd = forwardSolver.calculateMinimumHP(dungeon3);
    int result3_adv = advancedSolver.calculateMinimumHP(dungeon3);
    cout << "Test 3 - Expected: 1, Got (backward): " << result3 
         << ", Got (forward): " << result3_fwd 
         << ", Got (advanced): " << result3_adv << endl;
    
    // Test case 4: Complex path
    vector<vector<int>> dungeon4 = {{1, -3, 3}, {0, -2, 0}, {-3, -3, -3}};
    int result4 = solver.calculateMinimumHP(dungeon4);
    int result4_fwd = forwardSolver.calculateMinimumHP(dungeon4);
    int result4_adv = advancedSolver.calculateMinimumHP(dungeon4);
    cout << "Test 4 - Got (backward): " << result4 
         << ", Got (forward): " << result4_fwd 
         << ", Got (advanced): " << result4_adv << endl;
    
    cout << "A* Implementation completed!" << endl;
    
    cout << "\n=== Algorithm Analysis ===" << endl;
    cout << "Backward A*: Works from princess to start with Manhattan distance heuristic" << endl;
    cout << "Forward A*: Uses binary search + A* with distance heuristic" << endl;
    cout << "Advanced A*: Enhanced heuristic considering path difficulty" << endl;
    cout << "Time Complexity: O(b^d) where b=branching factor, d=depth" << endl;
    cout << "Space Complexity: O(b^d) for the priority queue" << endl;
    cout << "Heuristic: Manhattan distance is admissible for this grid problem" << endl;
}

int main() {
    testAStar();
    return 0;
}
