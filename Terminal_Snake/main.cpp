#include <iostream>
#include <unistd.h>
#include <termios.h>
using namespace std;
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;
void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}
void Draw()
{

    system("clear");

    // Print instructions
    cout << "Directions" << endl;
    cout << "__________" << endl;
    cout << "Use 'a', 'w', 's', 'd' to move." << endl;
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

            cout << " ";

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
}
void Input()
{

    struct termios oldSettings, newSettings;
    tcgetattr(0, &oldSettings); // Get current terminal settings
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO); // Turn off canonical mode and echoing
    tcsetattr(0, TCSANOW, &newSettings);     // Apply new terminal settings

    char ch;
    if (cin.get(ch) && ch == 27) // Check if the input is the Escape key (ASCII value 27)
        gameOver = true;         // Set gameOver to true to quit the game

    tcsetattr(0, TCSANOW, &oldSettings); // Restore original terminal settings
}
void Logic()
{
}
int main()
{
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        // Sleep(10)
        // usleep(10000);
    }
    return 0;
}
