/*
 * Задана система двусторонних дорог.
 * Найти множество городов, расстояние от которых до выделенного города ( столицы ) больше, чем Т.
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <time.h>

#define SUCCESS 0
#define INCORRECT_FILE -1

#define INF 1000000

using namespace std;

struct MatrixGraph
{
    int size;                                       // Количество вершин
    vector< vector<int> > ways;                     // Матрица

    MatrixGraph(ifstream &in);                      // Конструктор считывания из файла
    void print(string file);                        // Вывод на экран
    vector<int> find_ways_to_capital(int capital);  // Поиск кротчайших путей
};

struct List
{
    int name;                                       // Номер вершины
    int way;                                        // Длина грани
    List *next;                                     // Следующий элемент

    List();                                         // Конструктор
    void push(List *list, int name, int way);       // Добавление жлемента в конец
    void free(List *list);                          // Очистка памяти
};

struct ListGraph
{
    int size;                                       // Количество вершин
    vector<List*> ways;                             // Списки

    ListGraph(ifstream &in);                        // Конструктор считывания из файла
    void print();                                   // Вывод
    vector<int> find_ways_to_capital(int capital);  // Поиск кротчайших путей
    void free();                                    // Очистка памяти
};

MatrixGraph::MatrixGraph(ifstream &in)
{
    vector<int> v;

    while (true)
    {
        int num;
        in >> num;

        if (in.eof())
            break;

        v.push_back(num);
    }

    size = sqrt(v.size());
    for (int i = 0; i < size; i++)
    {
        vector<int> line;

        for (int j = 0; j < size; j++)
        {
            line.push_back(v[i * size + j]);
        }

        ways.push_back(line);
    }
}

void MatrixGraph::print(string name)
{
    cout << setw(4) << left << "";
    for (int i = 0; i < ways.size(); i++)
    {
        cout << setw(4) << left << i + 1;
    }
    cout << endl;

    for (int i = 0; i < ways.size(); i++)
    {
        cout << setw(4) << left << i + 1;
        for (int j = 0; j < ways[i].size(); j++)
            cout << setw(4) << left << ways[i][j];
        cout << endl;
    }

    ofstream out;
    out.open(name + ".gv", ios::trunc);

    out << "graph {" << endl;
    for (int i = 0; i < ways.size(); i++)
        for (int j = i; j < ways[0].size(); j++)
            if (ways[i][j])
                out << i + 1 << " -- " << j + 1 << "[label = \"" << ways[i][j] << "\"]" << endl;

    out << "}" << endl;

    out.close();
    string comand = "dot -Tpng " + name + ".gv -o " + name+ ".png";
    system(comand.c_str());
    system("rm *.gv");
}

vector<int> MatrixGraph::find_ways_to_capital(int capital)
{
    vector<int> dist(size, INF);
    vector<bool> used(size);

    dist[capital] = 0;

    int min_dist = 0;
    int min_vertex = capital;

    while (min_dist < INF)
    {
        int i = min_vertex;
        used[i] = true;

        for (int j = 0; j < size; j++)
            if (ways[i][j])
                if (dist[i] + ways[i][j] < dist[j])
                        dist[j] = dist[i] + ways[i][j];

        min_dist = INF;

        for (int j = 0; j < size; j++)
            if (!used[j] && dist[j] < min_dist)
            {
                min_dist = dist[j];
                min_vertex = j;
            }
    }

    return dist;
}

List::List()
{
    name = 0;
    way = 0;
    next = NULL;
}

void List::push(List *list, int name, int way)
{
    if (list->way == 0)
    {
        list->name = name;
        list->way = way;
        return;
    }

    List *temp = list;

    for (; temp->next != NULL; temp = temp->next);

    temp->next = new List;
    temp->next->name = name;
    temp->next->way = way;
}

void List::free(List *list)
{
    List *temp = list;

    while (temp)
    {
        List *el = temp;
        temp = temp->next;
        delete el;
    }
}

ListGraph::ListGraph(ifstream &in)
{
    MatrixGraph matrix(in);

    size = matrix.ways.size();

    for (int i = 0; i < size; i++)
        ways.push_back(new List());

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (matrix.ways[i][j])
                ways[i]->push(ways[i], j + 1, matrix.ways[i][j]);
}

void ListGraph::print()
{
    for (int i = 0; i < size; i++)
    {
        cout << i + 1;

        List *temp = ways[i];
        if (temp)
            for (; temp != NULL; temp = temp->next)
            {
                cout << " -> " << temp->name;
            }
        else
            cout << " -> NULL";

        cout << endl;
    }
}

vector<int> ListGraph::find_ways_to_capital(int capital)
{
    vector<int> dist(size, INF);
    vector<bool> used(size);

    dist[capital] = 0;

    int min_dist = 0;
    int min_vertex = capital;

    while (min_dist < INF)
    {
        int i = min_vertex;
        used[i] = true;

        List *temp = ways[i];
        for (; temp != NULL; temp = temp->next)
            if (dist[i] + temp->way < dist[temp->name - 1])
                    dist[temp->name - 1] = dist[i] + temp->way;

        min_dist = INF;

        for (int j = 0; j < size; j++)
            if (!used[j] && dist[j] < min_dist)
            {
                min_dist = dist[j];
                min_vertex = j;
            }
    }

    return dist;
}

void ListGraph::free()
{
    for (int i = 0; i < size; i++)
    {
        ways[i]->free(ways[i]);
    }
}

int input_file(ifstream &file)
{
    cout << "Выберите один из файлов:" << endl;
    system("ls *.in");
    cout << ">";

    string name;
    cin >> name;

    file.open(name);

    if (file.is_open() == false)
    {
        cerr << "Некорректный файл!" << endl;
        return INCORRECT_FILE;
    }

    vector<int> v;
    
    while (true)
    {
        int num;
        file >> num;

        if (file.eof())
            break;

        v.push_back(num);
    }

    for (int i = 0; i < sqrt(v.size()); i++)
        for (int j = 0; j < sqrt(v.size()); j++)
            if (v[i * sqrt(v.size()) + j] != v[j * sqrt(v.size()) + i])
            {
                cerr << "Таблица не симметрична!" << endl;
                return INCORRECT_FILE;
            }

    cout << endl;
    return SUCCESS;
}

int input_capital(int max)
{
    int num;
    cout << ">";

    while (scanf("%d", &num) != 1 || num <= 0 || num > max)
    {
        string buf;
        getline(cin, buf);

        cout << ">";
    }

    cout << endl;
    return num;
}

int input_t()
{
    int num;
    cout << ">";

    while (scanf("%d", &num) != 1 || num <= 0)
    {
        string buf;
        getline(cin, buf);

        cout << ">";
    }

    cout << endl;
    return num;
}

int main(void)
{
    ifstream file;
    if (input_file(file) != SUCCESS)
        return INCORRECT_FILE;

    file.clear();
    file.seekg(0);

    MatrixGraph matrix(file);
    matrix.print("matrix");

    cout << endl;

    file.clear();
    file.seekg(0);

    ListGraph list(file);
    list.print();

    cout << endl;

    cout << "Выберите вершину, которая будет столицей" << endl;
    int capital = input_capital(list.ways.size()) - 1;

    cout << "Введите расстояние от столицы" << endl;
    int T = input_t();

    clock_t start = clock();
    vector<int> dist_mat = matrix.find_ways_to_capital(capital);
    clock_t finish = clock();

    cout << "Длина пути из столицы до каждого из городов (матрица)" << endl;
    for (int i = 0; i < dist_mat.size(); i++)
        cout << dist_mat[i] << ' ';
    cout << endl << endl;

    cout << "Время поиска путей в матрице: " << finish - start << endl << endl;

    start = clock();
    vector<int> dist_list = list.find_ways_to_capital(capital);
    finish = clock();

    cout << "Длина пути из столицы до каждого из городов (списки)" << endl;
    for (int i = 0; i < dist_list.size(); i++)
        cout << dist_list[i] << ' ';
    cout << endl << endl;

    cout << "Время поиска путей в списках: " << finish - start << endl << endl;

    cout << "Города, расстояние до столицы которых больше, чем T" << endl;
    for (int i = 0; i < dist_mat.size(); i++)
        if (dist_mat[i] > T)
            cout << i + 1 << ' ';
    cout << endl;

    list.free();
    return SUCCESS;
}

