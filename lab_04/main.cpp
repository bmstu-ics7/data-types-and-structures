#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>

#define SUCCESS 0
#define INPUT_ERROR -1
#define NOT_WAY -2

using namespace std;

struct coord
{
    int x;
    int y;
};

template <class T>
struct my_list
{
    T data;
    my_list<T> *next;
    my_list<T> *prev;
};

template <class T>
struct stack_list
{
    my_list<T> *element;
    my_list<T> *first_element;

    stack_list()
    {
        element = NULL;
    }

    bool empty()
    {
        if (first_element == NULL)
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
            element->next = NULL;

            first_element = element;
            return;
        }

        element->next = new my_list<T>;
        element->next->data = el;
        element->next->prev = element;
        element->next->next = NULL;

        element = element->next;
    }

    void pop()
    {
        if (element != NULL)
        {
            my_list<T> *el = element;
            element = element->prev;

            delete el;

            if (element == NULL)
                first_element = NULL;
            else
                element->next = NULL;
        }
    }

    T take()
    {
        return element->data;
    }

    stack_list(const stack_list &copied)
    {
        my_list<T> *temp = copied.first_element;

        for (; temp != NULL; temp = temp->next)
            push(temp->data);
    }

    ~stack_list()
    {
        my_list<T> *temp = first_element;

        while (temp != NULL)
        {
            my_list<T> *el = temp;
            temp = temp->next;
            delete el;
        }
    }
};

template <class T>
struct stack_vector
{
    int element;
    vector<T> vec;

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
        vec.push_back(el);
    }

    void pop()
    {
        if (element > -1)
        {
            element--;
            vec.erase(vec.end() - 1);
        }
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

ostream& operator << (ostream &output, const stack_vector<coord> stack)
{
    for (int i = 0; i < stack.element + 1; i++)
        output << stack.vec[i].x << ' ' << stack.vec[i].y << endl;

    return output;
}

ostream& operator << (ostream &output, const stack_list<coord> stack)
{
    my_list<coord> *temp = stack.first_element;

    for (; temp != NULL; temp = temp->next)
        output << temp->data.x << ' ' << temp->data.y << endl;

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

        int take = rand() % neighbors.size();

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
    long long int result_vector = 0;
    for (int i = 0; i < 10; i++)
    {
        vector< vector<int> > copy = bench;

        clock_t start_time = clock();
        stack_vector<coord> way = find_with_stack< stack_vector<coord> >(copy, start, finish);
        clock_t finish_time = clock();

        result_vector += finish_time - start_time;
    }
    result_vector /= 10;

    long long int result_list = 0;
    for (int i = 0; i < 10; i++)
    {
        vector< vector<int> > copy = bench;

        clock_t start_time = clock();
        stack_vector<coord> way = find_with_stack< stack_vector<coord> >(copy, start, finish);
        clock_t finish_time = clock();

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

    clock_t start_vector = clock();
    stack_vector<coord> way_vector = find_with_stack< stack_vector<coord> >(lab_vector, start, finish);
    if (way_vector.empty()) cerr << "Нет пути!" << endl;
    draw_way< stack_vector<coord> >(lab_vector, way_vector);
    clock_t finish_vector = clock();

    //cout << way_vector;
    cout << lab_vector << endl;

    cout << "Реализация списком: " << endl;

    clock_t start_list = clock();
    stack_list<coord> way_list = find_with_stack< stack_list<coord> >(lab_list, start, finish);
    if (way_list.empty()) cerr << "Нет пути!" << endl;
    draw_way< stack_list<coord> >(lab_list, way_list);
    clock_t finish_list = clock();

    //cout << way_list;
    cout << lab_list << endl;

    cout << "Время реализации массивом: " << finish_vector - start_vector << " тиков" << endl;
    cout << "Время реализации списком: " << finish_list - start_list << " тиков" << endl << endl;

    benchmark(bench, start, finish, 10);

    return SUCCESS;
}

