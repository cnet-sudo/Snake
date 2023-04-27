
#include <chrono>
#include <random>
#include <vector>

#include "curses.h"

using namespace std;
using namespace std::chrono;

struct vector2di {

    int x = 0;

    int y = 0;
};

void move_snake(vector2di & snake_head, const vector2di & vector_step,vector<vector2di> & snake_tail, 
    vector2di & apple, vector2di rnd) {

    // устанавливаем цвет змеи
    attrset(A_BOLD | COLOR_PAIR(3));

    // рисуем хвост змеи если он есть
    if (!snake_tail.empty()) {

        for (auto const & mov : snake_tail) {

            move(mov.y, mov.x); 

            addch('#');
        }
     }
    // изменяем координаты головы змеи
    snake_head.x += vector_step.x;

    snake_head.y += vector_step.y;

    move(snake_head.y, snake_head.x);

    // проверяем символ в установленных координатах курсора
    auto s = static_cast<char>(winch(stdscr));

    if (s == '!' || s=='#') exit(0);

    // змея съедает яблоко
    if (s == '@') {

        // добавляем хвост змеи
        snake_tail.push_back( {snake_head.x, snake_head.y});
        // рисуем голову змеи
        addch('$'); 

        do {

        // новые координаты яблока
        apple.x = rnd.x; apple.y = rnd.y;

        move(apple.y, apple.x);

        s = static_cast<char>(winch(stdscr));
        // повторяем пока координаты яблока совпадают с хвостом змеи
        } while (s == '#'|| s == '$');
    }
    else {

        // рисуем голову змеи
        addch('$');
        // обновляем координаты хвоста змеи
        if (!snake_tail.empty()) {

            snake_tail.push_back( { snake_head.x, snake_head.y });

            snake_tail.erase(snake_tail.begin());       
        }
    }
    refresh(); 
};

void show_map(vector2di & apple, vector2di rnd) {

    // очистка экрана
    clear();
    // перемещение курсора
    move(1,50);

    attrset(A_DIM | COLOR_PAIR(1));

    printw("Змейка\n"); 

    // рисуем игровое поле 
    for (int i = 0; i < 25; i++) {   
        
        move(i+3, 5);

        for (int j = 0; j < 100; j++) {

            if (i == 0 || i == 24) addch('!');
            else 
                if (j==0 || j==99) addch('!');
                else addch(' ');
        }

        addch('\n');
    }
    if (apple.x == 0) { 
        
        apple.x = rnd.x; apple.y = rnd.y;
    }
    // рисуем яблоко
    move(apple.y, apple.x);

    attrset(A_BOLD | COLOR_PAIR(2));

    addch('@');

    refresh();    
}



int main(){

    // переключаем шрифт для отображения кирилицы
    system("chcp 1251");
    // инициализируем экран curses
    initscr();                   
    // прячим курсор
    curs_set(0);
    // запускаем цветной режим
    start_color();
    // режимы цветов
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_MAGENTA, COLOR_BLUE);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);
    //цвет фона
    bkgd(COLOR_PAIR(1));
    // начальные координаты головы
    vector2di snake_head  {10,10};
    // направление и шаг перемещения
    vector2di vector_step  {1,0};
    // координаты яблока
    vector2di apple;

    vector2di rnd_apple;
    // координаты хвоста змеи 
    vector<vector2di> snake_tail;
    // момент системного времени
    long long seed = system_clock::now().time_since_epoch().count();
    // запуск генератора случайных чисел
    default_random_engine rnd(static_cast<unsigned int>(seed));
    // установка диапазона случайных координат яблока
    uniform_int_distribution<int> apple_x(10, 97);
    uniform_int_distribution<int> apple_y(5, 22);
    int pause = 100;

    // игровой цикл
    while (true) {

    rnd_apple.x = apple_x(rnd);
    rnd_apple.y = apple_y(rnd);
    // рисуем карту
    show_map(apple, rnd_apple);
    // перемещаем змейку
    move_snake(snake_head, vector_step, snake_tail, apple,rnd_apple);
    // пауза
    timeout(pause);
    // разрешаем использовать специальные клавиши в нашем случае стрелки    
    keypad(stdscr, true);
    // проверка нажатой клавиши
    switch (getch()) {

    case KEY_UP:
        if (vector_step.y == 0) { 
        
        vector_step.y = -1; 
        vector_step.x = 0; 
        pause = 170; 
        }
        break;

    case KEY_DOWN:
        if (vector_step.y == 0) { 
            
            vector_step.y = 1; 
            vector_step.x = 0; 
            pause = 170;} 
        break;

    case KEY_LEFT:  
        if (vector_step.x == 0) { 
            
            vector_step.x = -1; 
            vector_step.y = 0; 
            pause = 100;} 
        break;

    case KEY_RIGHT:   
        if (vector_step.x == 0) { 
            
            vector_step.x = 1; 
            vector_step.y = 0; 
            pause = 100;} 
        break;

    case 'q': 
        // выключаем экран curses
        endwin(); 
        return 0; 

    default:
        break;
    }

    }
}

