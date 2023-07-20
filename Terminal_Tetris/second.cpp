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

bool IsPositionValid(int shapeIndex, int rotationIndex, int x, int y)
{
    const vector<vector<int>> &tetromino = tetrominos[shapeIndex][rotationIndex];
    for (size_t i = 0; i < tetromino.size(); i++)
    {
        for (size_t j = 0; j < tetromino[i].size(); j++)
        {
            if (tetromino[i][j] && (x + j < 0 || x + j >= width || y + i >= height || board[y + i][x + j] != ' '))
                return false;
        }
    }
    return true;
}

void DrawBoard()
{
    system("clear");

    // Print instructions
    cout << "Terminal Tetris" << endl;
    cout << "______________" << endl;
    cout << "Use 'a', 'd' to move left and right." << endl;
    cout << "Use 's' to move down faster." << endl;
    cout << "Use 'w' to rotate the piece." << endl;
    cout << "Press 'esc' to quit." << endl;
    cout << endl;

    // Draw game board
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                cout << "#";

            cout << board[i][j];

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
}

void PlaceTetromino()
{
    const vector<vector<int>> &tetromino = tetrominos[currentTetromino.shapeIndex][currentTetromino.rotationIndex];
    for (size_t i = 0; i < tetromino.size(); i++)
    {
        for (size_t j = 0; j < tetromino[i].size(); j++)
        {
            if (tetromino[i][j])
                board[currentTetromino.y + i][currentTetromino.x + j] = tetrominoColors[currentTetromino.shapeIndex];
        }
    }
}

void RemoveTetromino()
{
    const auto &tetromino = tetrominos[currentTetromino.shapeIndex][currentTetromino.rotationIndex];
    for (size_t i = 0; i < tetromino.size(); i++)
    {
        for (size_t j = 0; j < tetromino[i].size(); j++)
        {
            if (tetromino[i][j])
                board[currentTetromino.y + i][currentTetromino.x + j] = ' ';
        }
    }
}

void ResetTetromino()
{
    currentTetromino.shapeIndex = rand() % tetrominos.size();
    currentTetromino.rotationIndex = rand() % tetrominos[currentTetromino.shapeIndex].size();
    currentTetromino.x = width / 2 - tetrominos[currentTetromino.shapeIndex][currentTetromino.rotationIndex][0].size() / 2;
    currentTetromino.y = 0;
}

bool MoveTetromino(int dx, int dy, int dr)
{
    RemoveTetromino();
    if (IsPositionValid(currentTetromino.shapeIndex, (currentTetromino.rotationIndex + dr) % tetrominos[currentTetromino.shapeIndex].size(), currentTetromino.x + dx, currentTetromino.y + dy))
    {
        currentTetromino.x += dx;
        currentTetromino.y += dy;
        currentTetromino.rotationIndex = (currentTetromino.rotationIndex + dr) % tetrominos[currentTetromino.shapeIndex].size();
        return true;
    }
    PlaceTetromino();
    return false;
}

void CheckLines()
{
    for (int i = height - 1; i >= 0; i--)
    {
        bool lineIsFull = true;
        for (int j = 0; j < width; j++)
        {
            if (board[i][j] == ' ')
            {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull)
        {
            score += 100;
            for (int k = i; k > 0; k--)
            {
                for (int j = 0; j < width; j++)
                {
                    board[k][j] = board[k - 1][j];
                }
            }
            for (int j = 0; j < width; j++)
            {
                board[0][j] = ' ';
            }
        }
    }
}

bool IsGameOver()
{
    return !IsPositionValid(currentTetromino.shapeIndex, currentTetromino.rotationIndex, currentTetromino.x, currentTetromino.y);
}

int main()
{
    // Initialize ncurses
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    // Seed the random number generator for random tetromino shapes
    srand(time(NULL));

    isGameOver = false;
    score = 0;

    while (!isGameOver)
    {
        DrawBoard();
        usleep(50000); // Sleep for 0.05 seconds (50,000 microseconds) for smooth gameplay

        int ch = getch();
        switch (ch)
        {
        case 'a': // Move left
        case KEY_LEFT:
            MoveTetromino(-1, 0, 0);
            break;
        case 'd': // Move right
        case KEY_RIGHT:
            MoveTetromino(1, 0, 0);
            break;
        case 's': // Move down faster
        case KEY_DOWN:
            if (!MoveTetromino(0, 1, 0))
            {
                PlaceTetromino();
                CheckLines();
                ResetTetromino();
                isGameOver = IsGameOver();
            }
            break;
        case 'w': // Rotate
        case KEY_UP:
            MoveTetromino(0, 0, 1);
            break;
        case 27:               // Escape key (ASCII value 27)
            isGameOver = true; // Set isGameOver to true to quit the game
            break;
        default:
            break;
        }
    }

    // End ncurses mode
    endwin();

    cout << "Game Over!" << endl;
    cout << "Your Score: " << score << endl;
    return 0;
}
