#include <iostream>
#include <utility>
#include <queue>
#include <vector>

__attribute__((optimize("O3"))) int main() {
    int length = 0, jump_len = 0;
    char tmp;
    int shortest = -1;
    scanf("%d %d\n", &length, &jump_len);
    std::vector<std::vector<int>> array(2);
    for (int i = 0; i < 2; i++)
        array[i] = std::vector<int>(length);

    for (int y = 0; y < 2; y++)
        for (int x = 0; x < length; x++) {
            std::cin >> tmp;
            array[y][x] = (tmp == 'x' ? -1 : -2);
        }

    std::queue<std::pair<int, int>> Q;
    Q.emplace(0, 0);
    array[0][0] = 0;

    while (!Q.empty()) {
        auto actual = Q.front();
        Q.pop();

        // if the distance of end is jump or one slot
        if (actual.second + jump_len > length - 1 /*??*/ || actual.second + 1 >= length)
            if (shortest == -1 || array[actual.first][actual.second] + 1 < shortest)
                shortest = array[actual.first][actual.second] + 1;

        // check if diagonal move is unvisited
        if (actual.second + jump_len < length && array[actual.first ^ 1][actual.second + jump_len] == -2) {
            array[actual.first ^ 1][actual.second + jump_len] = array[actual.first][actual.second] + 1;
            Q.emplace(actual.first ^ 1, actual.second + jump_len);
        }
        // check if next slot is unvisited
        if (actual.second + jump_len < length && array[actual.first][actual.second + 1] == -2) {
            array[actual.first][actual.second + 1] = array[actual.first][actual.second] + 1;
            Q.emplace(actual.first, actual.second + 1);
        }
        // check if not on first slot && check if slot before is unvisited && check if I made less swaps then value of my index -1 ???
        if (actual.second > 0 && array[actual.first][actual.second - 1] == -2 &&
            array[actual.first][actual.second] < actual.second - 1) {
            array[actual.first][actual.second - 1] = array[actual.first][actual.second] + 1;
            Q.emplace(actual.first, actual.second - 1);
        }
    }
    std::cout << shortest << std::endl;
    return 0;
}