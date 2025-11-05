#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <climits>
#include <vector>

#include "sleep_utils.h"

// Include header files
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// Types
using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;

// Game class
class GameOfLife {
private:
    int64 width, height;
    std::vector<std::vector<bool>> field;
    int64 generation;

    // Function to clear console
    void clear_console() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    
    }

public:
    GameOfLife(int64 w, int64 h) : 
    width(w), height(h), generation(0), field(height, std::vector<bool>(width, false)) {
        std::srand(std::time(0));
    }

    /**
     * Initializes the game field with random live cells
     * @param life_probability Probability of cell being alive (0.0 to 1.0)
     */
    void initialize_random_field (double life_probability) {
        for (int64 x = 0; x < width; ++x) {
            for (int64 y = 0; y < height; ++y) {
                if ((static_cast<double>(std::rand()) / RAND_MAX) <= life_probability) {
                    field[y][x] = true;
                }
            }
        }
    }

    // Print the field into consol
    void display() {
        clear_console();

        std::string buffer;
        buffer.reserve((width + 3) * (height + 3) + 50);

        buffer += "Generation: " + std::to_string(generation) + "\n+";
        
        for (int64 x = 0; x < width; ++x) {
            buffer += "-";
        }
        buffer += "+\n";

        for (int64 y = 0; y < height; ++y) {
            buffer += "|";
            for (int64 x = 0; x < width; ++x) {
                if (field[y][x]) {
                    buffer += "█";
                } else {
                    buffer += " ";
                }
            }
            buffer += "|\n";
        }

        buffer += "+";
        for (int64 x = 0; x < width; ++x) {
            buffer += "-";
        }
        buffer += "+\n";

        std::cout << buffer;
        std::cout.flush();
    }

    /**
     * Count in standard neighborhood
     * @param x and @param y is coordinates of cell
     */
    int16 standard_count_neighbours(int64 x, int64 y) {
        int16 count = 0;

        for (int16 dx = -1; dx < 2; ++dx) {
            for (int16 dy = -1; dy < 2; ++dy) {
                if (dy == 0 && dx == 0) continue;

                int64 new_x = x + dx, new_y = y + dy;

                if (new_x > -1 && new_x < width && new_y > -1 && new_y < height) {
                    if (field[new_y][new_x]) ++count;
                }
            }
        }

        return count;
    }

    // Functions to make new generation with different rules 

    // B3/S23
    void standard_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neighbours == 2 || neighbours == 3) new_field[y][x] = true;
                } else {
                    if (neighbours == 3) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }

    // B36/S23
    void HighLife_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neighbours == 2 || neighbours == 3) new_field[y][x] = true;
                } else {
                    if (neighbours ==  3 || neighbours == 6) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }

    // B3/S012345678
    void LifeWithoutDeath_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neighbours >= 0 && neighbours <= 8) new_field[y][x] = true;
                } else {
                    if (neighbours ==  3) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }

    // B34/S34
    void Life34_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neighbours == 3 || neighbours == 4) new_field[y][x] = true;
                } else {
                    if (neighbours ==  3 || neighbours == 4) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }

    // B2/S0
    void Freezing_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neighbours == 0) new_field[y][x] = true;
                } else {
                    if (neighbours ==  2) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }

    // B2/S2
    void Desert_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neighbours == 2) new_field[y][x] = true;
                } else {
                    if (neighbours ==  2) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }

    // B2/S
    void Seeds_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (!is_alive) {
                   if (neighbours ==  2) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }

    // B3/S12345
    void Maze_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neighbours >= 1 && neighbours <= 5) new_field[y][x] = true;
                } else {
                    if (neighbours ==  3) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }

    // B1/S1
    void OnePeriodLife_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neighbours == 1) new_field[y][x] = true;
                } else {
                    if (neighbours ==  1) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }

    // B1/S012345678
    void Fractals_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neighbours = standard_count_neighbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neighbours >= 0 && neighbours <= 8) new_field[y][x] = true;
                } else {
                    if (neighbours ==  1) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }
};

/**
 * Functions which start game with different rules
 * @param life_probability Probability of cell being alive (0.0 to 1.0)
 */

// B3/S23
void StandardGame(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.standard_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 

    return;
}

// B36/S23
void HighLife(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.HighLife_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

// B3/S012345678
void LifeWithoutDeath(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.LifeWithoutDeath_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

// B34/S34
void Life34(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Life34_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

// B2/S0
void Freezing(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Freezing_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

// B2/S2
void Desert(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Desert_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

// B2/S
void Seeds(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Seeds_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

// B3/S12345
void Maze(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Maze_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

// B1/S1
void OnePeriodLife(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.OnePeriodLife_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

// B1/S012345678
void Fractals(int64 w, int64 h, int64 max_generation, double life_probability) {
    GameOfLife game(w, h);
    game.initialize_random_field(life_probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Fractals_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

// Function for manually start game
void manually_start() {
    std::cout << "You are in manual configuration mode\n";
    std::cout << "Select the size of the field:\n";
    std::cout << "1 -- random\n";
    std::cout << "2 -- manual input\n";
    
    short int choice;
    int64 w, h, max_generation;

    std::cin >> choice;
    switch (choice) {
        case 1:
            w = 20 + rand() % 20;
            h = 20 + rand() % 20;        
            break;
        case 2:
            std::cout << "First enter the width and then the height of the field: ";
            std::cin >> w >> h;
            break;
        default:
            std::cout << "You entered the wrong value.\n";
            return;
            break;
    }

    std::cout << "The sizes have been successfully initialized, enter the maximum generation.: ";
    std::cin >> max_generation;
    
    double life_probability;

    std::cout << "The generation has been initialized, enter the probability of the appearance of life in the initial position (from 0 to 1): ";
    std::cin >> life_probability;

    if (life_probability > 1 || life_probability < 0) {
        std::cout << "Your value does not lie in the range from 1 to 0.\n";
        return;
    }

    std::cout << "Now select the rule (Bab../Scd.. -- is born with the number of neighbors ab.., survives with cd..):\n";
    std::cout << "1 -- B3/S23 (StandardGame)\n";
    std::cout << "2 -- B36/S23 (HighLife)\n";
    std::cout << "3 -- B3/S012345678 (LifeWithoutDeath)\n";
    std::cout << "4 -- B34/S34 (Life34)\n";
    std::cout << "5 -- B2/S0 (Freezing)\n";
    std::cout << "6 -- B2/S2 (Desert)\n";
    std::cout << "7 -- B2/S (Seeds)\n";
    std::cout << "8 -- B3/S12345 (Maze)\n";
    std::cout << "9 -- B1/S1 (OnePeriodLife)\n";
    std::cout << "10 -- B1/S012345678 (Fractals):\n";
    std::cin >> choice;

    switch (choice) {
        case 1:
            std::cout << "I'm starting the game according to the standard rule\n";
            sleep(500);
            StandardGame(w, h, max_generation, life_probability);
            break;
        case 2:
            std::cout << "I'm starting the game according to the B36/S23 rule\n";
            sleep(500);
            HighLife(w, h, max_generation, life_probability);
            break;
        case 3:
            std::cout << "I'm starting the game according to the B3/S012345678 rule\n";
            sleep(500);
            LifeWithoutDeath(w, h, max_generation, life_probability);
            break;
        case 4:
            std::cout << "I'm starting the game according to the B34/S34 rule\n";
            sleep(500);
            Life34(w, h, max_generation, life_probability);
            break;
        case 5:
            std::cout << "I'm starting the game according to the B2/S0 rule\n";
            sleep(500);
            Freezing(w, h, max_generation, life_probability);
            break;
        case 6:
            std::cout << "I'm starting the game according to the B2/S2 rule\n";
            sleep(500);
            Desert(w, h, max_generation, life_probability);
            break;
        case 7:
            std::cout << "I'm starting the game according to the B2/S rule\n";
            sleep(500);
            Seeds(w, h, max_generation, life_probability);
            break;
        case 8:
            std::cout << "I'm starting the game according to the B3/S12345 rule\n";
            sleep(500);
            Maze(w, h, max_generation, life_probability);
            break;
        case 9:
            std::cout << "I'm starting the game according to the B1/S1 rule\n";
            sleep(500);
            OnePeriodLife(w, h, max_generation, life_probability);
            break;
        case 10:
            std::cout << "I'm starting the game according to the B1/S012345678 rule\n";
            sleep(500);
            Fractals(w, h, max_generation, life_probability);
            break;
        default:
            std::cout << "You entered the wrong value.\n";
            break;
   }   
}

// Function for fast random start
void fast_start() {
    std::cout << "You are in the quick start mode\n";
    GameOfLife game(40, 30);

    game.initialize_random_field(0.2);

    try {
        for (int16 i = 0; i < 100; ++i) {
            game.display();
            game.standard_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "You interrupted the demo, or something went wrong.\n";
    } 
}

int main() {
    short int choice = 0;
    std::cout << "Select the mode you want to start\n";
    std::cout << "Quick start with preset values -- 1\n";
    std::cout << "Selecting the manual mode setting -- 2\n";
    std::cin >> choice;

    if (choice == 1) {
        fast_start();
    } else if (choice == 2) {
        manually_start();
    } else {
        std::cout << "You entered the wrong number.\n";
    }

    
    return 0;
}
