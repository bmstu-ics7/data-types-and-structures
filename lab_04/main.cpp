#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

#define SUCCESS 0
#define INPUT_ERROR -1
#define NOT_WAY -2

#define STACK_SIZE 100000

using namespace std;

unsigned long long tick(void)
{
    unsigned long long d;
    unsigned long long a;
    __asm__ __volatile__ ("rdtsc" : "=a" (a), "=d" (d));
    return ((d << 32) | a);
}

vector<void*> free_memory;
bool search_memory = false;

struct coord
{
    int x;
    int y;
};

template <class T>
struct my_list
{
    T data;
    my_list<T> *prev;
};

template <class T>
struct stack_list
{
    my_list<T> *element;

    stack_list()
    {
        element = NULL;
    }

    bool empty()
    {
        if (element == NULL)
            return true;
        else
            return false;
    }

    void push(T el)
    {
        if (element == NULL)
        {
            element = new my_list<T>;
            element->data = el;
            element->prev = NULL;

            return;
        }

        my_list<T> * temp = new my_list<T>;
        temp->data = el;
        temp->prev = element;
        element = temp;

        if (search_memory)
        {
            for (int i = 0; i < free_memory.size(); i++)
            {
                if (element == free_memory[i])
                {
                    free_memory.erase(free_memory.begin() + i);
                    break;
                }
            }
        }
    }

    void pop()
    {
        if (element != NULL)
        {
            if (search_memory)
                free_memory.push_back(element);

            my_list<T> *el = element;
            element = element->prev;

            delete el;
        }
    }

    T take()
    {
        return element->data;
    }

    void free_list()
    {
        my_list<T> *temp = element;

        while (temp != NULL)
        {
            my_list<T> *el = temp;
            temp = temp->prev;
            delete el;
        }

        element = NULL;
    }
};

template <class T>
struct stack_vector
{
    int element;
    T vec[STACK_SIZE];

    stack_vector()
    {
        element = -1;
    }

    bool empty()
    {
        if (element == -1)
            return true;
        else
            return false;
    }

    void push(T el)
    {
        element++;
        vec[element] = el;
    }

    void pop()
    {
        if (element > -1)
            element--;
    }

    T take()
    {
        return vec[element];
    }
};

int input_lab(vector< vector<int> > &labyrinth)
{
    int n, m;
    cout << "Введите размерность: ";

    if (scanf("%d %d", &n, &m) != 2)
    {
        cerr << "Некорректный ввод!" << endl;
        return INPUT_ERROR;
    }

    if (n <= 0 || m <= 0)
    {
        cerr << "Некорректный ввод!" << endl;
        return INPUT_ERROR;
    }

    cout << "Введите матрицу в обычном виде (1 - стенка, 0 - проход)" << endl;

    for (int i = 0; i < n; i++)
    {
        vector<int> line;

        for (int j = 0; j < m; j++)
        {
            int num = 0;

            if (scanf("%d", &num) != 1)
            {
                cerr << "Некорректный ввод!" << endl;
                return INPUT_ERROR;
            }

            if (num == 0)
                line.push_back(0);
            else
                line.push_back(-1);
        }

        labyrinth.push_back(line);
    }

    return SUCCESS;
}

ostream& operator << (ostream &output, const vector< vector<int> > lab)
{
    output << "┏";
    for (int i = 0; i < lab[0].size(); i++)
        cout << "━";
    output << "┓" << endl;

    for (int i = 0; i < lab.size(); i++)
    {
        output << "┃";
        for (int j = 0; j < lab[i].size(); j++)
        {
            if (lab[i][j] == -1)
                output << "█";
            else if (lab[i][j] == -2)
            {
                output << "_";
            }
            else if (lab[i][j] == -3)
            {
                output << "*";
            }
            else
            {
                output << " ";
            }
        }
        output << "┃";

        output << endl;
    }

    output << "┗";
    for (int i = 0; i < lab[0].size(); i++)
        cout << "━";
    output << "┛" << endl;

    return output;
}

vector<coord> take_neighbor(vector< vector<int> > lab, coord cell)
{
    bool up = true, left = true, down = true, right = true;
    vector<coord> neighbors;

    if (cell.x == 0)
        left = false;

    if (cell.y == 0)
        up = false;

    if (cell.x == lab.size() - 1)
        right = false;

    if (cell.y == lab[0].size() - 1)
        down = false;

    if (up && lab[cell.x][cell.y - 1] == 0)
    {
        coord c;
        c.x = cell.x;
        c.y = cell.y - 1;
        neighbors.push_back(c);
    }

    if (right && lab[cell.x + 1][cell.y] == 0)
    {
        coord c;
        c.x = cell.x + 1;
        c.y = cell.y;
        neighbors.push_back(c);
    }

    if (down && lab[cell.x][cell.y + 1] == 0)
    {
        coord c;
        c.x = cell.x;
        c.y = cell.y + 1;
        neighbors.push_back(c);
    }

    if (left && lab[cell.x - 1][cell.y] == 0)
    {
        coord c;
        c.x = cell.x - 1;
        c.y = cell.y;
        neighbors.push_back(c);
    }

    return neighbors;
}

template <class T>
T find_with_stack(vector< vector<int> > &lab, coord start, coord finish)
{
    T way;

    way.push(start);
    lab[way.take().x][way.take().y] = -2;

    while (true)
    {
        if (way.empty())
        {
            return way;
        }

        if (way.take().x == finish.x && way.take().y == finish.y)
            break;

        vector<coord> neighbors = take_neighbor(lab, way.take());

        if (neighbors.size() == 0)
        {
            way.pop();
            continue;
        }

        int take = 0;//rand() % neighbors.size();

        coord c;
        c.x = neighbors[take].x;
        c.y = neighbors[take].y;

        way.push(c);
        lab[way.take().x][way.take().y] = -2;
    }

    return way;
}

template <class T>
void draw_way(vector< vector<int> > &lab, T way)
{
    while (!way.empty())
    {
        lab[way.take().x][way.take().y] = -3;
        way.pop();
    }
}

int input_all(vector< vector<int> > &lab, coord &start, coord &finish)
{
    int start_x, start_y, finish_x, finish_y;

    if (input_lab(lab) != SUCCESS)
        return INPUT_ERROR;

    cout << "Введите стартовую позицию: ";
    if (scanf("%d %d", &start_x, &start_y) != 2)
    {
        cerr << "Некорректный ввод!" << endl;
        return INPUT_ERROR;
    }

    if (start_x < 0 || start_x >= lab.size() || start_y < 0 || start_y >= lab[0].size())
    {
        cerr << "Некорректный ввод!" << endl;
        return INPUT_ERROR;
    }

    cout << "Введите финишную позицию: ";
    if (scanf("%d %d", &finish_x, &finish_y) != 2)
    {
        cerr << "Некорректный ввод!" << endl;
        return INPUT_ERROR;
    }

    if (finish_x < 0 || finish_x >= lab.size() || finish_y < 0 || finish_y >= lab[0].size())
    {
        cerr << "Некорректный ввод!" << endl;
        return INPUT_ERROR;
    }

    if (lab[start_x][start_y] == -1)
    {
        cerr << "Некорректный ввод!" << endl;
        return INPUT_ERROR;
    }

    if (lab[finish_x][finish_y] == -1)
    {
        cerr << "Некорректный ввод!" << endl;
        return INPUT_ERROR;
    }

    start.x = start_x;
    start.y = start_y;
    finish.x = finish_x;
    finish.y = finish_y;

    return SUCCESS;
}

void benchmark(vector< vector<int> > bench, coord start, coord finish, int count)
{
    unsigned long long result_vector = 0;
    for (int i = 0; i < 10; i++)
    {
        vector< vector<int> > copy = bench;

        stack_vector<coord> way;
        unsigned long long start_time = tick();
        way = find_with_stack< stack_vector<coord> >(copy, start, finish);
        unsigned long long finish_time = tick();

        result_vector += finish_time - start_time;
    }
    result_vector /= 10;

    unsigned long long result_list = 0;
    for (int i = 0; i < 10; i++)
    {
        vector< vector<int> > copy = bench;

        stack_list<coord> way;
        unsigned long long start_time = tick();
        way = find_with_stack< stack_list<coord> >(copy, start, finish);
        unsigned long long finish_time = tick();

        way.free_list();

        result_list += finish_time - start_time;
    }
    result_list /= 10;

    cout << "Среднее время реализации массивом: " << result_vector << " тиков" << endl;
    cout << "Среднее время реализации списком: " << result_list << " тиков" << endl;
}

int main(void)
{
    srand(time(NULL));

    vector< vector<int> > lab_vector, lab_list, bench;
    coord start, finish;

    if (input_all(lab_vector, start, finish) != SUCCESS)
        return INPUT_ERROR;

    cout << endl;

    lab_list = lab_vector;
    bench = lab_vector;

    cout << "Реализация массивом: " << endl;

    stack_vector<coord> way_vector;
    unsigned long long start_vector = tick();
    way_vector = find_with_stack< stack_vector<coord> >(lab_vector, start, finish);
    unsigned long long finish_vector = tick();
    if (way_vector.empty()) cerr << "Нет пути!" << endl;
    draw_way< stack_vector<coord> >(lab_vector, way_vector);

    cout << lab_vector << endl;

    cout << "Реализация списком: " << endl;

    stack_list<coord> way_list;
    unsigned long long start_list = tick();
    way_list = find_with_stack< stack_list<coord> >(lab_list, start, finish);
    unsigned long long finish_list = tick();
    if (way_list.empty()) cerr << "Нет пути!" << endl;
    draw_way< stack_list<coord> >(lab_list, way_list);

    cout << lab_list << endl;

    cout << "Время реализации массивом: " << finish_vector - start_vector << " тиков" << endl;
    cout << "Время реализации списком: " << finish_list - start_list << " тиков" << endl << endl;

    benchmark(bench, start, finish, 10);

    cout << endl;
    search_memory = true;
    stack_list<coord> memory_test = find_with_stack< stack_list<coord> >(bench, start, finish);
    cout << "Фрагментация памяти (освобожденные, но не использованные адреса памяти): " << endl;
    for (int i = 0; i < free_memory.size(); i++)
        cout << free_memory[i] << endl;

    search_memory = false;
    memory_test.free_list();

    return SUCCESS;
}

