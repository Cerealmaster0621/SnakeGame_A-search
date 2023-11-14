#pragma once

#include <vector>
#include <deque>
#include <string>

using std::vector, std::deque, std::string;

// マスの位置を表す
struct Position {
    int row, column;  // 行・列の添字（左上は(0,0)）
};

// Position型用の演算子
Position operator + (const Position& a, const Position& b);
Position operator - (const Position& a, const Position& b);
bool operator == (const Position& a, const Position& b);

// aとbとのマンハッタン距離を計算する
int distance(const Position& a, const Position& b);

// マスの状態
const char CELL_EMPTY = '.';  // 空
const char CELL_APPLE = 'A';  // りんご
const char CELL_HEAD = 'H';   // 蛇の頭
const char CELL_SNAKE = 'S';  // 蛇の体（頭以外）

// 操作の番号
const int MOVE_RIGHT = 0;
const int MOVE_UP = 1;
const int MOVE_LEFT = 2;
const int MOVE_DOWN = 3;

// 操作の効果（蛇の頭がどこに移るか）
constexpr Position move_delta[4] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};

// 盤面の状況を表す
struct Board {
    int num_rows, num_columns;  // 行数、列数
    Position apple;             // りんごの位置
    deque<Position> snake;      // 蛇が占めるマスの配列（頭はsnake[0]、体はsnake[1]からsnake[snake.size()-1]まで）
    vector<string> cells;       // マスの行列（左上はcells[0][0]）

    // pが盤面内かを判定する
    bool is_valid_position(const Position& p) const;

    // pのマスの状態を返す
    const char& get_cell(const Position& p) const;

    // 蛇の頭の位置を返す
    const Position& get_head() const;

    // 操作mを行った時の蛇の頭の位置を返す
    Position get_head_after_move(int m) const;

    Board(int num_rows, int num_columns);
};
