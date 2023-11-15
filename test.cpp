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

//A*search algorithm
Position a_star(const Position& start, const Position& goal, const Board& board){
    auto compare = [](const Node& a, const Node& b) { return a.totalCost() > b.totalCost(); }; //compare two nodes
    std::priority_queue<Node, std::vector<Node>, decltype(compare)> openSet(compare); // this sets contains Nodes that needs to be treated, lowest cost comes on top
    std::unordered_map<Position, Node> allNodes; //contains every Nodes that has or needs to be searched

    openSet.push({start, 0, heuristic(start, goal),nullptr});//openset initial value(can't be empty unless search has done)  
    allNodes[start] = {start, 0, heuristic(start, goal),nullptr};//allNodes initial value

    while (!openSet.empty()){
        Node current = openSet.top();//current Node = top of the openset queue that has lowest totalcost
        openSet.pop(); // delete the current node from openset since we don't need to iterate again

        if (current.Position == goal){
            
        }
    }
    //return Position coordinates
}

//decides which direction should snake move based on the 
//coordinates of a_star function has returned.
int direction(const Position& result, const Position& start){
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
    return direction(board.apple, board.get_head());
}

