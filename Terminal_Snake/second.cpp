#include <iostream>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int tailLength;
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;

// Function to set up the initial game state
void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    tailLength = 0;
}

// Function to handle user input
void Input()
{
    struct termios oldSettings, newSettings;
    tcgetattr(0, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newSettings);

    char ch;
    if (read(0, &ch, 1) >= 0)
    {
        switch (ch)
        {
        case 'a':
            if (dir != RIGHT) // Disallow reversing the direction
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT) // Disallow reversing the direction
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN) // Disallow reversing the direction
                dir = UP;
            break;
        case 's':
            if (dir != UP) // Disallow reversing the direction
                dir = DOWN;
            break;
        case 27: // Escape key
            gameOver = true;
            break;
            
        }
    }

    tcsetattr(0, TCSANOW, &oldSettings);
}

// Function to update the game state
void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < tailLength; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case STOP:
        // No specific action for STOP
        break;
    }

    if (x < 0 || x >= width || y < 0 || y >= height)
        gameOver = true;

    for (int i = 0; i < tailLength; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tailLength++;
    }
}

// Function to render the game state
void Draw()
{
    system("clear");

    for (int i = 0; i < width + 2; i++)
        std::cout << "#";
    std::cout << std::endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                std::cout << "#";

            if (i == y && j == x)
                std::cout << "O";
            else if (i == fruitY && j == fruitX)
                std::cout << "F";
            else
            {
                bool isTail = false;
                for (int k = 0; k < tailLength; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        std::cout << "o";
                        isTail = true;
                    }
                }

                if (!isTail)
                    std::cout << " ";
            }

            if (j == width - 1)
                std::cout << "#";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < width + 2; i++)
        std::cout << "#";
    std::cout << std::endl;

    std::cout << "Score: " << score << std::endl;
}

int main()
{
    Setup();

    while (!gameOver)
    {
        Draw();
        Input();
        Logic();

        // Wait for 100 milliseconds before updating the game state
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}