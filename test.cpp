
#include "player.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <random>
#include <map>

using namespace std;

namespace std { // hash function for custom Node unordered map
    template <>
    struct hash<Position> {
        size_t operator()(const Position& position) const {
            return hash<int>()(position.row) ^ hash<int>()(position.column);
        }
    };
}

struct PositionComparator {
    bool operator()(const Position& a, const Position& b) const {
        if (a.row == b.row) {
            return a.column < b.column;
        }
        return a.row < b.row;
    }
};


//random Iterator for generating random functions
template<typename Iter, typename RandomGenerator> 
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    uniform_int_distribution<> dis(0, distance(start, end) - 1);
    advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static random_device rd;
    static mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

struct Node {
    Position position;
    int cost;
    int heuristic;
    Node* parent;
    //f(x) = g(x)+h(x)
    int total_cost() const{
        return cost+heuristic;
    }
};

//heuristic(Manhattan)
int heuristic(const Position& a, const Position& b) {
    return distance(a,b);
}

vector<Position> get_neighbors(const Position& a,const Board& board){ //get neighbored Node and store it in result
    vector<Position> result = {};
    deque<Position> snake_body = board.snake;
    if (board.is_valid_position({a.row,a.column+1})) result.push_back({a.row,a.column+1});//0
    if (board.is_valid_position({a.row-1,a.column})) result.push_back({a.row-1,a.column});//1
    if (board.is_valid_position({a.row,a.column-1})) result.push_back({a.row,a.column-1});//2
    if (board.is_valid_position({a.row+1,a.column})) result.push_back({a.row+1,a.column});//3
    for (const auto& body_part : snake_body) { //check if body part is on neighbor
        auto it = find(result.begin(), result.end(), body_part);
        if (it != result.end()) {
            result.erase(it); // Remove from result if there is snake body part found
        }
    }
    return result;
}

//return full paths to goal(0 is next position, last is goal)
vector<Position> a_star(const Position& start, const Position& goal, const Board& board){
    auto compare = [](const Node& a, const Node& b) { return a.total_cost() > b.total_cost(); }; //compare two nodes
    priority_queue<Node, vector<Node>, decltype(compare)> open_set(compare); // this sets contains Nodes that needs to be treated, lowest cost comes on top
    unordered_map<Position, Node> all_nodes; //contains every Nodes that has or needs to be searched

    open_set.push({start, 0, heuristic(start, goal),nullptr});//open_set initial value(can't be empty unless search has done)  
    all_nodes[start] = {start, 0, heuristic(start, goal),nullptr};//all_nodes initial value

    while (!open_set.empty()){
        Node current = open_set.top();//current Node = top of the open_set queue that has lowest total_cost
        open_set.pop(); // delete the current node from open_set since we don't need to iterate again

        if (current.position == goal){ //when path search finished, go back to the parents Node(nullptr) and save every
        //Nodes in path vector. return REVERSED order of path since path[0] should be current position and last index to goal.
            vector<Position> path = {};
            while(!(current.parent == nullptr)){
                path.push_back(current.position);
                current = *current.parent; 
            }
            reverse(path.begin(),path.end());
            return path;
        }

        for(const auto& neighbor : get_neighbors(current.position, board)){
            if(!board.is_valid_position(neighbor)) continue; //if the given neighbor is not valid, continue
            //!!!make node before adding in the unordered_map or priority queue. cause infinite loop for same adrees on Parent Node!!!
            Node neighbor_node = {neighbor, current.cost + 1, heuristic(neighbor,goal),&all_nodes[current.position]};
            bool is_new_node = all_nodes.find(neighbor) == all_nodes.end();//check if it's not already in all_nodes map
            bool is_closer_to_goal = is_new_node || (current.cost+1) < all_nodes[neighbor].cost;//check new route is closer to goal
            if(is_closer_to_goal){ //if New Node is not in all_nodes or have better costs
                all_nodes[neighbor] = neighbor_node;//apppend new Nodes
                open_set.push(neighbor_node); //append new open_set
            }
        }
    }
    return {{-1,-1}}; //return empty path if no possible path found
}

//based on vector<Position> from a_star, decide next move
int direction(const Board& board, const Position& next_position){
    cout<<"Score : "<<board.snake.size()-1<<", next row : "<<next_position.row<<", next column : "<<next_position.column<<endl;
    if (next_position.column > board.get_head().column) {
        return 0; //when targetted column is bigger than current column -> move right
    } else if (next_position.column < board.get_head().column) {
        return 2; //when targetted column is smaller than current column -> move left
    } else if (next_position.row > board.get_head().row) {
        return 3; //when targetted row is bigger than current row -> move down
    } else if (next_position.row < board.get_head().row) {
        return 1; //when targetted row is smaller than current row -> move up
    }
    return -1;
}

// Simple wandering strategy
Position wandering_strategy(const Board& board) {
    vector<Position> possible_paths = get_neighbors(board.get_head(), board);
    // Filter out positions leading to immediate collision
    possible_paths.erase(remove_if(possible_paths.begin(), possible_paths.end(),
                                [&](const Position& pos) {
                                    return !board.is_valid_position(pos);
                                }),
                        possible_paths.end());

    if (!possible_paths.empty()) {
        // Select a random safe position to move to
        return *select_randomly(possible_paths.begin(), possible_paths.end());
    }

    // If no safe move is available, return an invalid position
    return {-1, -1};
}

vector<Position> bfs(const Position& start, const Position& goal, const Board& board) {
    queue<Position> q;
    map<Position, Position> parent;
    vector<Position> path;

    q.push(start);
    parent[start] = { -1, -1 }; // Mark the start position with a special value

    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        if (current == goal) {
            // Construct the path
            while (!(current == start)) {
                path.push_back(current);
                current = parent[current];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& neighbor : get_neighbors(current, board)) {
            if (parent.find(neighbor) == parent.end()) { // if not visited
                q.push(neighbor);
                parent[neighbor] = current;
            }
        }
    }

    return {{-1, -1}}; // return empty path if no path is found
}

// Modified choose_next_move
int choose_next_move(const Board& board) {
    vector<Position> search_result = a_star(board.get_head(), board.apple, board);
    if (search_result[0] != Position{-1, -1}) {
        return direction(board, search_result[0]);
    }

    // If A* fails, use BFS
    search_result = bfs(board.get_head(), board.apple, board);
    if (search_result[0] != Position{-1, -1}) {
        return direction(board, search_result[0]);
    }
    return 1;
    // ... rest of the code for other strategies
}