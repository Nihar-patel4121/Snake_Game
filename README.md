# 🐍 Snake Game - C++ Project 🎮🚀

## 📌 Overview

This is a classic Snake Game with a modern twist 🔹🎮, implemented in C++. It features obstacles, levels, and a performance rating system ⭐, making the gameplay progressively challenging as the player advances.

---

## 🌟 Features

### 🎯 Grid-Based Gameplay
✅ The game operates on a structured grid where the snake moves in four directions.  
✅ Random fruit placement to increase the score.  

### 🐍 Snake Movement & Growth
✅ The snake is represented using an array, with new body segments added as it consumes fruits.  
✅ Collision detection with walls, obstacles, and itself.  

### 🔥 Levels & Difficulty Progression
✅ Speed increases after every 5 fruits eaten.  
✅ Random obstacles appear as difficulty increases.  
✅ The game becomes progressively more challenging.  

### ⭐ Performance Rating System
✅ Score system based on the number of fruits collected.  
✅ A rating system evaluates the player's performance.  

### 🎮 Game Controls
➡ Arrow keys (or WASD) for movement  
⏸ Pause and Resume functionality  

### 🖥 Optimized for Console Execution
✅ Designed for Online GDB and standard C++ compilers.  
✅ No `windows.h` dependency, ensuring portability.  

---

## ⚙ Technologies Used

- 🛠 C++ (Standard Library)  
- 📉 Basic Data Structures (Arrays for snake representation)  
- 🎢 Randomization (Fruit & obstacle placement)  

---

## 🫠 Code Design & Working

### 🐍 How the Snake Moves
- The snake is represented using an array, where each element stores a coordinate (x, y).  
- The head of the snake moves in the direction chosen by the player (WASD / Arrow keys).  
- The body follows the head by shifting positions.  
- The movement is controlled using a game loop, continuously updating the position of the snake.  

### 🍎 How Food Appears Randomly
- A random function generates an (x, y) coordinate for the fruit.  
- The function ensures that food does not appear on the snake's body or obstacles.  
- Each time the snake eats the fruit, its length increases, and a new fruit spawns at another random location.  

### 💥 How Collision Detection Works
- **Wall Collision** → If the snake moves beyond the grid boundaries, the game ends.  
- **Self Collision** → If the snake's head touches any part of its body, the game ends.  
- **Obstacle Collision** → As the level increases, random obstacles appear, and if the snake hits an obstacle, the game ends.  

### 🎯 How the Game Ends
- If the snake collides with itself, a wall, or an obstacle, the game displays a **Game Over** message.  
- The final score is shown, and the player can restart.  

---

## 🚀 How to Run

## 👥 Team - Bug Busters 🏆
This project was developed by Bug Busters, a passionate and dedicated team from Dhirubhai Ambani University (DAU). From conceptualization to execution, we poured our efforts into designing an immersive and challenging Snake Game that goes beyond the classic version by introducing obstacles, levels, and a performance rating system.

💡 Our journey was filled with brainstorming sessions, debugging marathons, and countless refinements to ensure smooth gameplay. Implementing the grid system, optimizing snake movement, and developing advanced mechanics like difficulty progression and obstacle placement required both creativity and technical expertise.

Throughout this project, we encountered numerous challenges—from handling complex logic in C++ to fine-tuning collision detection and randomization. However, every hurdle strengthened our understanding of game development, data structures, and algorithmic problem-solving.

This game is more than just a project; it’s a testament to our teamwork, perseverance, and passion for coding. We hope you enjoy playing it as much as we enjoyed building it! 🚀🐍🎮

## 👨‍💻 Team Members:
-📌 Nihar Patel - 202401148
-📌 Raj Patel - 202401153
-📌 Praneel Sharma - 202401166
-📌 Pranshu Patel - 202401167

### 🔮 Future Enhancements
-✅ Adding a graphical interface using SFML or SDL 🎨
-✅ Implementing multiplayer support 👥
-✅ Storing high scores using file handling 📁

### ⭐ Contributions & Feedback
If you have suggestions or want to contribute to the project, feel free to create a pull request or open an issue on GitHub! 🚀

📌 Star the repository if you like our work! ⭐


### 📌 Compile the code using a C++ compiler:
```sh
g++ snake_game.cpp -o snake_game



