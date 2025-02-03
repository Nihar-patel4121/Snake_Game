#include <iostream>
#include <vector>
#include <ctime>  // For seeding the random number generator

#if defined (_WIN32) || (_WIN64)
    #include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <ctime>  // For seeding the random number generator

using namespace std;

class SnakeGame {
public:
    bool gameover, paused;
    const int height = 25, width = 75;  // Making the grid square-shaped
    int tailX[100], tailY[100], nTail;
    int x, y;  // Snake head position
    int fruitX, fruitY; // Fruit position
    int score, level;
    int speed; // Initial speed
    int highScore;  // Track the highest score
    vector<pair<int, int>> obstacles; // Stores obstacle positions
    enum eDirection { STOP = 0, UP, LEFT, DOWN, RIGHT };
    eDirection dir;

    SnakeGame() {
        gameover = false;
        paused = false;
        dir = STOP;
        level = 1;
        speed = 150; // Initial speed
        score = 0;
        nTail = 0;
        highScore = 0;  // Initialize high score to 0
        srand(time(0)); // Seed the random number generator
    }

    void Setup() {
        gameover = false;
        paused = false;
        dir = STOP;
        x = width / 2;
        y = height / 2;
        fruitX = rand() % (width - 2) + 1;
        fruitY = rand() % (height - 2) + 1;
        nTail = 0;
        score = 0;
        level = 1;
        speed = 150; // Initial speed
        obstacles.clear();
    }

    void GenerateObstacle() {
        obstacles.clear();
        int numObstacles = level * 5;
        for (int i = 0; i < numObstacles; i++) {
            int obsX, obsY;
            do {
                obsX = rand() % width;
                obsY = rand() % height;
            } while ((obsX == x && obsY == y) || (obsX == fruitX && obsY == fruitY));
            obstacles.push_back({obsX, obsY});
        }
    }

    void Draw() {
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        // Draw the game grid
        for (int i = 0; i < height + 3; i++) {  
            for (int j = 0; j < width + 3; j++) {  // Adjust width to fit the square grid
                if (i == 0 || i == height + 2 || j == 0 || j == width + 2) {
                    cout << "\033[31m#\033[0m";  // Red for grid walls
                } else if (i == y + 1 && j == x + 1) {
                    cout << "\033[32mO\033[0m";  // Snake head in green
                } else if (i == fruitY + 1 && j == fruitX + 1) {
                    cout << "\033[33mF\033[0m";  // Fruit in yellow
                } else {
                    bool print = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j - 1 && tailY[k] == i - 1) {
                            cout << "\033[32mo\033[0m";  // Snake tail in green
                            print = true;
                            break;
                        }
                    }
                    if (!print) {
                        bool isObstacle = false;
                        for (auto obs : obstacles) {
                            if (obs.first == j - 1 && obs.second == i - 1) {
                                cout << "\033[34mX\033[0m";  // Obstacles in blue
                                isObstacle = true;
                                break;
                            }
                        }
                        if (!isObstacle) cout << " ";  // Empty space
                    }
                }
            }

            // Display instructions on the right side
            if (i == 2) cout << "  Controls:";
            if (i == 4) cout << "  W - Move Up";
            if (i == 5) cout << "  A - Move Left";
            if (i == 6) cout << "  S - Move Down";
            if (i == 7) cout << "  D - Move Right";
            if (i == 9) cout << "  P - Pause Game";
            if (i == 10) cout << "  J - Resume Game";
            if (i == 11) cout << "  X - Restart Game";
            if (i == 13) cout << "  F - Eat Fruit (+10 Points)";
            if (i == 15) cout << "  X - Obstacles (Avoid)";
            if (i == 17) cout << "  Score: " << score;
            if (i == 18) cout << "  Level: " << level;

            cout << endl;
        }

        // Print the welcome message below the grid
        cout << "\n\033[34mWelcome, Snake Commander!\033[0m" << endl;  // Blue
cout << "\033[32mYour mission: Eat, grow, and survive.\033[0m" << endl;  // Green
cout << "\033[33mLet's see how far your instincts can take you!\033[0m" << endl;  // Yellow

        // Print the rough message
        cout << "\033[31mLooks like it's going to be a bumpy ride...\033[0m" << endl << endl;
    }

    void Input() {
        if (_kbhit()) {
            char current = _getch();
            switch (current) {
                case 'w': if (dir != DOWN) dir = UP; break;
                case 'a': if (dir != RIGHT) dir = LEFT; break;
                case 's': if (dir != UP) dir = DOWN; break;
                case 'd': if (dir != LEFT) dir = RIGHT; break;
                case 'x': gameover = true; break;
                case 'p': paused = true; break;
                case 'j': if (paused) paused = false; break;
            }
        }
    }

    void Logic() {
        if (paused) return;

        int prevX = tailX[0], prevY = tailY[0];
        tailX[0] = x;
        tailY[0] = y;
        int prev2X, prev2Y;

        for (int i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        switch (dir) {
            case UP: y--; break;
            case DOWN: y++; break;
            case LEFT: x--; break;
            case RIGHT: x++; break;
            default: break;
        }

        if (x < 0 || x >= width || y < 0 || y >= height+1) gameover = true;

        for (int i = 0; i < nTail; i++) {
            if (x == tailX[i] && y == tailY[i]) gameover = true;
        }

        for (auto obs : obstacles) {
            if (x == obs.first && y == obs.second) gameover = true;
        }

        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % (width - 2) + 1;
            fruitY = rand() % (height - 2) + 1;
            nTail++;

            if (score % 50 == 0) {
                level++;
                speed = max(10, speed - 15);
                GenerateObstacle();
            }

            if (score > highScore) {
                highScore = score;
            }
        }
    }

    void RatePerformance() {
        string performance;

        if (level <= 2) {
            performance = "Poor";
            cout << "\033[31mYour Performance: Poor\033[0m" << endl;
            cout << "You need to get past level 2 to do better!" << endl;
        } else if (level > 2 && level <= 5) {
            performance = "Average";
            cout << "\033[33mYour Performance: Average\033[0m" << endl;
            cout << "Not bad, but you can do better!" << endl;
        } else {
            performance = "Excellent";
            cout << "\033[32mYour Performance: Excellent\033[0m" << endl;
            cout << "Great job! You're a Snake Commander!" << endl;
        }
    }

    void GameLoop() {
        Setup();
        while (!gameover) {
            Draw();
            Input();
            Logic();
            Sleep(speed);
        }
    }
};

int main() {
    SnakeGame game;
    while (true) {
        game.GameLoop();

        cout << "Game Over! Final Score: " << game.score << " | Reached Level: " << game.level << endl;
        game.RatePerformance();  // Display performance based on final score
        int k;
        cout << "Press 1 to Exit, 2 to Replay: ";
        cin >> k;
        if (k == 1) {
            cout << "Highest Score: " << game.highScore << endl;

            cout << "\n\tTHANKS FOR PLAYING\n";
            return 0;
        }
    }
}

#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #define CLR "clear"

    int _kbhit() {
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF) {
            ungetc(ch, stdin);
            return 1;
        }
        return 0;
    }

    int _getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }

using namespace std;

class SnakeGame {
public:
    bool gameover, paused;
    const int height = 25, width = 75;  // Making the grid square-shaped
    int tailX[100], tailY[100], nTail;
    int x, y;  // Snake head position
    int fruitX, fruitY; // Fruit position
    int score, level;
    int speed; // Initial speed
    int highScore;  // Track the highest score
    vector<pair<int, int>> obstacles; // Stores obstacle positions
    enum eDirection { STOP = 0, UP, LEFT, DOWN, RIGHT };
    eDirection dir;

    SnakeGame() {
        gameover = false;
        paused = false;
        dir = STOP;
        level = 1;
        speed = 150; // Initial speed
        score = 0;
        nTail = 0;
        highScore = 0;  // Initialize high score to 0
        srand(time(0)); // Seed the random number generator
    }

    void Setup() {
        gameover = false;
        paused = false;
        dir = STOP;
        x = width / 2;
        y = height / 2;
        fruitX = rand() % (width - 2) + 1;
        fruitY = rand() % (height - 2) + 1;
        nTail = 0;
        score = 0;
        level = 1;
        speed = 150; // Initial speed
        obstacles.clear();
    }

    void GenerateObstacle() {
        obstacles.clear();
        int numObstacles = level * 5;
        for (int i = 0; i < numObstacles; i++) {
            int obsX, obsY;
            do {
                obsX = rand() % width;
                obsY = rand() % height;
            } while ((obsX == x && obsY == y) || (obsX == fruitX && obsY == fruitY));
            obstacles.push_back({obsX, obsY});
        }
    }

    void Draw() {
        // COORD coord = {0, 0};
        // SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        // cout << "\033[H\033[j";

        system(CLR);

        // Draw the game grid
        for (int i = 0; i < height + 3; i++) {  
            for (int j = 0; j < width + 3; j++) {  // Adjust width to fit the square grid
                if (i == 0 || i == height + 2 || j == 0 || j == width + 2) {
                    cout << "\033[31m#\033[0m";  // Red for grid walls
                } else if (i == y + 1 && j == x + 1) {
                    cout << "\033[32mO\033[0m";  // Snake head in green
                } else if (i == fruitY + 1 && j == fruitX + 1) {
                    cout << "\033[33mF\033[0m";  // Fruit in yellow
                } else {
                    bool print = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j - 1 && tailY[k] == i - 1) {
                            cout << "\033[32mo\033[0m";  // Snake tail in green
                            print = true;
                            break;
                        }
                    }
                    if (!print) {
                        bool isObstacle = false;
                        for (auto obs : obstacles) {
                            if (obs.first == j - 1 && obs.second == i - 1) {
                                cout << "\033[34mX\033[0m";  // Obstacles in blue
                                isObstacle = true;
                                break;
                            }
                        }
                        if (!isObstacle) cout << " ";  // Empty space
                    }
                }
            }

            // Display instructions on the right side
            if (i == 2) cout << "  Controls:";
            if (i == 4) cout << "  W - Move Up";
            if (i == 5) cout << "  A - Move Left";
            if (i == 6) cout << "  S - Move Down";
            if (i == 7) cout << "  D - Move Right";
            if (i == 9) cout << "  P - Pause Game";
            if (i == 10) cout << "  J - Resume Game";
            if (i == 11) cout << "  X - Restart Game";
            if (i == 13) cout << "  F - Eat Fruit (+10 Points)";
            if (i == 15) cout << "  X - Obstacles (Avoid)";
            if (i == 17) cout << "  Score: " << score;
            if (i == 18) cout << "  Level: " << level;

            cout << endl;
        }

        // Print the welcome message below the grid
        cout << "\n\033[34mWelcome, Snake Commander!\033[0m" << endl;  // Blue
cout << "\033[32mYour mission: Eat, grow, and survive.\033[0m" << endl;  // Green
cout << "\033[33mLet's see how far your instincts can take you!\033[0m" << endl;  // Yellow

        // Print the rough message
        cout << "\033[31mLooks like it's going to be a bumpy ride...\033[0m" << endl << endl;
    }

    void Input() {
        if (_kbhit()) {
            char current = _getch();
            switch (current) {
                case 'w': if (dir != DOWN) dir = UP; break;
                case 'a': if (dir != RIGHT) dir = LEFT; break;
                case 's': if (dir != UP) dir = DOWN; break;
                case 'd': if (dir != LEFT) dir = RIGHT; break;
                case 'x': gameover = true; break;
                case 'p': paused = true; break;
                case 'j': if (paused) paused = false; break;
            }
        }
    }

    void Logic() {
        if (paused) return;

        int prevX = tailX[0], prevY = tailY[0];
        tailX[0] = x;
        tailY[0] = y;
        int prev2X, prev2Y;

        for (int i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        switch (dir) {
            case UP: y--; break;
            case DOWN: y++; break;
            case LEFT: x--; break;
            case RIGHT: x++; break;
            default: break;
        }

        if (x < 0 || x >= width || y < 0 || y >= height+1) gameover = true;

        for (int i = 0; i < nTail; i++) {
            if (x == tailX[i] && y == tailY[i]) gameover = true;
        }

        for (auto obs : obstacles) {
            if (x == obs.first && y == obs.second) gameover = true;
        }

        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % (width - 2) + 1;
            fruitY = rand() % (height - 2) + 1;
            nTail++;

            if (score % 50 == 0) {
                level++;
                speed = max(10, speed - 15);
                GenerateObstacle();
            }

            if (score > highScore) {
                highScore = score;
            }
        }
    }

    void RatePerformance() {
        string performance;

        if (level <= 2) {
            performance = "Poor";
            cout << "\033[31mYour Performance: Poor\033[0m" << endl;
            cout << "You need to get past level 2 to do better!" << endl;
        } else if (level > 2 && level <= 5) {
            performance = "Average";
            cout << "\033[33mYour Performance: Average\033[0m" << endl;
            cout << "Not bad, but you can do better!" << endl;
        } else {
            performance = "Excellent";
            cout << "\033[32mYour Performance: Excellent\033[0m" << endl;
            cout << "Great job! You're a Snake Commander!" << endl;
        }
    }

    void GameLoop() {
        Setup();
        while (!gameover) {
            Draw();
            Input();
            Logic();
            #if defined (_WIN32) || (_WIN64)
                Sleep(speed);
            #else
                usleep(speed * 700);
            #endif
        }
    }
};

int main() {
    SnakeGame game;
    while (true) {
        game.GameLoop();

        cout << "Game Over! Final Score: " << game.score << " | Reached Level: " << game.level << endl;
        game.RatePerformance();  // Display performance based on final score
        int k;
        cout << "Press 1 to Exit, 2 to Replay: ";
        cin >> k;
        if (k == 1) {
            cout << "Highest Score: " << game.highScore << endl;

            cout << "\n\tTHANKS FOR PLAYING\n";
            return 0;
        }
    }
}

#endif