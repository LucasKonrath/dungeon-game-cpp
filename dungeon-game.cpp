#include <iostream>
#include <vector>

using std::vector;
using std::max;
using std::min;


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