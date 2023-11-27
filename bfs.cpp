#include<bits/stdc++.h>
#include "player.h"

using namespace std;

struct Position_override {
    bool operator()(const Position& a, const Position& b) const {
        if (a.row == b.row) {
            return a.column < b.column;
        }
        return a.row < b.row;
    }
};

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

//BFS search
/* q
*  visited_paths <-> instead of Node struct, make previous list for recording it's connection this time
*  results_paths
*/
vector<Position> bfs(const Position& start, const Position& goal, const Board& board){
    queue<Position> q;
    map<Position,bool, Position_override> visited{};
    map<Position, Position,Position_override> previous{};

    q.push(start);
    visited[start] = true;
    
    //looping phase
    while(!q.empty()){
        Position current = q.front();
        q.pop();
        vector<Position> neighbors = get_neighbors(current, board);
        for(auto& neighbor : neighbors){
            if (visited[neighbor] == false){
                q.push(neighbor);
                visited[neighbor] = true;
                previous[neighbor] = current;
            }
        }
    }
    //pathfinding phase
    vector<Position> results;
    Position parent_node = goal;
    results.push_back(parent_node);
    bool flag = true;
    while(flag){
        if(previous.find(parent_node) == previous.end()){
            return {{-1, -1}};
        }
        parent_node = previous[parent_node];
        if (parent_node == start){
            results.push_back(parent_node);
            reverse(results.begin(),results.end());
            return results;
        }
        results.push_back(parent_node);
    }
    return {{-1,-1}};
}

vector<Position> survival_mode(const Position& start, const Board& board) {
    queue<Position> q;
    map<Position, int, Position_override> distance;//records distance between start-current possition
    map<Position, Position, Position_override> previous;

    q.push(start);
    distance[start] = 0;
    Position farthest_position = start;//records furthest position from start <=> keep updating end in bfs
    //looping phase
    while (!q.empty()) {
        Position current = q.front();
        q.pop();
        vector<Position> neighbors = get_neighbors(current, board);
        //looping phase (records distance)
        for (auto& neighbor : neighbors) {
            if (distance.find(neighbor) == distance.end()) {
                q.push(neighbor);
                distance[neighbor] = distance[current] + 1;
                previous[neighbor] = current;
                if (distance[neighbor] > distance[farthest_position]) {
                    farthest_position = neighbor;
                }
            }
        }
    }
    //pathfinding phase
    vector<Position> path;
    bool flag = 1;
    while(flag) {
        Position parent_node = farthest_position;
        while (parent_node != start) {
            path.push_back(parent_node);
            parent_node = previous[parent_node];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
        return path;
    }
    return {{-1,-1}};
}



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
    return 1;
}

int choose_next_move(const Board& board) {
    vector<Position> path = bfs(board.get_head(), board.apple, board);
    int result;
    if (path[0] != Position{-1, -1}){
        result = direction(board,path[1]);
        return result;
    }
    else{
        path = survival_mode(board.get_head(),board);
        result = direction(board,path[1]);
        return result;
    }
    return 1;
}