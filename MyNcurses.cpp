#include <curses.h>
#include <chrono>
#include <random>
#include <vector>

using namespace std;
using namespace std::chrono;
// координаты головы
int x=10;
int y=10;
// направление и шаг перемещения
int stepx = 1;
int stepy = 0;
// координаты яблока
int xq = 0;
int yq = 0;
// координаты хвоста змеи 
vector<int> vx;
vector<int> vy;

// момент системного времени
long seed = system_clock::now().time_since_epoch().count();
// запуск генератора случайных чисел
default_random_engine rnd(seed);
// установка диапазона случайных чисел
uniform_int_distribution<int> dx(10, 97);
uniform_int_distribution<int> dy(5, 22);

void move_snake() 
{
    // рисуем хвост змеи если он есть
    attrset(A_BOLD | COLOR_PAIR(3));
    if (!vx.empty()) 
    {
        for (int i = 0; i < vx.size(); i++)
        {
            move(vy[i],vx[i]);
            addch('#');
        }
     }
    // рисуем голову змеи
    x += stepx; 
    y += stepy;
    move(y,x);
    // проверяем символ в установленных координатах курсора
    auto s = static_cast<char>(winch(stdscr));
    if (s == '!' || s=='#') exit(0);
    if (s == '@') 
    {
        // добавляем хвост змеи
        vx.push_back(x); vy.push_back(y);
        addch('$'); 
        do {
        // новые координаты яблока
        xq = dx(rnd); yq = dy(rnd); 
        move(yq, xq);
        auto s = static_cast<char>(winch(stdscr));
        } while (s == '#');
    }
    else
    {
        addch('$');
        // обновляем координаты змеи
        if (!vx.empty()) 
        {
        vx.push_back(x); vy.push_back(y);
        vx.erase(vx.begin()); 
        vy.erase(vy.begin());
        }
    }
    refresh(); 
};

void show_map() 
{
    // очистка экрана
    clear();
    // перемещение курсора
    move(1,50);
    attrset(A_DIM | COLOR_PAIR(1));
    printw("Змейка\n"); 
    // рисуем игровое поле 
    for (int i = 0; i < 25; i++)
    {   move(i+3, 5);
        for (int j = 0; j < 100; j++)
        {
            if (i == 0 || i == 24) { addch('!'); }
            else if (j==0 || j==99) { addch('!'); } else addch(' ');
        }
        addch('\n');
    }
    if (xq == 0) { xq = dx(rnd); yq = dy(rnd); }
    // рисуем яблоко
    move(yq,xq);
    attrset(A_BOLD | COLOR_PAIR(2));
    addch('@');
    refresh();    
}



int main()
{
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
    // игровой цикл
    while (true) 
    {
    show_map();
    move_snake();
    // пауза
    timeout(150);   
    // разрешаем использовать специальные клавиши в нашем случае стрелки    
    keypad(stdscr, true);
    // проверка нажатой клавиши
    switch (getch()) 
    {
    case KEY_UP:  if (stepy == 0) { stepy = -1; stepx = 0; }break;
    case KEY_DOWN: if (stepy == 0) {stepy = 1; stepx = 0;} break; 
    case KEY_LEFT:  if (stepx == 0) {stepx = -1; stepy = 0;} break; 
    case KEY_RIGHT:   if (stepx == 0) { stepx = 1; stepy = 0; } break; 
    case 'q': exit(0); 
    default:break;
    }
    }
    // выключаем экран curses
    endwin();                    
    return 0;
}

