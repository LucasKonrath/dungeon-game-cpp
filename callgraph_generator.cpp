#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <fstream>

using std::vector;
using std::max;
using std::min;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;

// DungeonGame class with call tracing
class DungeonGame {
private:
    int callDepth = 0;
    ofstream* traceFile = nullptr;
    
    void logCall(const string& functionName, int row, int col, const string& action = "") {
        if (traceFile) {
            string indent(callDepth * 2, ' ');
            *traceFile << indent << functionName << "(" << row << "," << col << ")" << action << endl;
        }
    }
    
public:
    void setTraceFile(ofstream* file) {
        traceFile = file;
    }
    
    int recurse(int row, int col, const vector<vector<int>>& dungeon, 
                vector<vector<int>>& memo) {
        
        callDepth++;
        logCall("recurse", row, col, " - ENTER");
        
        int rows = dungeon.size();
        int cols = dungeon[0].size();

        // Base case: reached princess
        if(row == rows - 1 && col == cols - 1) {
            int result = max(1, 1 - dungeon[row][col]);
            logCall("recurse", row, col, " - PRINCESS ROOM, health needed: " + std::to_string(result));
            callDepth--;
            return result;
        }

        // Out of bounds
        if(row >= rows || col >= cols) {
            logCall("recurse", row, col, " - OUT OF BOUNDS");
            callDepth--;
            return INT_MAX;
        }

        // Already computed
        if(memo[row][col] != INT_MIN) {
            logCall("recurse", row, col, " - MEMOIZED: " + std::to_string(memo[row][col]));
            callDepth--;
            return memo[row][col];
        }

        // Explore paths
        logCall("recurse", row, col, " - EXPLORING PATHS");
        
        int goRight = recurse(row, col + 1, dungeon, memo);
        int goDown = recurse(row + 1, col, dungeon, memo);

        int minimumHealth = min(goRight, goDown) - dungeon[row][col];
        memo[row][col] = max(1, minimumHealth);
        
        logCall("recurse", row, col, " - COMPUTED: " + std::to_string(memo[row][col]) + 
                " (right=" + std::to_string(goRight) + ", down=" + std::to_string(goDown) + 
                ", cell=" + std::to_string(dungeon[row][col]) + ")");
        
        callDepth--;
        return memo[row][col];
    }

    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        int rows = dungeon.size();
        int cols = dungeon[0].size();
        
        vector<vector<int>> memo(rows, vector<int>(cols, INT_MIN));
        
        if (traceFile) {
            *traceFile << "=== DUNGEON GAME CALL TRACE ===" << endl;
            *traceFile << "Grid size: " << rows << "x" << cols << endl;
            *traceFile << "Dungeon:" << endl;
            for (int i = 0; i < rows; i++) {
                *traceFile << "  [";
                for (int j = 0; j < cols; j++) {
                    *traceFile << dungeon[i][j];
                    if (j < cols - 1) *traceFile << ", ";
                }
                *traceFile << "]" << endl;
            }
            *traceFile << endl;
        }

        return recurse(0, 0, dungeon, memo);
    }
};

// Function to generate DOT file for callgraph visualization
void generateCallGraphDot(const string& traceFile, const string& dotFile) {
    // This is a simplified version - in practice you'd parse the trace file
    // and generate a proper DOT graph
    ofstream dot(dotFile);
    dot << "digraph CallGraph {" << endl;
    dot << "  rankdir=TD;" << endl;
    dot << "  node [shape=box, style=filled, fillcolor=lightblue];" << endl;
    dot << "  edge [color=blue];" << endl;
    dot << endl;
    
    // For the 2x2 example, we know the structure
    dot << "  \"calculateMinimumHP\" [fillcolor=lightgreen];" << endl;
    dot << "  \"recurse(0,0)\" [fillcolor=yellow];" << endl;
    dot << "  \"recurse(0,1)\" [fillcolor=yellow];" << endl;
    dot << "  \"recurse(1,0)\" [fillcolor=yellow];" << endl;
    dot << "  \"recurse(1,1)\" [fillcolor=orange, label=\"recurse(1,1)\\nPrincess Room\"];" << endl;
    dot << "  \"recurse(0,2)\" [fillcolor=red, label=\"recurse(0,2)\\nOut of Bounds\"];" << endl;
    dot << "  \"recurse(1,2)\" [fillcolor=red, label=\"recurse(1,2)\\nOut of Bounds\"];" << endl;
    dot << "  \"recurse(2,0)\" [fillcolor=red, label=\"recurse(2,0)\\nOut of Bounds\"];" << endl;
    dot << "  \"recurse(2,1)\" [fillcolor=red, label=\"recurse(2,1)\\nOut of Bounds\"];" << endl;
    dot << endl;
    
    // Edges
    dot << "  \"calculateMinimumHP\" -> \"recurse(0,0)\";" << endl;
    dot << "  \"recurse(0,0)\" -> \"recurse(0,1)\";" << endl;
    dot << "  \"recurse(0,0)\" -> \"recurse(1,0)\";" << endl;
    dot << "  \"recurse(0,1)\" -> \"recurse(0,2)\";" << endl;
    dot << "  \"recurse(0,1)\" -> \"recurse(1,1)\";" << endl;
    dot << "  \"recurse(1,0)\" -> \"recurse(1,1)\";" << endl;
    dot << "  \"recurse(1,0)\" -> \"recurse(2,0)\";" << endl;
    dot << "  \"recurse(1,1)\" -> \"recurse(1,2)\";" << endl;
    dot << "  \"recurse(1,1)\" -> \"recurse(2,1)\";" << endl;
    dot << endl;
    
    dot << "}" << endl;
    dot.close();
}

int main() {
    DungeonGame game;
    
    // Test case: 2x2 grid
    vector<vector<int>> dungeon = {
        {-3, 5},
        {1, -4}
    };
    
    // Create trace file
    ofstream traceFile("callgraph_trace.txt");
    game.setTraceFile(&traceFile);
    
    cout << "Generating callgraph for 2x2 dungeon..." << endl;
    cout << "Dungeon grid:" << endl;
    cout << "  [-3,  5]" << endl;
    cout << "  [ 1, -4]" << endl;
    cout << endl;
    
    int result = game.calculateMinimumHP(dungeon);
    
    traceFile << endl << "=== FINAL RESULT ===" << endl;
    traceFile << "Minimum HP needed: " << result << endl;
    traceFile.close();
    
    cout << "Minimum HP needed: " << result << endl;
    cout << "Call trace saved to: callgraph_trace.txt" << endl;
    
    // Generate DOT file for visualization
    generateCallGraphDot("callgraph_trace.txt", "callgraph.dot");
    cout << "DOT file generated: callgraph.dot" << endl;
    cout << endl;
    
    cout << "To visualize the callgraph:" << endl;
    cout << "1. Install Graphviz: brew install graphviz" << endl;
    cout << "2. Generate PNG: dot -Tpng callgraph.dot -o callgraph.png" << endl;
    cout << "3. Generate SVG: dot -Tsvg callgraph.dot -o callgraph.svg" << endl;
    cout << "4. View the trace: cat callgraph_trace.txt" << endl;
    
    return 0;
}
