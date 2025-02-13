#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>  // For seeding the random number generator

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
    const int height = 40, width = 40;  // Making the grid square-shaped
    int tailX[100], tailY[100], nTail;  //o position
    int x, y;  // Snake head position    //O position
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
        speed = 150;
        score = 0;
        nTail = 2;
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
        nTail = 2;
        score = 0;
        level = 1;
        speed = 150;// Initial speed
        obstacles.clear();

        for(int i=0; i<nTail; i++) {
            tailX[i] = x - i - 1;
            tailY[i] = y;
        }
    }

    void GenerateObstacle() {
        obstacles.clear();
        int numObstacles = level * 3;
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
                        if (tailX[k] == j - 1  && tailY[k] == i - 1) {  //for joining O with o
                            cout << "\033[32mo\033[0m";  // Snake tail in green
                            print = true;
                            break;
                        }
                    }
                    if (!print) {
                        bool isObstacle = false;
                        for (auto obs : obstacles) {
                            if (obs.first == j  && obs.second == i ) {
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

            if (i == 2) cout << "   \033[34mW E L C O M E,  S N A K E  C O M M A N D E R!\033[0m";
            if (i == 4) cout << "   \033[32mYour mission: Eat, grow, and survive.\033[0m";
            if (i == 5) cout << "   \033[33mLet's see how far your instincts can take you!\033[0m";
            if (i == 6) cout << "   \033[31mLooks like it's going to be a bumpy ride...\033[0m";
            if (i == 9) cout << "\033[34m  Controls:\033[0m";
            if (i == 11) cout << "\033[32m  W - Move Up\033[0m";
            if (i == 12) cout << "\033[32m  A - Move Left\033[0m";
            if (i == 13) cout << "\033[32m  S - Move Down\033[0m";
            if (i == 14) cout << "\033[32m  D - Move Right\033[0m";
            if (i == 16) cout << "\033[35m  P - Pause Game\033[0m";       // Purple  
            if (i == 17) cout << "\033[36m  J - Resume Game\033[0m";      // Cyan  
            if (i == 18) cout << "\033[38;5;214m  X - Restart Game\033[0m"; // Orange  
            if (i == 20) cout << "\033[33m  F - Eat Fruit (+10 Points)\033[0m";
            if (i == 21) cout << "\033[31m  X - Obstacles (Avoid)\033[0m";
            if (i == 23) cout << "\033[37m  Score: " << score << "\033[0m";
            if (i == 24) cout << "\033[37m  Level: " << level << "\033[0m";

            
            cout << endl;
        }

        
    }

    void Input() {
        if (_kbhit()) {
            char current = _getch();
            if(dir != STOP) {
                switch (current) {
                    case 'w': if (dir != DOWN) dir = UP; break;
                    case 'a': if (dir != RIGHT) dir = LEFT; break;
                    case 's': if (dir != UP) dir = DOWN; break;
                    case 'd': if (dir != LEFT) dir = RIGHT; break;
                    case 'x': gameover = true; break;
                    case 'p': paused = true; break;
                    case 'r': if (paused) paused = false; break;
                }
            } else {
                if(current == 's') dir = DOWN;
                else if(current == 'w') dir = UP;
                else if(current == 'd') dir = RIGHT;
            }
        }
    }

    void Logic() {
        if (dir == STOP) { return; }
        if(paused) { return; }
    
        // Move the tail
        int prevX = tailX[0], prevY = tailY[0];
        tailX[0] = x;
        tailY[0] = y;
        int temp_prev2X, temp_prev2Y;
    
        for (int i = 1; i < nTail; i++) {
            temp_prev2X = tailX[i];
            temp_prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = temp_prev2X;
            prevY = temp_prev2Y;
        }
    
        // Move the head
        switch (dir) {
            case UP: y--; break;
            case DOWN: y++; break;
            case LEFT: x--; break;
            case RIGHT: x++; break;
            default: break;
        }
    
        // Check for collisions
        if (x < 0 || x >= width + 1 || y < 0 || y >= height + 1) gameover = true;
    
        for (int i = 0; i < nTail; i++) {
            if (x == tailX[i] && y == tailY[i]) gameover = true;
        }
    
        for (auto obs : obstacles) {
            if (x == obs.first - 1&& y == obs.second - 1) gameover = true;
        }
    
        // Check if the snake eats the fruit
        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % (width - 2) + 1;
            fruitY = rand() % (height - 2) + 1;
            nTail++;
    
            if (score % 50 == 0) {
                level++;
                speed = max(10, speed - 10);
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

};

int main() {
    int i = 0;
    int last_score = 0;
    vector<int> vec1;
    vector<int>vec2;
    while (true) {
        SnakeGame game;
        game.Setup();
    
        while (!game.gameover) {
            game.Draw();
            game.Input();
            game.Logic();
            Sleep(game.speed);
        }
        vec1.push_back(game.level);
        vec2.push_back(game.score);
        last_score = game.score;
        game.RatePerformance();  // Display performance based on final score
        cout << "\033[31mG A M E  O V E R !!\033[0m" << endl; // Red color
        cout << "\033[34mLast Game Final Score: " << last_score << "\033[0m" << endl; // Dark blue
        int k;
        cout << "Press 1 to Exit, 2 to Replay: ";
        cin >> k;
        i++;
        sort(vec1.begin(),vec1.end());
        sort(vec2.begin(),vec2.end());
        
        if (k == 1) {
            std::cout << "\033[33mHighest Score Achieved: " << vec2[i - 1] << "\n\033[0m"
            << "\033[38;5;214mHighest Level Achieved: " << vec1[i - 1] << "\033[0m";
  

            
            std::cout << "\n\n\t\033[38;5;214mT H A N K S  \033[0m \033[37mF O R\033[0m \033[32m  P L A Y I N G\033[0m\n\n";


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
        const int height = 40, width = 40;  // Making the grid square-shaped
        int tailX[100], tailY[100], nTail;  //o position
        int x, y;  // Snake head position    //O position
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
            speed = 150;
            score = 0;
            nTail = 2;
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
            nTail = 2;
            score = 0;
            level = 1;
            speed = 150;// Initial speed
            obstacles.clear();
    
            for(int i=0; i<nTail; i++) {
                tailX[i] = x - i - 1;
                tailY[i] = y;
            }
        }
    
        void GenerateObstacle() {
            obstacles.clear();
            int numObstacles = level * 3;
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
            //COORD coord = {0, 0};
            //SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            system("clear");
    
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
                            if (tailX[k] == j - 1  && tailY[k] == i - 1) {  //for joining O with o
                                cout << "\033[32mo\033[0m";  // Snake tail in green
                                print = true;
                                break;
                            }
                        }
                        if (!print) {
                            bool isObstacle = false;
                            for (auto obs : obstacles) {
                                if (obs.first == j  && obs.second == i ) {
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
    
                if (i == 2) cout << "   \033[34mW E L C O M E,  S N A K E  C O M M A N D E R!\033[0m";
                if (i == 4) cout << "   \033[32mYour mission: Eat, grow, and survive.\033[0m";
                if (i == 5) cout << "   \033[33mLet's see how far your instincts can take you!\033[0m";
                if (i == 6) cout << "   \033[31mLooks like it's going to be a bumpy ride...\033[0m";
                if (i == 9) cout << "\033[34m  Controls:\033[0m";
                if (i == 11) cout << "\033[32m  W - Move Up\033[0m";
                if (i == 12) cout << "\033[32m  A - Move Left\033[0m";
                if (i == 13) cout << "\033[32m  S - Move Down\033[0m";
                if (i == 14) cout << "\033[32m  D - Move Right\033[0m";
                if (i == 16) cout << "\033[35m  P - Pause Game\033[0m";       // Purple  
                if (i == 17) cout << "\033[36m  J - Resume Game\033[0m";      // Cyan  
                if (i == 18) cout << "\033[38;5;214m  X - Restart Game\033[0m"; // Orange  
                if (i == 20) cout << "\033[33m  F - Eat Fruit (+10 Points)\033[0m";
                if (i == 21) cout << "\033[31m  X - Obstacles (Avoid)\033[0m";
                if (i == 23) cout << "\033[37m  Score: " << score << "\033[0m";
                if (i == 24) cout << "\033[37m  Level: " << level << "\033[0m";
    
                
                cout << endl;
            }
    
            // Print the welcome message below the grid
            // cout << "\n\033[34mW E L C O M E,  S N A K E  C O M M A N D E R!\033[0m" << endl;  // Blue
            // cout << "\033[32mYour mission: Eat, grow, and survive.\033[0m" << endl;  // Green
            // cout << "\033[33mLet's see how far your instincts can take you!\033[0m" << endl;  // Yellow
    
            // // Print the rough message
            // cout << "\033[31mLooks like it's going to be a bumpy ride...\033[0m" << endl << endl;
        }
    
        void Input() {
            if (_kbhit()) {
                char current = _getch();
                if(dir != STOP) {
                    switch (current) {
                        case 'w': if (dir != DOWN) dir = UP; break;
                        case 'a': if (dir != RIGHT) dir = LEFT; break;
                        case 's': if (dir != UP) dir = DOWN; break;
                        case 'd': if (dir != LEFT) dir = RIGHT; break;
                        case 'x': gameover = true; break;
                        case 'p': paused = true; break;
                        case 'r': if (paused) paused = false; break;
                    }
                } else {
                    if(current == 's') dir = DOWN;
                    else if(current == 'w') dir = UP;
                    else if(current == 'd') dir = RIGHT;
                }
            }
        }
    
        void Logic() {
            if (dir == STOP) { return; }
            if(paused) { return; }
        
            // Move the tail
            int prevX = tailX[0], prevY = tailY[0];
            tailX[0] = x;
            tailY[0] = y;
            int temp_prev2X, temp_prev2Y;
        
            for (int i = 1; i < nTail; i++) {
                temp_prev2X = tailX[i];
                temp_prev2Y = tailY[i];
                tailX[i] = prevX;
                tailY[i] = prevY;
                prevX = temp_prev2X;
                prevY = temp_prev2Y;
            }
        
            // Move the head
            switch (dir) {
                case UP: y--; break;
                case DOWN: y++; break;
                case LEFT: x--; break;
                case RIGHT: x++; break;
                default: break;
            }
        
            // Check for collisions
            if (x < 0 || x >= width + 1 || y < 0 || y >= height + 1) gameover = true;
        
            for (int i = 0; i < nTail; i++) {
                if (x == tailX[i] && y == tailY[i]) gameover = true;
            }
        
            for (auto obs : obstacles) {
                if (x == obs.first && y == obs.second) gameover = true;
            }
        
            // Check if the snake eats the fruit
            if (x == fruitX && y == fruitY) {
                score += 10;
                fruitX = rand() % (width - 2) + 1;
                fruitY = rand() % (height - 2) + 1;
                nTail++;
        
                if (score % 50 == 0) {
                    level++;
                    speed = max(10, speed - 10);
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
    
    };
    
    int main() {
        int i = 0;
        int last_score = 0;
        vector<int> vec1;
        vector<int>vec2;
        while (true) {
            SnakeGame game;
            game.Setup();
        
            while (!game.gameover) {
                game.Draw();
                game.Input();
                game.Logic();
                usleep(game.speed*1000);
            }
            vec1.push_back(game.level);
            vec2.push_back(game.score);
            last_score = game.score;
            game.RatePerformance();  // Display performance based on final score
            cout << "\033[31mG A M E  O V E R !!\033[0m" << endl; // Red color
            cout << "\033[34mLast Game Final Score: " << last_score << "\033[0m" << endl; // Dark blue
            int k;
            cout << "Press 1 to Exit, 2 to Replay: ";
            cin >> k;
            i++;
            sort(vec1.begin(),vec1.end());
            sort(vec2.begin(),vec2.end());
            
            if (k == 1) {
                std::cout << "\033[33mHighest Score Achieved: " << vec2[i - 1] << "\n\033[0m"
                << "\033[38;5;214mHighest Level Achieved: " << vec1[i - 1] << "\033[0m";
      
    
                
                std::cout << "\n\n\t\033[38;5;214mT H A N K S  \033[0m \033[37mF O R\033[0m \033[32m  P L A Y I N G\033[0m\n\n";
    
    
                return 0;   
            }
        }
        
    }


#endif
