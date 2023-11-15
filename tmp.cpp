//https://github.com/Cerealmaster0621/SnakeGame_A-search

#include "player.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <cmath>

using namespace std;

/*
    * Position = {row, column};
    * const int MOVE_RIGHT = 0;
    * const int MOVE_UP = 1;
    * const int MOVE_LEFT = 2;
    * const int MOVE_DOWN = 3;
    * 
*/

namespace std { // hash function for custom Node unordered map
    template <>
    struct hash<Position> {
        size_t operator()(const Position& Position) const {
            return hash<int>()(Position.row) ^ hash<int>()(Position.column);
        }
    };
}

struct Node {
    Position Position; //coordinates of the node
    int cost;
    int heuristic;
    Node* parent;
    //f(x) = g(x)+h(x)
    int totalCost() const{
        return cost+heuristic;
    }
};

//heuristic(Manhattan)
int heuristic(const Position& a, const Position& b) {
    return distance(a,b);
}

vector<Position> getNeighbors(const Position& a,const Board& board){ //get neighbored Node and store it in result
    vector<Position> result = {};
    if (board.is_valid_position({a.row,a.column+1})) result.push_back({a.row,a.column+1});//0
    if (board.is_valid_position({a.row-1,a.column})) result.push_back({a.row-1,a.column});//1
    if (board.is_valid_position({a.row,a.column-1})) result.push_back({a.row,a.column-1});//2
    if (board.is_valid_position({a.row+1,a.column})) result.push_back({a.row+1,a.column});//3
    return result;
}

//return full paths to goal(0 is current position, last is goal)
Position a_star(const Position& start, const Position& goal, const Board& board) {
    auto compare = [](const Node& a, const Node& b) { return a.totalCost() > b.totalCost(); };
    priority_queue<Node, vector<Node>, decltype(compare)> openSet(compare);
    unordered_map<Position, Node> allNodes;
    cout<<"a_ start function called "<<endl;
    openSet.push({start, 0, heuristic(start, goal), nullptr});
    allNodes[start] = {start, 0, heuristic(start, goal), nullptr};
    cout<<"open set : "<<openSet.top().parent<<endl;
    cout<<"allNodes : "<<allNodes[start].parent<<endl;
    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();
        cout<<"current row : "<<current.Position.row<<", current column : "<<current.Position.column<<endl;
        if (current.Position == goal) {
            // If the goal is the start, there is no next position to move to
            cout<<"current position is goal"<<endl;
            if (current.parent == nullptr) {
                return start; // or some indication there is no next move
            }

            // Trace back to find the next position after the start
            int loopCount = 0;
            while (current.parent != nullptr && current.parent->Position != start) {
                current = *current.parent;
            }
            return current.Position;
        }

        for (const auto& neighbor : getNeighbors(current.Position, board)) {
            if (!board.is_valid_position(neighbor)) continue;

            bool isNewNode = allNodes.find(neighbor) == allNodes.end();
            bool isCloserToGoal = (current.cost + 1 < allNodes[neighbor].cost);

            if (isNewNode || isCloserToGoal) {
                if (neighbor != current.Position) {  // Prevent self-parenting
                    allNodes[neighbor] = {neighbor, current.cost + 1, heuristic(neighbor, goal), &current};
                    openSet.push(allNodes[neighbor]);
                }
            }
        }
    }
    return {0,0}; // Return an empty Position if no path is found
}


//NEEDS MODIFICATION
int direction(const Board& board, const Position& result){
    if (result.column > board.get_head().column) {
        return 0; //when targetted column is bigger than current column -> move right
    } else if (result.column < board.get_head().column) {
        return 2; //when targetted column is smaller than current column -> move left
    } else if (result.row > board.get_head().row) {
        return 3; //when targetted row is bigger than current row -> move down
    } else if (result.row < board.get_head().row) {
        return 1; //when targetted row is smaller than current row -> move up
    }
    return 1;
}

int choose_next_move(const Board& board) {
    Position result = a_star(board.get_head(),board.apple, board);  
    return direction(board,result);
}

