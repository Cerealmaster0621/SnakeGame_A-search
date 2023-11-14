#include "player.h"
#include <iostream>
#include <vector>
#include <queue>
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

struct Node {
    int x,y; //coordinates of the node
    int cost;
    int heuristic;
    Node* parent;
    //f(x) = g(x)+h(x)
    int totalCost() const{
        return cost+heuristic;
    }
};

//Compare several Nodes by total cost and decide
//which Node should be in closed Node
struct CompareNode{
    bool operator()(const Node* a,const Node* b){
        return a->totalCost() > b-> totalCost();
    }
};

//heuristic(Manhattan)
int heuristic(const Position& a, const Position& b) {
    return distance(a,b);
}

//A*search algorithm
Position a_star(const Position& start, const Position& goal, const vector<vector<int>>& grid){
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
        return 3; //when targetted row is bigger than current row -> move up
    } else if (result.row < start.row) {
        return 1; //when targetted row is smaller than current row -> move down
    }
    return -1;
}

int choose_next_move(const Board& board) {
    Position target = board.apple;
    Position start = board.get_head();
    return direction(target, start);
}

