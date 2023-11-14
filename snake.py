#!/usr/bin/python3

from datetime import datetime
from pathlib import Path
import argparse
import getpass
import shutil
import subprocess
import sys

score_file = Path('core/score.txt')
player_file = Path('core/player.cpp')
high_score_file = Path('high_score.txt')

def check(condition, error_message):
    if not condition:
        print(error_message)
        sys.exit(1)

def read_file(file):
    with open(file) as f:
        return f.readlines()

def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('mode', choices=['compile', 'run', 'score'], help='モード')
    parser.add_argument('file', type=Path, help='操作を決める関数が入っているファイル')
    parser.add_argument('--rows', type=int, default=4, help='runモードで行数を指定する（デフォルト値: 4）')
    parser.add_argument('--columns', type=int, default=6, help='runモードで列数を指定する（デフォルト値: 6）')
    parser.add_argument('--turns', type=int, default=1000, help='runモードでターン数を指定する（デフォルト値: 1000）')
    parser.add_argument('--seed', type=int, default=47, help='runモードで乱数の種を指定する（デフォルト値: 47）')
    parser.add_argument('--speed', type=float, default=1.0, help='runモードで再生速度を指定する（デフォルト値: 1.0）')
    parser.add_argument('--show-details', action='store_true', help='scoreモードで細かい評価を表示する')
    parser.add_argument('--precise', action='store_true', help='scoreモードでより精度の高い評価を計算する')
    global args
    args = parser.parse_args()

    check(args.rows >= 3, '行数は3以上でなければいけません')
    check(args.columns >= 3, '列数は3以上でなければいけません')
    check(args.turns >= 0, 'ターン数は0以上でなければいけません')
    check(args.speed > 0, '再生速度は正の実数でなければいけません')

def replace_player_file(source):
    check(source.is_file(), 'エラー: 指定されたファイルが見つかりません')
    if player_file.is_file() and read_file(source) == read_file(player_file):
        return False
    else:
        shutil.copy(source, player_file)
        return True

def run(num_rows, num_columns, num_turns, seed, batch_mode):
    runargs = [num_rows, num_columns, num_turns, seed, int(not batch_mode), int(100 / args.speed), score_file]
    res = subprocess.run(['./core/run'] + list(map(str, runargs)), capture_output=batch_mode)
    if batch_mode:
        check(res.returncode == 0, 'エラー: 実行時エラーが発生しました')

def compute_score(num_repeats):
    print('評価を行います')
    if args.show_details:
        print()
        print('(行数, 列数, ターン数)\t得点')

    seed = 0
    total_score = 0
    total_weight = 0

    worstcase = lambda x, y: (x * y) ** 2
    bestcase = lambda x, y: x * y * (x + y) // 2

    small_sizes = ((3, 3), (3, 4), (4, 3), (3, 5), (5, 3), (4, 4), (3, 8), (8, 3), (4, 6), (6, 4), (5, 5))
    large_sizes = ((4, 8), (8, 4), (5, 7), (7, 5), (6, 6), (5, 8), (8, 5), (6, 8), (8, 6), (7, 7))

    batches = (
        (1, [(x, y, worstcase(x, y) * 10) for x, y in small_sizes]),
        (2, [(x, y, worstcase(x, y)) for x, y in small_sizes]),
        (3, [(x, y, bestcase(x, y)) for x, y in small_sizes]),
        (4, [(x, y, bestcase(x, y) * 3 // 4) for x, y in small_sizes]),
        (2, [(x, y, worstcase(x, y) * 3) for x, y in large_sizes]),
        (4, [(x, y, worstcase(x, y)) for x, y in large_sizes]),
        (6, [(x, y, bestcase(x, y)) for x, y in large_sizes]),
        (8, [(x, y, bestcase(x, y) // 2) for x, y in large_sizes]),
    )

    for weight, batch in batches:
        total_weight += weight
        for test_case in batch:
            score_file.unlink(missing_ok=True)
            for _ in range(num_repeats):
                run(*test_case, seed, True)
                seed += 1
            score = sum(map(int, read_file(score_file)))
            max_score = (test_case[0] * test_case[1] - 1) * num_repeats
            if args.show_details:
                completion_rate = int(score / max_score * 100)
                print(f'{test_case}\t\t{completion_rate:3d}%')
            total_score += score / max_score / len(batch) * weight

    score_file.unlink(missing_ok=True)
    total_score = int(total_score * 100 / total_weight)
    if args.show_details:
        print()
    return total_score

def score():
    if args.precise:
        total_score = compute_score(50)
        print(f'評価結果（高い精度）: {total_score}%')
    else:
        total_score = compute_score(5)
        print(f'評価結果: {total_score}%')
        if high_score_file.is_file():
            with open(high_score_file) as f:
                high_score = int(f.readline())
        else:
            high_score = -1
        if total_score > high_score:
            print('記録が更新されました！')
            with open(high_score_file, 'w') as f:
                f.write(f'{total_score}\n{datetime.now()}\n{getpass.getuser()}\n\n')
                f.writelines(read_file(args.file))

def main():
    parse_arguments()
    if replace_player_file(args.file):
        print('指定されたファイルをコンパイルします')
        if subprocess.run(['make', '-C', 'core']).returncode != 0:
            print('エラー: コンパイルに失敗しました')
            player_file.unlink()
            sys.exit(1)
        print('コンパイルに成功しました')
    else:
        print('指定されたファイルが前回と同じ内容なのでコンパイルは省きます')
    if args.mode == 'run':
        run(args.rows, args.columns, args.turns, args.seed, False)
    elif args.mode == 'score':
        score()

try:
    main()
except KeyboardInterrupt:
    print('実行が中止されました')
    sys.exit(1)
