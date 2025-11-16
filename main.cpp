// main.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

struct Entity {
    char glyph;    // how it appears on screen
    int x, y;      // position on grid
};

class Game {
public:
    Game(int w, int h)
        : width(w), height(h), running(true) {
        grid.resize(height, std::string(width, '.'));

        // create simple walls on border
        for (int x = 0; x < width; ++x) {
            grid[0][x] = '#';
            grid[height - 1][x] = '#';
        }
        for (int y = 0; y < height; ++y) {
            grid[y][0] = '#';
            grid[y][width - 1] = '#';
        }

        player = {'P', 2, 2};
        enemy  = {'E', width - 3, height - 3};
    }

    void run() {
        using namespace std::chrono;
        const int targetFPS = 10;
        const auto frameDuration = milliseconds(1000 / targetFPS);

        while (running) {
            auto frameStart = steady_clock::now();

            render();
            handleInput();
            update();

            auto frameEnd = steady_clock::now();
            auto workTime = duration_cast<milliseconds>(frameEnd - frameStart);
            if (workTime < frameDuration) {
                std::this_thread::sleep_for(frameDuration - workTime);
            }
        }
    }

private:
    int width, height;
    bool running;
    std::vector<std::string> grid;
    Entity player;
    Entity enemy;

    void clearScreen() {
        // Very simple and not ideal, but fine for a demo.
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void render() {
        clearScreen();

        // start from base grid
        std::vector<std::string> frame = grid;

        // draw entities
        frame[player.y][player.x] = player.glyph;
        frame[enemy.y][enemy.x]   = enemy.glyph;

        // print grid
        for (const auto& row : frame) {
            std::cout << row << "\n";
        }
        std::cout << "\nControls: W/A/S/D to move, Q to quit\n";
    }

    void handleInput() {
        char c;
        std::cout << "Input: ";
        std::cin >> c;
        c = std::tolower(c);

        int dx = 0, dy = 0;

        switch (c) {
            case 'w': dy = -1; break;
            case 's': dy =  1; break;
            case 'a': dx = -1; break;
            case 'd': dx =  1; break;
            case 'q': running = false; return;
            default:
                std::cout << "Unknown command.\n";
                return;
        }

        // tentative new position
        int newX = player.x + dx;
        int newY = player.y + dy;

        // collision with walls
        if (grid[newY][newX] == '#') {
            std::cout << "Bumped into a wall!\n";
            return;
        }

        player.x = newX;
        player.y = newY;
    }

    void update() {
        // simple enemy AI: move horizontally toward player
        if (enemy.x < player.x) enemy.x++;
        else if (enemy.x > player.x) enemy.x--;

        // collision detection: player vs enemy
        if (player.x == enemy.x && player.y == enemy.y) {
            clearScreen();
            std::cout << "The enemy caught you! Game over.\n";
            running = false;
        }
    }
};

int main() {
    Game game(20, 10);
    game.run();
    return 0;
}
