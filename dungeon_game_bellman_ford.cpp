#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using std::vector;
using std::pair;
using std::max;
using std::min;
using std::cout;
using std::endl;

class DungeonGameBellmanFord {
private:
    struct Edge {
        int from_row, from_col, to_row, to_col, weight;
        
        Edge(int fr, int fc, int tr, int tc, int w) 
            : from_row(fr), from_col(fc), to_row(tr), to_col(tc), weight(w) {}
    };
    
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        // Create edges for the graph
        vector<Edge> edges;
        createEdges(dungeon, edges);
        
        // Use Bellman-Ford to find minimum health needed
        // We work backwards from princess to start
        vector<vector<int>> minHealth(rows, vector<int>(cols, INT_MAX));
        
        // Initialize princess room
        minHealth[rows-1][cols-1] = max(1, 1 - dungeon[rows-1][cols-1]);
        
        // Relax edges (rows * cols - 1) times
        for (int i = 0; i < rows * cols - 1; i++) {
            bool updated = false;
            
            // Process each edge (in reverse direction for backward algorithm)
            for (const Edge& edge : edges) {
                int fromHealth = minHealth[edge.to_row][edge.to_col];
                if (fromHealth != INT_MAX) {
                    // Calculate health needed at source to reach destination
                    int healthNeeded = max(1, fromHealth - dungeon[edge.from_row][edge.from_col]);
                    
                    if (healthNeeded < minHealth[edge.from_row][edge.from_col]) {
                        minHealth[edge.from_row][edge.from_col] = healthNeeded;
                        updated = true;
                    }
                }
            }
            
            // Early termination if no updates
            if (!updated) break;
        }
        
        return minHealth[0][0];
    }
    
private:
    void createEdges(vector<vector<int>>& dungeon, vector<Edge>& edges) {
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // Add edges from current cell to adjacent cells
                for (auto& dir : directions) {
                    int newRow = i + dir.first;
                    int newCol = j + dir.second;
                    
                    if (newRow < rows && newCol < cols) {
                        // Edge weight is the value of destination cell
                        edges.push_back(Edge(i, j, newRow, newCol, dungeon[newRow][newCol]));
                    }
                }
            }
        }
    }
};

// Alternative Bellman-Ford implementation using distance relaxation
class DungeonGameBellmanFordDistance {
private:
    struct Edge {
        int from_idx, to_idx, weight;
        
        Edge(int f, int t, int w) : from_idx(f), to_idx(t), weight(w) {}
    };
    
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; // right, down
    
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) {
            return 1;
        }
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        int totalCells = rows * cols;
        
        // Convert 2D coordinates to 1D index
        auto getIndex = [cols](int row, int col) { return row * cols + col; };
        auto getCoords = [cols](int idx) { return std::make_pair(idx / cols, idx % cols); };
        
        // Create edges
        vector<Edge> edges;
        createEdges(dungeon, edges, getIndex);
        
        // Bellman-Ford algorithm
        vector<int> dist(totalCells, INT_MAX);
        int princessIdx = getIndex(rows - 1, cols - 1);
        
        // Initialize: minimum health needed at princess room
        dist[princessIdx] = max(1, 1 - dungeon[rows-1][cols-1]);
        
        // Relax edges V-1 times
        for (int i = 0; i < totalCells - 1; i++) {
            bool updated = false;
            
            // Process edges in reverse (for backward propagation)
            for (const Edge& edge : edges) {
                if (dist[edge.to_idx] != INT_MAX) {
                    auto [toRow, toCol] = getCoords(edge.to_idx);
                    auto [fromRow, fromCol] = getCoords(edge.from_idx);
                    
                    int healthNeeded = max(1, dist[edge.to_idx] - dungeon[fromRow][fromCol]);
                    
                    if (healthNeeded < dist[edge.from_idx]) {
                        dist[edge.from_idx] = healthNeeded;
                        updated = true;
                    }
                }
            }
            
            if (!updated) break;
        }
        
        return dist[getIndex(0, 0)];
    }
    
private:
    void createEdges(vector<vector<int>>& dungeon, vector<Edge>& edges, 
                    std::function<int(int, int)> getIndex) {
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                for (auto& dir : directions) {
                    int newRow = i + dir.first;
                    int newCol = j + dir.second;
                    
                    if (newRow < rows && newCol < cols) {
                        int fromIdx = getIndex(i, j);
                        int toIdx = getIndex(newRow, newCol);
                        edges.push_back(Edge(fromIdx, toIdx, dungeon[newRow][newCol]));
                    }
                }
            }
        }
    }
};

// Forward Bellman-Ford with binary search
class DungeonGameBellmanFordForward {
private:
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
        
        // Use Bellman-Ford to find maximum health at each cell
        vector<vector<int>> maxHealth(rows, vector<int>(cols, INT_MIN));
        
        // Initialize starting position
        maxHealth[0][0] = startHealth + dungeon[0][0];
        if (maxHealth[0][0] <= 0) return false;
        
        // Relax edges V-1 times
        for (int iter = 0; iter < rows * cols - 1; iter++) {
            bool updated = false;
            
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (maxHealth[i][j] == INT_MIN) continue;
                    
                    // Try all neighbors
                    for (auto& dir : directions) {
                        int newRow = i + dir.first;
                        int newCol = j + dir.second;
                        
                        if (newRow < rows && newCol < cols) {
                            int newHealth = maxHealth[i][j] + dungeon[newRow][newCol];
                            
                            if (newHealth > 0 && newHealth > maxHealth[newRow][newCol]) {
                                maxHealth[newRow][newCol] = newHealth;
                                updated = true;
                            }
                        }
                    }
                }
            }
            
            if (!updated) break;
        }
        
        return maxHealth[rows-1][cols-1] > 0;
    }
};

// Test function
void testBellmanFord() {
    cout << "=== Bellman-Ford Implementation Test ===" << endl;
    
    DungeonGameBellmanFord solver;
    DungeonGameBellmanFordDistance distanceSolver;
    DungeonGameBellmanFordForward forwardSolver;
    
    // Test case 1: Basic example
    vector<vector<int>> dungeon1 = {{-3, 5}, {1, -4}};
    int result1 = solver.calculateMinimumHP(dungeon1);
    int result1_dist = distanceSolver.calculateMinimumHP(dungeon1);
    int result1_fwd = forwardSolver.calculateMinimumHP(dungeon1);
    cout << "Test 1 - Expected: 7, Got (basic): " << result1 
         << ", Got (distance): " << result1_dist 
         << ", Got (forward): " << result1_fwd << endl;
    
    // Test case 2: Single cell negative
    vector<vector<int>> dungeon2 = {{-5}};
    int result2 = solver.calculateMinimumHP(dungeon2);
    int result2_dist = distanceSolver.calculateMinimumHP(dungeon2);
    int result2_fwd = forwardSolver.calculateMinimumHP(dungeon2);
    cout << "Test 2 - Expected: 6, Got (basic): " << result2 
         << ", Got (distance): " << result2_dist 
         << ", Got (forward): " << result2_fwd << endl;
    
    // Test case 3: Single cell positive
    vector<vector<int>> dungeon3 = {{5}};
    int result3 = solver.calculateMinimumHP(dungeon3);
    int result3_dist = distanceSolver.calculateMinimumHP(dungeon3);
    int result3_fwd = forwardSolver.calculateMinimumHP(dungeon3);
    cout << "Test 3 - Expected: 1, Got (basic): " << result3 
         << ", Got (distance): " << result3_dist 
         << ", Got (forward): " << result3_fwd << endl;
    
    // Test case 4: Complex case
    vector<vector<int>> dungeon4 = {{1, -3, 3}, {0, -2, 0}, {-3, -3, -3}};
    int result4 = solver.calculateMinimumHP(dungeon4);
    int result4_dist = distanceSolver.calculateMinimumHP(dungeon4);
    int result4_fwd = forwardSolver.calculateMinimumHP(dungeon4);
    cout << "Test 4 - Got (basic): " << result4 
         << ", Got (distance): " << result4_dist 
         << ", Got (forward): " << result4_fwd << endl;
    
    cout << "Bellman-Ford Implementation completed!" << endl;
    
    cout << "\n=== Algorithm Analysis ===" << endl;
    cout << "Basic Bellman-Ford: Works backward from princess, relaxes health requirements" << endl;
    cout << "Distance Bellman-Ford: Uses 1D indexing and distance relaxation" << endl;
    cout << "Forward Bellman-Ford: Uses binary search + forward health propagation" << endl;
    cout << "Time Complexity: O(V*E) where V = rows*cols, E = edges ≈ 2*V" << endl;
    cout << "Space Complexity: O(V) for distance array" << endl;
}

int main() {
    testBellmanFord();
    return 0;
}
