//https://github.com/Cerealmaster0621/SnakeGame_A-search
//15/11/2023 snake doesn't recognize it's own body yet. needs modification for that.    

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

//return full paths to goal(0 is next position, last is goal)
vector<Position> a_star(const Position& start, const Position& goal, const Board& board){
    auto compare = [](const Node& a, const Node& b) { return a.totalCost() > b.totalCost(); }; //compare two nodes
    std::priority_queue<Node, std::vector<Node>, decltype(compare)> openSet(compare); // this sets contains Nodes that needs to be treated, lowest cost comes on top
    std::unordered_map<Position, Node> allNodes; //contains every Nodes that has or needs to be searched

    openSet.push({start, 0, heuristic(start, goal),nullptr});//openset initial value(can't be empty unless search has done)  
    allNodes[start] = {start, 0, heuristic(start, goal),nullptr};//allNodes initial value

    while (!openSet.empty()){
        Node current = openSet.top();//current Node = top of the openset queue that has lowest totalcost
        openSet.pop(); // delete the current node from openset since we don't need to iterate again

        if (current.Position == goal){ //when path search finished, go back to the parents Node(nullptr) and save every
        //Nodes in path vector. return REVERSED order of path since path[0] should be current position and last index to goal.
            vector<Position> path = {};
            while(!(current.parent == nullptr)){
                path.push_back(current.Position);
                current = *current.parent; 
            }
            reverse(path.begin(),path.end());
            return path;
        }

        for(const auto& neighbor : getNeighbors(current.Position, board)){
            if(!board.is_valid_position(neighbor)) continue; //if the given neighbor is not valid, continue
            //!!!make node before adding in the unordered_map or priority queue. cause infinite loop for same adrees on Parent Node!!!
            Node neighborNode = {neighbor, current.cost + 1, heuristic(neighbor,goal),&allNodes[current.Position]};
            bool isNewNode = allNodes.find(neighbor) == allNodes.end();//check if it's not already in allNodes map
            bool isCloserToGoal = isNewNode || (current.cost+1) < allNodes[neighbor].cost;//check new route is closer to goal
            if(isCloserToGoal){ //if New Node is not in allNodes or have better costs
                allNodes[neighbor] = neighborNode;//apppend new Nodes
                openSet.push(neighborNode); //append new openSet
            }
        }
    }
    return {}; //return empty path if no possible path found
}

//based on vector<Position> from a_star, decide next move
int direction(const Board& board, const Position& start){
    vector<Position> path = a_star(board.get_head(), board.apple, board);
    Position result = path[0];
    cout<<"next row : "<<result.row<<", next column : "<<result.column<<endl;
    if (result.column > start.column) {
        return 0; //when targetted column is bigger than current column -> move right
    } else if (result.column < start.column) {
        return 2; //when targetted column is smaller than current column -> move left
    } else if (result.row > start.row) {
        return 3; //when targetted row is bigger than current row -> move down
    } else if (result.row < start.row) {
        return 1; //when targetted row is smaller than current row -> move up
    }
    return -1;
}

int choose_next_move(const Board& board) {
    //return direction(a_star(board.get_head(), board.apple, board), board.get_head());
    // return direction(board.apple, board.get_head());
    return direction(board, board.get_head());
}