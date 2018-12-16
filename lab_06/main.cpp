#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <time.h>

#define SUCCESS 0
#define INCORRECT_FILE -1

using namespace std;

int count_of_comp = 0;

struct Tree
{
    int data;                                                   // Ключ узла
    Tree *left;                                                 // Левый элемент
    Tree *right;                                                // Правый элемент

    Tree();                                                     // Конструктор пустого узла
    Tree(int d);                                                // Конструктор узла с ключом
    void input(ifstream &in);                                   // Ввод дерева из файла
    void print_recursive(vector<string> probels, bool end);     // Вывод дерева в консоль
    void print_recursive_in_file(ofstream &file);               // Вывод дерева в .ps файл
    void print(string name);                                    // Вызов функций вывода
    Tree *remove(Tree *root, int data);                         // Удаление элемента
    void free();                                                // Очистка дерева
};

struct BalancedTree
{
    int data;                                                   // Ключ узла
    BalancedTree *left;                                         // Левый элемент
    BalancedTree *right;                                        // Правый элемент
    unsigned char height;                                       // Высота узла

    BalancedTree();                                             // Конструктор пустого узла
    BalancedTree(int k);                                        // Конструктор узла с ключом
    void print_recursive(vector<string> probels, bool end);     // Вывод дерева в консоль
    void print_recursive_in_file(ofstream &file);               // Вывод дерева в .ps файл
    void print(string name);                                    // Вызов функций вывода
    void free();                                                // Очистка узла
};

struct BalancedTreeHead
{
    BalancedTree *head;                                         // Вехний элемент сбалансированного дерева

    BalancedTreeHead();                                         // Конструктор пустого дерева
    void input(ifstream &in);                                   // Ввод дерева из файла
    void print(string name);                                    // Вывод дерева в консоль и в файл .ps
    unsigned char height(BalancedTree *root);                   // Получение высоты узла
    int bfactor(BalancedTree *root);                            // Вычисляет разность высот дочерних узлов
    void fixheight(BalancedTree *root);                         // Обновление высоты узла
    BalancedTree *rotateright(BalancedTree *p);                 // Правый поворот вокруг узла
    BalancedTree *rotateleft(BalancedTree *q);                  // Левый поворот вокруг узла
    BalancedTree *balance(BalancedTree *root);                  // Балансировка узла
    BalancedTree *insert(BalancedTree *root, int data);         // Вставка ключа в узел
    BalancedTree *findmin(BalancedTree *root);                  // Поиск узла с минимальным ключом в узле
    BalancedTree *removemin(BalancedTree *root);                // Удаление узла с минимальным ключом из дерева p
    BalancedTree *remove(BalancedTree *root, int data);         // Удаление элемента
    void free();                                                // Очистка дерева
};

struct ClosedHashElement
{
    int data;                                                   // Значение клча
    int attempt;                                                // Номер попытки записи
    bool free;                                                  // Свободна ли ячейка

    ClosedHashElement();                                        // Конструктор пустого элемента
};

struct ClosedHashTable
{
    vector<ClosedHashElement> table;                            // Таблица данных
    int digit;                                                  // Число для хеширования

    ClosedHashTable();                                          // Конструктор пустой таблицы
    int function(int data);                                     // Функция хеширования
    void input(ifstream &in);                                   // Ввод таблицы
    void print();                                               // Вывод таблицы
    int insert(int data);                                       // Вставка элемента
    void reorganisation();                                      // Нахождение новой функции хеширования
    void remove(int data);                                      // Удаление элемента
};

struct File
{
    fstream file;                                               // Сам файл
    string name;                                                // Название файла

    File();                                                     // Пустой конструктор
    void input(string name);                                    // Открытие файла
    void print();                                               // Вывод содержимого файла
    void remove(int data);                                      // Удаление элемента из файла
    void close();                                               // Закрытие файла
};

Tree::Tree()
{
    data = 0;
    left = NULL;
    right = NULL;
}

Tree::Tree(int d)
{
    data = d;
    left = NULL;
    right = NULL;
}

void Tree::input(ifstream &in)
{
    int num;
    in >> num;
    data = num;

    while (true)
    {
        in >> num;

        if (in.eof())
            break;

        Tree *now = this;
        while (true)
        {
            if (num < now->data)
            {
                if (now->left)
                {
                    now = now->left;
                }
                else
                {
                    now->left = new Tree;
                    now->left->left = NULL;
                    now->left->right = NULL;
                    now->left->data = num;
                    break;
                }
            }
            else
            {
                if (now->right)
                {
                    now = now->right;
                }
                else
                {
                    now->right = new Tree;
                    now->right->left = NULL;
                    now->right->right = NULL;
                    now->right->data = num;
                    break;
                }
            }
        }
    }
}

void Tree::print_recursive(vector<string> probels, bool end)
{
    for (int i = 0; i < probels.size(); i++)
    {
        cout << probels[i];
    }

    if (end)
    {
        probels.push_back("  ");
        cout << "└▶" << data << endl;
    }
    else
    {
        probels.push_back("│ ");
        cout << "├▶" << data << endl;
    }

    if (left && right)
    {
        left->print_recursive(probels, false);
        right->print_recursive(probels, true);
    }
    else if (left && !right)
        left->print_recursive(probels, true);
    else if (!left && right)
        right->print_recursive(probels, true);
}

void Tree::print_recursive_in_file(ofstream &file)
{
    if (left)
    {
        file << data << " -> " << left->data << ";" << endl;
        left->print_recursive_in_file(file);
    }

    if (right)
    {
        file << data << " -> " << right->data << ";" << endl;
        right->print_recursive_in_file(file);
    }
}

void Tree::print(string name)
{
    ofstream file;
    file.open(name + ".gv");

    file << "digraph Tree {" << endl;

    print_recursive_in_file(file);

    file << "}" << endl;

    file.close();
    string comand = "dot -Tpng " + name + ".gv -o " + name+ ".png";
    system(comand.c_str());
    system("rm *.gv");

    cout << data << endl;

    vector<string> probels;

    if (left && right)
    {
        left->print_recursive(probels, false);
        right->print_recursive(probels, true);
    }
    else if (left && !right)
        left->print_recursive(probels, true);
    else if (!left && right)
        right->print_recursive(probels, true);
}

Tree* Tree::remove(Tree *root, int k)
{
    if(!root)
        return NULL;

    if (k < root->data)
    {
        count_of_comp++;
        root->left = remove(root->left, k);
    }
    else if (k > root->data)
    {
        count_of_comp++;
        root->right = remove(root->right, k);
    }
    else
    {
        Tree* q = root->left;
        Tree* r = root->right;
        delete root;

        if (!r && !q)
            return NULL;

        if (!r)
            return q;

        if (!q)
            return r;

        if (r->left == NULL)
        {
            r->left = q;
            return r;
        }

        Tree *temp = r;
        Tree *prev = temp;

        while (temp->left)
        {
            prev = temp;
            temp = temp->left;
        }

        prev->left = NULL;
        temp->right = r;
        temp->left = q;

        return temp;
    }

    return root;
}

void Tree::free()
{
    if (left)
    {
        left->free();
        delete left;
    }

    if (right)
    {
        right->free();
        delete right;
    }
}

BalancedTree::BalancedTree()
{
    data = 0;
    height = 1;
    left = NULL;
    right = NULL;
}

BalancedTree::BalancedTree(int k)
{
    data = k;
    height = 1;
    left = NULL;
    right = NULL;
}

void BalancedTree::print_recursive(vector<string> probels, bool end)
{
    for (int i = 0; i < probels.size(); i++)
    {
        cout << probels[i];
    }

    if (end)
    {
        probels.push_back("  ");
        cout << "└▶" << data << endl;
    }
    else
    {
        probels.push_back("│ ");
        cout << "├▶" << data << endl;
    }

    if (left && right)
    {
        left->print_recursive(probels, false);
        right->print_recursive(probels, true);
    }
    else if (left && !right)
        left->print_recursive(probels, true);
    else if (!left && right)
        right->print_recursive(probels, true);
}

void BalancedTree::print_recursive_in_file(ofstream &file)
{
    if (left)
    {
        file << data << " -> " << left->data << ";" << endl;
        left->print_recursive_in_file(file);
    }

    if (right)
    {
        file << data << " -> " << right->data << ";" << endl;
        right->print_recursive_in_file(file);
    }
}

void BalancedTree::print(string name)
{
    ofstream file;
    file.open(name + ".gv");

    file << "digraph Tree {" << endl;

    print_recursive_in_file(file);

    file << "}" << endl;

    file.close();
    string comand = "dot -Tpng " + name + ".gv -o " + name+ ".png";
    system(comand.c_str());
    system("rm *.gv");

    cout << data << endl;

    vector<string> probels;
    if (left && right)
    {
        left->print_recursive(probels, false);
        right->print_recursive(probels, true);
    }
    else if (left && !right)
        left->print_recursive(probels, true);
    else if (!left && right)
        right->print_recursive(probels, true);
}

void BalancedTree::free()
{
    if (left)
    {
        left->free();
        delete left;
    }

    if (right)
    {
        right->free();
        delete right;
    }
}

BalancedTreeHead::BalancedTreeHead()
{
    head = new BalancedTree;
}

void BalancedTreeHead::input(ifstream &in)
{
    int num;
    in >> num;
    head->data = num;

    while (true)
    {
        in >> num;

        if (in.eof())
            break;

        head = insert(head, num);
    }
}

void BalancedTreeHead::print(string name)
{
    head->print(name);
}

unsigned char BalancedTreeHead::height(BalancedTree* root)
{
    return root ? root->height : 0;
}

int BalancedTreeHead::bfactor(BalancedTree* root)
{
    return height(root->right) - height(root->left);
}

void BalancedTreeHead::fixheight(BalancedTree* root)
{
    unsigned char hl = height(root->left);
    unsigned char hr = height(root->right);

    root->height = (hl > hr ? hl : hr) + 1;
}

BalancedTree* BalancedTreeHead::rotateright(BalancedTree* p)
{
    BalancedTree* q = p->left;
    p->left = q->right;
    q->right = p;

    fixheight(p);
    fixheight(q);

    return q;
}

BalancedTree* BalancedTreeHead::rotateleft(BalancedTree* q)
{
    BalancedTree* p = q->right;
    q->right = p->left;
    p->left = q;

    fixheight(q);
    fixheight(p);

    return p;
}

BalancedTree* BalancedTreeHead::balance(BalancedTree* root)
{
    fixheight(root);

    if (bfactor(root) == 2)
    {
        if (bfactor(root->right) < 0)
            root->right = rotateright(root->right);

        return rotateleft(root);
    }
    if (bfactor(root) == -2)
    {
        if (bfactor(root->left) > 0 )
            root->left = rotateleft(root->left);

        return rotateright(root);
    }
    return root;
}

BalancedTree* BalancedTreeHead::insert(BalancedTree* root, int data)
{
    if (!root)
        return new BalancedTree(data);

    if (data < root->data)
        root->left = insert(root->left, data);
    else
        root->right = insert(root->right, data);

    return balance(root);
}

BalancedTree* BalancedTreeHead::findmin(BalancedTree* root)
{
    return root->left ? findmin(root->left) : root;
}

BalancedTree* BalancedTreeHead::removemin(BalancedTree* root)
{
    if(root->left == 0)
        return root->right;

    root->left = removemin(root->left);

    return balance(root);
}

BalancedTree* BalancedTreeHead::remove(BalancedTree* root, int k)
{
    if(!root)
        return 0;

    if(k < root->data)
    {
        count_of_comp++;
        root->left = remove(root->left, k);
    }
    else if(k > root->data)
    {
        count_of_comp++;
        root->right = remove(root->right, k);
    }
    else
    {
        BalancedTree* q = root->left;
        BalancedTree* r = root->right;
        delete root;

        if(!r)
            return q;

        BalancedTree* min = findmin(r);
        min->right = removemin(r);
        min->left = q;

        return balance(min);
    }

    return balance(root);
}

void BalancedTreeHead::free()
{
    head->free();
}

ClosedHashElement::ClosedHashElement()
{
    data = 0;
    attempt = 0;
    free = true;
}

ClosedHashTable::ClosedHashTable()
{
}

int ClosedHashTable::function(int data)
{
    return data % digit;
}

void ClosedHashTable::input(ifstream &in)
{
    int num;
    in >> num;

    vector<int> values;
    values.push_back(num);

    while (true)
    {
        in >> num;

        if (in.eof())
            break;

        values.push_back(num);
    }

    for (int i = values.size() % 2 ? values.size() : values.size() + 1; ; i += 2)
    {
        bool is_simple = true;
        for (int j = 2; j < sqrt(i); j++)
        {
            if (i % j == 0)
            {
                is_simple = false;
                break;
            }
        }

        if (is_simple)
        {
            digit = i;
            break;
        }
    }

    for (int i = 0; i < digit; i++)
    {
        ClosedHashElement el;
        table.push_back(el);
    }

reorg_table:
    for (int i = 0; i < values.size(); i++)
        if (insert(values[i]) == 4)
            goto reorg_table;
}

void ClosedHashTable::print()
{
    cout << "┏━━━━━━━━┳━━━━━━━┳━━━━━━━┓" << endl;
    cout << "┃"
         << setw(8) << left << "Key"
         << "┃"
         << setw(7) << left << "Data"
         << "┃"
         << setw(7) << left << "Attempt"
         << "┃"
         << endl;

    cout << "┣━━━━━━━━╋━━━━━━━╋━━━━━━━┫" << endl;

    for (int i = 0; i < table.size(); i++)
    {
        if (!table[i].free)
            cout << "┃"
                 << setw(8) << left << i
                 << "┃"
                 << setw(7) << left << table[i].data
                 << "┃"
                 << setw(7) << left << table[i].attempt
                 << "┃"
                 << endl;
        else
            cout << "┃"
                 << setw(8) << left << i
                 << "┃"
                 << setw(7) << left << "NULL"
                 << "┃"
                 << setw(7) << left << "NULL"
                 << "┃"
                 << endl;
    }

    cout << "┗━━━━━━━━┻━━━━━━━┻━━━━━━━┛" << endl;
}

int ClosedHashTable::insert(int data)
{
    int key = function(data);

    if (table[key].free)
    {
        table[key].data = data;
        table[key].attempt = 1;
        table[key].free = false;

        return 1;
    }
    else
    {
        for (int att = 2; ; att++)
        {
            if (table[(key + att - 1) % table.size()].free)
            {
                table[(key + att - 1) % table.size()].data = data;
                table[(key + att - 1) % table.size()].attempt = att;
                table[(key + att - 1) % table.size()].free = false;

                return att;
            }

            if (att == 4)
            {
                reorganisation();
                return att;
            }
        }
    }
}

void ClosedHashTable::reorganisation()
{
    for (int i = digit + 2; ; i += 2)
    {
        bool is_simple = true;
        for (int j = 2; j < sqrt(i); j++)
        {
            if (i % j == 0)
            {
                is_simple = false;
                break;
            }
        }

        if (is_simple)
        {
            digit = i;
            break;
        }
    }

    int new_el = digit - table.size();

    for (int i = 0; i < table.size(); i++)
        table[i].free = true;

    for (int i = 0; i < new_el; i++)
    {
        ClosedHashElement el;
        table.push_back(el);
    }
}

void ClosedHashTable::remove(int data)
{
    int key = function(data);

    for (int i = 0; i < table.size(); i++)
    {
        if (table[(key + i) % table.size()].data == data)
        {
            count_of_comp++;
            table[(key + i) % table.size()].free = true;
            break;
        }
    }
}

File::File()
{
}

void File::input(string name)
{
    file.open(name, ios::in | ios::out);
    this->name = name;
}

void File::print()
{
    file.clear();
    file.seekg(0);

    while (true)
    {
        int num;
        file >> num;

        if (file.eof())
            break;

        cout << num << ' ';
    }

    cout << endl;
}

void File::remove(int data)
{
    file.clear();
    file.seekg(0);

    vector<int> val;

    while (true)
    {
        int num;
        file >> num;

        if (file.eof())
            break;

        val.push_back(num);
    }

    int find = -1;
    for (int i = 0; i < val.size(); i++)
    {
        count_of_comp++;
        if (val[i] == data)
        {
            find = i;
            break;
        }
    }

    if (find != -1)
    {
        file.close();

        string comand = "rm " + name;
        system(comand.c_str());

        file.open(name, ios::in | ios::out | ios::trunc);

        val.erase(val.begin() + find);

        for (int i = 0; i < val.size(); i++)
            file << val[i] << ' ';
    }
}

void File::close()
{
    file.close();
}

int input_file(ifstream &file, string &name)
{
    cout << "Выберите один из файлов:" << endl;
    system("ls *.in");
    cout << ">";
    cin >> name;

    file.open(name);

    if (file.is_open() == false)
    {
        cerr << "Некорректный файл!" << endl;
        return INCORRECT_FILE;
    }

    int num;
    vector<int> v;

    while (true)
    {
        file >> num;

        if (file.eof())
            break;

        for (int i = 0; i < v.size(); i++)
            if (num == v[i])
            {
                cerr << "В файле есть повторяющиеся элементы!" << endl;
                return INCORRECT_FILE;
            }

        v.push_back(num);
    }

    cout << endl;
    return SUCCESS;
}

int input_num()
{
    int num;

    cout << "Введите число, которое хотите удалить" << endl;
    cout << ">";

    while (scanf("%d", &num) != 1)
    {
        string buf;
        getline(cin, buf);

        cout << ">";
    }

    cout << endl;
    return num;
}

void create_tree(Tree **tree, ifstream &in_file)
{
    in_file.clear();
    in_file.seekg(0);

    cout << "Бинарное дерево:" << endl;
    *tree = new Tree;
    (*tree)->input(in_file);
    *tree = (*tree)->remove(*tree, 8);
    (*tree)->print("tree_first");
    cout << endl;
}

void create_balanced_tree(BalancedTreeHead &balance_tree, ifstream &in_file)
{
    in_file.clear();
    in_file.seekg(0);

    cout << "Сбалансированное дерево:" << endl;
    balance_tree.input(in_file);
    balance_tree.print("balance_first");
    cout << endl;
}

void create_hash_table(ClosedHashTable &hash_table, ifstream &in_file)
{
    in_file.clear();
    in_file.seekg(0);

    cout << "Хеш-таблица:" << endl;
    hash_table.input(in_file);
    hash_table.print();
    cout << endl;
}

void create_file(File &file, string name)
{
    file.input(name);

    cout << "Файл:" << endl;
    file.print();
    cout << endl;
}

void remove_tree(Tree *tree, int remove_num)
{
    count_of_comp = 0;
    clock_t start = clock();
    tree = tree->remove(tree, remove_num);
    clock_t finish = clock();

    cout << "Дерево:" << endl;
    cout << "Время удаления элемента: " << finish - start << endl;
    cout << "Количество сравнений: " << count_of_comp << endl;
    cout << endl;

    tree->print("tree_second");
}

void remove_balanced_tree(BalancedTreeHead &balance_tree, int remove_num)
{
    count_of_comp = 0;
    clock_t start = clock();
    balance_tree.head = balance_tree.remove(balance_tree.head, remove_num);
    clock_t finish = clock();

    cout << "Сбалансированное дерево:" << endl;
    cout << "Время удаления элемента: " << finish - start << endl;
    cout << "Количество сравнений: " << count_of_comp << endl;
    cout << endl;

    balance_tree.print("balance_second");
}

void remove_hash_table(ClosedHashTable &hash_table, int remove_num)
{
    count_of_comp = 0;
    clock_t start = clock();
    hash_table.remove(remove_num);
    clock_t finish = clock();

    cout << "Хеш-таблица:" << endl;
    cout << "Время удаления элемента: " << finish - start << endl;
    cout << "Количество сравнений: " << count_of_comp << endl;
    cout << endl;

    hash_table.print();
}

void remove_file(File &file, int remove_num)
{
    count_of_comp = 0;
    clock_t start = clock();
    file.remove(remove_num);
    clock_t finish = clock();

    cout << "Файл:" << endl;
    cout << "Время удаления элемента: " << finish - start << endl;
    cout << "Количество сравнений: " << count_of_comp << endl;
    cout << endl;

    file.print();
}

int main(void)
{
    Tree *tree = NULL;
    BalancedTreeHead balance_tree;
    ClosedHashTable hash_table;
    File file;

    ifstream in_file;
    string name;
    if (input_file(in_file, name) != SUCCESS)
        return INCORRECT_FILE;

    create_tree(&tree, in_file);
    create_balanced_tree(balance_tree, in_file);
    create_hash_table(hash_table, in_file);
    in_file.close();
    create_file(file, name);

    int remove_num = input_num();

    remove_tree(tree, remove_num);
    remove_balanced_tree(balance_tree, remove_num);
    remove_hash_table(hash_table, remove_num);
    remove_file(file, remove_num);

    tree->free();
    balance_tree.free();
    return SUCCESS;
}

