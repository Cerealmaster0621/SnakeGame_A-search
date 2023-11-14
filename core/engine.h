#pragma once

#include "game.h"
#include <random>
#include <string>

struct Engine : Board {
    int num_turns, turn, score;
    std::string finished_msg;
    std::mt19937 mt_engine;

    Engine(int num_rows, int num_columns, int time_limit, int seed);

    char& get_cell(const Position& p);

    Position get_random_position();

    void place_apple();

    void move_tail();

    void move_head(const Position& where);

    void process_move_command(int d);
};
