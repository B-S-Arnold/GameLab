#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <vector>
#include <ncurses.h>
#include <random>
using namespace std;

const int width = 12;
const int height = 20;
vector<vector<char>> board(height, vector<char>(width, ' '));

const vector<vector<vector<int>>> tetrominos = {
    {{1, 1, 1, 1}},         // I
    {{1, 1, 1}, {0, 1, 0}}, // T
    {{1, 1, 0}, {0, 1, 1}}, // Z
    {{0, 1, 1}, {1, 1, 0}}, // S
    {{1, 1}, {1, 1}},       // O
    {{1, 1, 1}, {0, 0, 1}}, // L
    {{1, 1, 1}, {1, 0, 0}}  // J
};

const vector<char> tetrominoColors = {'C', 'M', 'R', 'G', 'Y', 'B', 'W'};

struct Tetromino
{
    size_t shapeIndex;
    size_t rotationIndex;
    int x;
    int y;
};

Tetromino currentTetromino;

bool isGameOver;
int score;

