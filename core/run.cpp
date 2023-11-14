#include "engine.h"
#include "player.h"
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>

using namespace std;

const char* F_NORMAL = "\033[0m";
const char* F_RED = "\033[91m";
const char* F_GREEN = "\033[92m";
const char* F_DARK_GREEN = "\033[32m";

void print_state(const Engine& ge) {
    cout << endl;
    for (const string& row : ge.cells) {
        cout << " ";
        for (const char& ch : row) {
            if (ch == CELL_APPLE) {
                cout << F_RED;
            } else if (ch == CELL_SNAKE) {
                cout << F_DARK_GREEN;
            } else if (ch == CELL_HEAD) {
                cout << F_GREEN;
            }
            cout << ch << F_NORMAL;
        }
        cout << endl;
    }
    cout << endl;
}

const int num_arg = 8;

int main(int argc, char **argv) {
    assert(argc == num_arg);
    int ai = 1;
    int num_rows = atoi(argv[ai++]);
    int num_columns = atoi(argv[ai++]);
    int num_turns = atoi(argv[ai++]);
    int seed = atoi(argv[ai++]);
    int verbose = atoi(argv[ai++]);
    int sleep_ms = atoi(argv[ai++]);
    char* score_file = argv[ai++];
    assert(ai == num_arg);
    assert(num_rows >= 3 && num_columns >= 3);
    assert(verbose == 0 || verbose == 1);
    assert(sleep_ms >= 0);

    Engine ge(num_rows, num_columns, num_turns, seed);
    while (ge.finished_msg == "") {
        if (verbose)
            print_state(ge);
        ge.process_move_command(choose_next_move(ge));
        if (verbose)
            this_thread::sleep_for(chrono::milliseconds(sleep_ms));
    }

    if (verbose) {
        cout << ge.finished_msg << endl;
        cout << "スコア: " << ge.score << endl;
    }

    ofstream file;
    file.open(score_file, ofstream::app);
    file << ge.score << endl;
}
