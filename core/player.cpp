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
    Position position; //coordinates of the node
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
    auto compare = [](const Node& a, const Node& b) { return a.totalCost() > b.totalCost(); }; //compare f(x) of a and b and store as variable
    priority_queue<Node*, vector<Node*>, decltype(compare)> openSet(compare); //always make top the Node that has highest totalcost


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

