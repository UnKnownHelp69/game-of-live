#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <climits>
#include <vector>

#include "sleep_utils.h"

// For windows
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// Ty[es
using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;

// Main class
class Game_of_life {
private:
    int64 width, height;
    std::vector<std::vector<bool>> field;
    int64 generation;

    // For clean console
    void clear_console() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    
    }

public:
    // Initializing class 
    Game_of_life(int64 w, int64 h) : 
    width(w), height(h), generation(0), field(height, std::vector<bool>(width, false)) {
        std::srand(std::time(0));
    }

    // Function to randome vaklues in field
    void randomize_field (double life_probability) {
        for (int64 x = 0; x < width; ++x) {
            for (int64 y = 0; y < height; ++y) {
                if ((static_cast<double>(std::rand()) / RAND_MAX) <= life_probability) {
                    field[y][x] = true;
                }
            }
        }
    }

    // Function to put field in stdout 
    void display() {
        clear_console();

        std::string buffer;
        buffer.reserve((width + 3) * (height + 3) + 50);

        buffer += "Поколение: " + std::to_string(generation) + "\n+";
        
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


    // Function to std neighborhood
    int16 standart_count_neigbours(int64 x, int64 y) {
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

    // Functions with different rules

    // B3/S23
    void standart_next_generation() {
        std::vector<std::vector<bool>> new_field(height, std::vector<bool>(width, false));

        for (int64 y = 0; y < height; ++y) {
            for (int64 x = 0; x < width; ++x) {
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neigbours == 2 || neigbours == 3) new_field[y][x] = true;
                } else {
                    if (neigbours == 3) new_field[y][x] = true;
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
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neigbours == 2 || neigbours == 3) new_field[y][x] = true;
                } else {
                    if (neigbours ==  3 || neigbours == 6) new_field[y][x] = true;
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
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neigbours >= 0 && neigbours <= 8) new_field[y][x] = true;
                } else {
                    if (neigbours ==  3) new_field[y][x] = true;
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
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neigbours == 3 || neigbours == 4) new_field[y][x] = true;
                } else {
                    if (neigbours ==  3 || neigbours == 4) new_field[y][x] = true;
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
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neigbours == 0) new_field[y][x] = true;
                } else {
                    if (neigbours ==  2) new_field[y][x] = true;
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
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neigbours == 2) new_field[y][x] = true;
                } else {
                    if (neigbours ==  2) new_field[y][x] = true;
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
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (!is_alive) {
                   if (neigbours ==  2) new_field[y][x] = true;
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
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neigbours >= 1 && neigbours <= 5) new_field[y][x] = true;
                } else {
                    if (neigbours ==  3) new_field[y][x] = true;
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
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neigbours == 1) new_field[y][x] = true;
                } else {
                    if (neigbours ==  1) new_field[y][x] = true;
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
                int16 neigbours = standart_count_neigbours(x, y);
                bool is_alive = field[y][x];

                if (is_alive) {
                    if (neigbours >= 0 && neigbours <= 8) new_field[y][x] = true;
                } else {
                    if (neigbours ==  1) new_field[y][x] = true;
                }
            }
        }
        field = new_field;
        ++generation;
    }
};

void StandartGame(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.standart_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 

    return;
}

void HighLife(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.HighLife_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

void LifeWithoutDeath(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.LifeWithoutDeath_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

void Life34(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Life34_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

void Freezing(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Freezing_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

void Desert(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Desert_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

void Seeds(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Seeds_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

void Maze(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Maze_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

void OnePeriodLife(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.OnePeriodLife_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

void Fractals(int64 w, int64 h, int64 max_generation, double probability) {
    Game_of_life game(w, h);
    game.randomize_field(probability);

    try {
        for (int64 i = 0; i < max_generation; ++i) {
            game.display();
            game.Fractals_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

void manually_start() {
    std::cout << "Вы в режиме ручной настройки\n";
    std::cout << "Выберите размеры поля:\n";
    std::cout << "1 -- случайные\n";
    std::cout << "2 -- ручной ввод\n";
    
    short int choice;
    int64 w, h, max_generation;

    std::cin >> choice;
    switch (choice) {
        case 1:
            w = 20 + rand() % 20;
            h = 20 + rand() % 20;        
            break;
        case 2:
            std::cout << "Введите вначале ширину, а затем высоту поля : ";
            std::cin >> w >> h;
            break;
        default:
            std::cout << "Вы ввели не то значение\n";
            return;
            break;
    }

    std::cout << "Размеры были успешно инциализированы, введите максимальное поколение: ";
    std::cin >> max_generation;
    
    double probability;

    std::cout << "Поколение было инициализировано, введите вероятность появление жизни в начальном положении (от 0 до 1): ";
    std::cin >> probability;

    if (probability > 1 || probability < 0) {
        std::cout << "Ваше значение не лежит в промеджутке от 1 до 0.\n";
        return;
    }

    std::cout << "Теперь выберите правило (Bab../Scd.. -- рождается при количестве соседей ab.., выживает при cd..):\n";
    std::cout << "1 -- B3/S23 (StandartGame)\n";
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
            std::cout << "Запускаю игру по стандартным правилам\n";
            sleep(500);
            StandartGame(w, h, max_generation, probability);
            break;
        case 2:
            std::cout << "Запускаю игру по правилам B36/S23\n";
            sleep(500);
            HighLife(w, h, max_generation, probability);
            break;
        case 3:
            std::cout << "Запускаю игру по правилам B3/S012345678\n";
            sleep(500);
            LifeWithoutDeath(w, h, max_generation, probability);
            break;
        case 4:
            std::cout << "Запускаю игру по правилам B34/S34\n";
            sleep(500);
            Life34(w, h, max_generation, probability);
            break;
        case 5:
            std::cout << "Запускаю игру по правилам B2/S0\n";
            sleep(500);
            Freezing(w, h, max_generation, probability);
            break;
        case 6:
            std::cout << "Запускаю игру по правилам B2/S2\n";
            sleep(500);
            Desert(w, h, max_generation, probability);
            break;
        case 7:
            std::cout << "Запускаю игру по правилам B2/S\n";
            sleep(500);
            Seeds(w, h, max_generation, probability);
            break;
        case 8:
            std::cout << "Запускаю игру по правилам B3/S12345\n";
            sleep(500);
            Maze(w, h, max_generation, probability);
            break;
        case 9:
            std::cout << "Запускаю игру по правилам B1/S1\n";
            sleep(500);
            OnePeriodLife(w, h, max_generation, probability);
            break;
        case 10:
            std::cout << "Запускаю игру по правилам B1/S012345678\n";
            sleep(500);
            Fractals(w, h, max_generation, probability);
            break;
        default:
            std::cout << "Вы ввели не то значение\n";
            break;
   }   
}


void fast_start() {
    std::cout << "Вы в режиме быстрого старта\n";
    Game_of_life game(40, 30);

    game.randomize_field(0.2);

    try {
        for (int16 i = 0; i < 100; ++i) {
            game.display();
            game.standart_next_generation();
            sleep(500);
        }
    } catch(...) {
        std::cout << "Вы прервали демонстрацию, или что-то пошло не так.\n";
    } 
}

int main() {
    short int choice = 0;
    std::cout << "Выберите режим, который хотите запустить\n";
    std::cout << "Быстрый старт с заданными значениями -- 1\n";
    std::cout << "Выбора ручной настройки режима -- 2\n";
    std::cin >> choice;

    if (choice == 1) {
        fast_start();
    } else if (choice == 2) {
        manually_start();
    } else {
        std::cout << "Вы ввели не то число\n";
    }

    
    return 0;
}


// Чтобы работал русский язык на винде, нужно в настройках включить эксперементальную поддержку unicode
