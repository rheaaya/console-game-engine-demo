#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

const char* COLOR_RESET = "\x1b[0m";
const char* COLOR_PLAYER = "\x1b[32m";
const char* COLOR_ENEMY = "\x1b[31m";
const char* COLOR_WALL = "\x1b[37m";
const char* COLOR_FLOOR = "\x1b[90m";
const char* COLOR_TEXT = "\x1b[36m";
const char* COLOR_WARNING = "\x1b[33m";

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

struct Entity {
    char glyph;
    int x;
    int y;
};

class Game {
public:
    Game(int w, int h)
        : width(w), height(h), running(true),
          grid(height, std::string(width, '.')) {
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
            auto elapsed = duration_cast<milliseconds>(frameEnd - frameStart);
            if (elapsed < frameDuration) {
                std::this_thread::sleep_for(frameDuration - elapsed);
            }
        }
    }

private:
    int width;
    int height;
    bool running;
    std::vector<std::string> grid;
    Entity player;
    Entity enemy;

    void render() {
        clearScreen();
        std::vector<std::string> frame = grid;
        frame[player.y][player.x] = player.glyph;
        frame[enemy.y][enemy.x] = enemy.glyph;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                char ch = frame[y][x];
                if (ch == player.glyph) {
                    std::cout << COLOR_PLAYER << ch << COLOR_RESET;
                } else if (ch == enemy.glyph) {
                    std::cout << COLOR_ENEMY << ch << COLOR_RESET;
                } else if (ch == '#') {
                    std::cout << COLOR_WALL << ch << COLOR_RESET;
                } else {
                    std::cout << COLOR_FLOOR << ch << COLOR_RESET;
                }
            }
            std::cout << '\n';
        }

        std::cout << COLOR_TEXT
                  << "\nControls: W/A/S/D to move, Q to quit.\n"
                  << "Avoid the red E. If it reaches you, the game ends.\n"
                  << COLOR_RESET;
    }

    void handleInput() {
        char c;
        std::cout << COLOR_TEXT << "Input: " << COLOR_RESET;
        if (!(std::cin >> c)) {
            running = false;
            return;
        }

        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

        int dx = 0, dy = 0;
        switch (c) {
            case 'w': dy = -1; break;
            case 's': dy = 1;  break;
            case 'a': dx = -1; break;
            case 'd': dx = 1;  break;
            case 'q':
                running = false;
                return;
            default:
                std::cout << COLOR_WARNING << "Unknown command.\n" << COLOR_RESET;
                return;
        }

        int newX = player.x + dx;
        int newY = player.y + dy;

        if (grid[newY][newX] == '#') {
            std::cout << COLOR_WARNING << "You hit a wall!\n" << COLOR_RESET;
        } else {
            player.x = newX;
            player.y = newY;
        }
    }

    void update() {
        if (enemy.x < player.x) enemy.x++;
        else if (enemy.x > player.x) enemy.x--;

        if (enemy.y < player.y) enemy.y++;
        else if (enemy.y > player.y) enemy.y--;

        if (enemy.x == player.x && enemy.y == player.y) {
            clearScreen();
            std::cout << COLOR_ENEMY
                      << "The enemy caught you! Game over.\n"
                      << COLOR_RESET;
            running = false;
        }
    }
};

int main() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hOut != INVALID_HANDLE_VALUE &&
        GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | 0x0004);
    }
#endif

    clearScreen();
    std::cout << COLOR_TEXT
              << "WELCOME TO RIYA'S GAME\n\n"
              << "It's pretty simple:\n"
              << "  - You are the " << COLOR_PLAYER << "P" << COLOR_TEXT << " (green).\n"
              << "  - The enemy is the " << COLOR_ENEMY << "E" << COLOR_TEXT << " (red).\n"
              << "  - Move with W A S D.\n"
              << "  - Stay inside the walls (#) and avoid the enemy.\n"
              << "  - If the enemy reaches you, the game ends.\n\n"
              << "Press Enter to start playing..."
              << COLOR_RESET;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    Game game(20, 10);
    game.run();
    return 0;
}
