#include <iostream>
#include <windows.h>
#include <vector>
#include <ctime>
using namespace std;//meow
const int WIDTH = 30;
const int HEIGHT = 20;
bool gameOver;
int score;
int fruitX, fruitY;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

struct Point { int x, y; };
vector<Point> snake;

void Setup() {
    gameOver = false;
    dir = RIGHT;
    score = 0;
    snake.clear();
    snake.push_back({WIDTH / 2, HEIGHT / 2});
    srand((unsigned)time(nullptr));
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
}

void Draw() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    for (int i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << endl;
    for (int y = 0; y < HEIGHT; y++) {
        cout << "#";
        for (int x = 0; x < WIDTH; x++) {
            bool print = false;
            if (x == fruitX && y == fruitY) { cout << "F"; print = true; }
            for (int i = 0; i < snake.size(); i++)
                if (snake[i].x == x && snake[i].y == y) {
                    cout << (i == 0 ? "O" : "o");
                    print = true;
                    break;
                }
            if (!print) cout << " ";
        }
        cout << "#\n";
    }
    for (int i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << "\nScore: " << score << "\n";
}

void Input() {
    if (GetAsyncKeyState('A') & 0x8000) dir = LEFT;
    else if (GetAsyncKeyState('D') & 0x8000) dir = RIGHT;
    else if (GetAsyncKeyState('W') & 0x8000) dir = UP;
    else if (GetAsyncKeyState('S') & 0x8000) dir = DOWN;
    else if (GetAsyncKeyState('X') & 0x8000) gameOver = true;
}

void Logic() {
    Point prev = snake[0];
    Point newHead = prev;

    switch (dir) {
    case LEFT: newHead.x--; break;
    case RIGHT: newHead.x++; break;
    case UP: newHead.y--; break;
    case DOWN: newHead.y++; break;
    default: break;
    }

    if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT) {
        gameOver = true; return;
    }

    for (auto &s : snake) if (s.x == newHead.x && s.y == newHead.y) {
        gameOver = true; return;
    }

    snake.insert(snake.begin(), newHead);

    if (newHead.x == fruitX && newHead.y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
    } else {
        snake.pop_back();
    }
}

int main() {
    Setup();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(120);
    }

    cout << "\nGame Over!\nFinal Score: " << score << endl;
    return 0; }

  
