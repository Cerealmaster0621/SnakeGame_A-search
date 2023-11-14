#include "engine.h"

using namespace std;

Engine::Engine(int _num_rows, int _num_columns, int _num_turns, int seed) : Board(_num_rows, _num_columns), num_turns(_num_turns), turn(0), score(0), finished_msg(), mt_engine(seed) {
    move_head(get_random_position());
    place_apple();
}

char& Engine::get_cell(const Position& p) {
    return cells[p.row][p.column];
}

Position Engine::get_random_position() {
    int row = uniform_int_distribution<>(0, num_rows - 1)(mt_engine);
    int column = uniform_int_distribution<>(0, num_columns - 1)(mt_engine);
    return {row, column};
}

void Engine::place_apple() {
    if ((int) snake.size() < num_rows * num_columns) {
        do {
            apple = get_random_position();
        } while (get_cell(apple) != CELL_EMPTY);
        get_cell(apple) = CELL_APPLE;
    } else {
        finished_msg = "りんごを置く場所がなくなりました";
    }
}

void Engine::move_tail() {
    get_cell(snake.back()) = CELL_EMPTY;
    snake.pop_back();
}

void Engine::move_head(const Position& where) {
    if (!snake.empty()) {
        get_cell(snake.front()) = CELL_SNAKE;
    }
    get_cell(where) = CELL_HEAD;
    snake.push_front(where);
}

void Engine::process_move_command(int d) {
    ++turn;
    if (d < 0 || d >= 4) {
        finished_msg = "無効な操作（" + to_string(d) + "）をしました";
        return;
    }
    Position where = get_head_after_move(d);
    if (!is_valid_position(where)) {
        finished_msg = "蛇が壁にぶつかりました";
        return;
    }
    if (get_cell(where) == CELL_APPLE) {
        move_head(where);
        ++score;
        place_apple();
    } else {
        move_tail();
        if (get_cell(where) == CELL_EMPTY) {
            move_head(where);
        } else {
            finished_msg = "蛇が自体にぶつかりました";
        }
    }
    if (turn == num_turns) {
        finished_msg = "最後のターンが終わりました";
    }
}
