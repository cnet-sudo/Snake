#include <chrono>
#include <random>
#include <vector>
#include <string>
#include <curses.h>

using namespace std;
using namespace std::chrono;

bool game_exit = false;

struct vector2di {

    int x = 0;

    int y = 0;
};

void move_snake(vector2di& snake_head, vector2di& vector_step, vector<vector2di>& snake_tail,
    vector2di& apple, vector2di rnd, int& eaten_apples) {

    // устанавливаем цвет змеи
    attrset(A_BOLD | COLOR_PAIR(3));

    // рисуем хвост змеи если он есть
    if (!snake_tail.empty()) {

        for (auto const& mov : snake_tail) {

            move(mov.y, mov.x);

            printw("#");
        }
    }
    // изменяем координаты головы змеи
    snake_head.x += vector_step.x;

    snake_head.y += vector_step.y;

    move(snake_head.y, snake_head.x);

    // проверяем символ в установленных координатах курсора
    auto s = static_cast<char>(winch(stdscr));

    if (s == '*' || s == '#') {

        // устанавливаем цвет надписи
        attrset(A_BOLD | COLOR_PAIR(4));
        move(13, 55);
        printw("Конец игры");
        move(14, 42);
        printw("Выход - < q > Начать заново - < n >");
        do {

            if (getch() == 'q') {

                game_exit = true;
                return;
            }
            if (getch() == 'n')
            {
                // начальные координаты головы
                snake_head = { 10,10 };
                // направление и шаг перемещения
                vector_step = { 1,0 };
                snake_tail.clear();
                apple = { 15,15 };
                eaten_apples = 0;
                return;
            }
        } while (true);
    }

    // змея съедает яблоко
    if (s == '@') {
        eaten_apples++;
        // добавляем хвост змеи
        snake_tail.push_back({ snake_head.x, snake_head.y });
        // рисуем голову змеи
        printw("$");

        do {

            // новые координаты яблока
            apple.x = rnd.x; apple.y = rnd.y;

            move(apple.y, apple.x);

            auto s = static_cast<char>(winch(stdscr));
            // повторяем пока координаты яблока совпадают с хвостом змеи
        } while (s == '#');
    }
    else {

        // рисуем голову змеи
        printw("$");
        // обновляем координаты хвоста змеи
        if (!snake_tail.empty()) {

            snake_tail.erase(snake_tail.begin());

            snake_tail.push_back({ snake_head.x, snake_head.y });


        }
    }
};

void show_map(const vector2di& apple, const int& eaten_apples) {

    // очистка экрана
    clear();
    // перемещение курсора
    move(2, 55);

    attrset(A_DIM | COLOR_PAIR(1));

    printw("Змейка\t\t");
    attrset(A_BOLD | COLOR_PAIR(5));
    printw(" количество собранных яблок < ");
    string s_eaten_apples = to_string(eaten_apples);
    printw(s_eaten_apples.c_str());
    printw(" > ");
    attrset(A_DIM | COLOR_PAIR(1));
    // рисуем игровое поле 
    for (int y = 4; y < 28; y++) {

        for (int x = 5; x < 112; x++) {

            if (y == 4 || y == 27 || x == 5 || x == 111) {

                move(y, x); 
                printw("*");
            }

        }

    }
    // рисуем яблоко
    move(apple.y, apple.x);
    attrset(A_BOLD | COLOR_PAIR(2));
    printw("@");

}

int main() {

    // переключаем шрифт для отображения кириллицы
    system("chcp 1251");
    // инициализируем экран curses
    initscr();
    // прячем курсор
    curs_set(0);
    // запускаем цветной режим
    start_color();
    // режимы цветов
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_MAGENTA, COLOR_BLUE);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);
    init_pair(4, COLOR_RED, COLOR_BLUE);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    //цвет фона
    bkgd(COLOR_PAIR(1));

    // начальные координаты головы
    vector2di snake_head{ 10,10 };
    // направление и шаг перемещения
    vector2di vector_step{ 1,0 };
    // координаты яблока
    vector2di apple{ 15,15 };
    vector2di rnd_apple;
    // координаты хвоста змеи 
    vector<vector2di> snake_tail;
    // частота смены кадров
    int frame_rate = 100;
    // количество собранных яблок
    int eaten_apples = 0;

    // момент системного времени
    long long seed = system_clock::now().time_since_epoch().count();
    // запуск генератора случайных чисел
    default_random_engine rnd(static_cast<unsigned int>(seed));
    // установка диапазона случайных координат яблока
    uniform_int_distribution<int> apple_x(10, 97);
    uniform_int_distribution<int> apple_y(5, 22);
    
    // разрешаем использовать специальные клавиши в нашем случае стрелки    
    keypad(stdscr, true);
    // выключает отображение нажатых клавиш на экране
    noecho();
    // игровой цикл
    while (!game_exit) {

        rnd_apple.x = apple_x(rnd);
        rnd_apple.y = apple_y(rnd);
        // рисуем карту
        show_map(apple, eaten_apples);
        // перемещаем змейку
        move_snake(snake_head, vector_step, snake_tail, apple, rnd_apple, eaten_apples);
        // пауза
        timeout(frame_rate);

        // проверка нажатой клавиши
        switch (getch()) {
        case KEY_UP:
            if (vector_step.y == 0) {

                vector_step.y = -1;
                vector_step.x = 0;
                frame_rate = 170;
            }
            break;
        case KEY_DOWN:
            if (vector_step.y == 0) {

                vector_step.y = 1;
                vector_step.x = 0;
                frame_rate = 170;
            }
            break;
        case KEY_LEFT:
            if (vector_step.x == 0) {

                vector_step.x = -1;
                vector_step.y = 0;
                frame_rate = 100;
            }
            break;
        case KEY_RIGHT:
            if (vector_step.x == 0) {

                vector_step.x = 1;
                vector_step.y = 0;
                frame_rate = 100;
            }
            break;
        case 'q':
            game_exit = true;
            break;
        default:
            break;
        }

    }
    endwin();
    return 0;
}
