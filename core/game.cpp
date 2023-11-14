#include "game.h"
#include <cstdlib>

Position operator + (const Position& a, const Position& b) {
    return {a.row + b.row, a.column + b.column};
}

Position operator - (const Position& a, const Position& b) {
    return {a.row - b.row, a.column - b.column};
}

bool operator == (const Position& a, const Position& b) {
    return a.row == b.row && a.column == b.column;
}

int distance(const Position& a, const Position& b) {
    Position delta = a - b;
    return abs(delta.row) + abs(delta.column);
}

bool Board::is_valid_position(const Position& p) const {
    return p.row >= 0 && p.row < num_rows && p.column >= 0 && p.column < num_columns;
}

const char& Board::get_cell(const Position& p) const {
    return cells[p.row][p.column];
}

const Position& Board::get_head() const {
    return snake.front();
}

Position Board::get_head_after_move(int m) const {
    return get_head() + move_delta[m];
}

Board::Board(int _num_rows, int _num_columns) : num_rows(_num_rows), num_columns(_num_columns), apple({-1, -1}) {
    cells.resize(num_rows, string(num_columns, CELL_EMPTY));
}
