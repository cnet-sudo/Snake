
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

void show_map(const vector2di& apple, const int& eaten_apples) {

    // очистка экрана
    clear();
    // перемещение курсора
    move(2, 55);
    // устанавливаем атрибуты для текста яркость и номер цветовой пары
    attrset(A_DIM | COLOR_PAIR(1));
    // выводим текст в консоль
    printw("Змейка\t\t");
    attrset(A_BOLD | COLOR_PAIR(5));
    printw(" количество собранных яблок < ");
    // значение целочисленной переменной переводим в строковое
    string s_eaten_apples = to_string(eaten_apples);
    // выводим значение переменной
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

void move_snake(vector2di& snake_head, vector2di& vector_step, vector<vector2di>& snake_tail,
    vector2di& apple, vector2di rnd, int& eaten_apples) {

    // устанавливаем цвет змейки
    attrset(A_BOLD | COLOR_PAIR(3));

    // Если  змейка с хвостом, рисуем хвост
    if (!snake_tail.empty()) {

        for (auto const& mov : snake_tail) {

            move(mov.y, mov.x);

            printw("#");
        }
    }
    // изменяем координаты головы змейки
    snake_head.x += vector_step.x;
    snake_head.y += vector_step.y;
    // перемещаем курсов в координаты головы змейки
    move(snake_head.y, snake_head.x);
    // проверяем символ в установленных координатах курсора
    auto s = static_cast<char>(winch(stdscr));
    // *** Если змейка столкнулась с хвостом или границами игрового поля
    //  ### Выводим игровое меню ###
    if (s == '*' || s == '#') {

        attrset(A_BOLD | COLOR_PAIR(4));
        move(13, 55);
        printw("Конец игры");
        move(14, 42);
        printw("Выход - < q >  Начать заново - < n >");
        // *** Цикл выбора игрового меню ***
        do {
            if (getch() == 'q') {
                // выход из игры
                game_exit = true;
                return;
            }
            if (getch() == 'n')
            {
                // рестарт задаём начальные координаты переменным
                snake_head = { 10,10 };
                vector_step = { 1,0 };
                snake_tail.clear();
                apple = { 15,15 };
                eaten_apples = 0;
                return;
            }
        } while (true);
        // *** Конец цикла выбора игрового меню ***
    }
    // ### Конец игрового меню ###

        //  Если змейка съедает яблоко
    if (s == '@') {
        // увеличиваем количество съеденных яблок
        eaten_apples++;
        // добавляем хвост змейки
        snake_tail.push_back({ snake_head.x, snake_head.y });
        // рисуем голову змейки
        printw("$");

        do {
            // задаём новые координаты яблока
            apple.x = rnd.x; apple.y = rnd.y;
            move(apple.y, apple.x);
            auto s = static_cast<char>(winch(stdscr));
            // повторяем цикл пока координаты яблока совпадают с хвостом змейки
        } while (s == '#');
    }
    // Если змейка переместилась в свободное поле
    else {
        // рисуем голову змейки
        printw("$");
        // обновляем координаты хвоста змеи
        if (!snake_tail.empty()) {
            snake_tail.erase(snake_tail.begin());
            snake_tail.push_back({ snake_head.x, snake_head.y });
        }
    }
};


int main()
{
    // переключаем шрифт для отображения кириллицы
    system("chcp 1251");
    // инициализируем экран curses
    initscr();
    // прячем курсор
    curs_set(0);
    // запускаем цветной режим
    start_color();
    // режимы цветов 1,2,3,4,5
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_MAGENTA, COLOR_BLUE);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);
    init_pair(4, COLOR_RED, COLOR_BLUE);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    //цвет фона, режим 1
    bkgd(COLOR_PAIR(1));
    // начальные координаты головы змейки
    vector2di snake_head{ 10,10 };
    // направление и шаг перемещения змейки
    vector2di vector_step{ 1,0 };
    // постоянные координаты яблока 
    vector2di apple{ 15,15 };
    // случайные координаты яблока
    vector2di rnd_apple;
    // динамический массив координат хвоста змеи 
    vector<vector2di> snake_tail;
    // частота смены кадров
    int frame_rate = 100;
    // количество собранных яблок
    int eaten_apples = 0;

    // **** начало работы генератора случайных чисел ****
    // момент системного времени
    long long seed = system_clock::now().time_since_epoch().count();
    // запуск генератора случайных чисел
    default_random_engine rnd(static_cast<unsigned int>(seed));
    // установка диапазона случайных координат яблока
    uniform_int_distribution<int> apple_x(10, 97);
    uniform_int_distribution<int> apple_y(5, 22);
    // **** конец работы генератора случайных чисел ****

    // разрешаем использовать специальные клавиши в нашем случае стрелки для управления змейкой   
    keypad(stdscr, true);
    // выключает отображение нажатых клавиш на экране
    noecho();

    // **** игровой цикл ****
    while (!game_exit) {
        // присваиваем случайные значения переменным случайных координат яблока
        rnd_apple.x = apple_x(rnd);
        rnd_apple.y = apple_y(rnd);
        // рисуем игровое поле
        show_map(apple, eaten_apples);
        // перемещаем змейку по игровому полю
        move_snake(snake_head, vector_step, snake_tail, apple, rnd_apple, eaten_apples);
        // частота сменны игрового кадра
        timeout(frame_rate);

        // **** управление змейкой и игрой ****
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
        // ****  конец управления змейкой и игрой **** 
    }
    // **** конец игрового цикла ****
    endwin();
    return 0;
}


