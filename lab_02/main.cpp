/*
 * Ввести список машин, имеющихся в автомагазине, содержащий марку автомобиля,
 * страну-производитель, цену, цвет и состояние: новый – гарантия (в годах);
 * нет - год выпуска, пробег, количество ремонтов, количество собственников.
 * Вывести цены не новых машин указанной марки с одним предыдущим собственником,
 * отсутствием ремонта в указанном диапазоне цен.
*/

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <algorithm>

#define SUCCESS 0
#define INCORRECT_INPUT -1

using namespace std;

struct Car
{
    string model;
    string country;
    int price;
    string color;

    // 0 - new, 1 - old
    int mode;

    union
    {
        // Новый
        struct
        {
            int guarantee;
        } new_car;

        // Старый
        struct
        {
            int year;
            int mileage;
            int count_fix;
            int count_men;
        } old_car;
    } cond;
};

struct Key 
{
    int id;
    int price;
};

bool is_int(string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }

    return true;
}

/*
 * Ввод машины
 */

istream& operator >> (istream &input, struct Car &car)
{
    string buf;
    getline(input, buf);

    cout << "~~~~ Input car ~~~~" << endl;

    cout << "Model: ";
    getline(input, car.model);
    //input >> car.model;

    cout << "Country: ";
    getline(input, car.country);
    //input >> car.country;

    cout << "Price: ";
    input >> buf;

    if (!is_int(buf))
    {
        cerr << "Incorrect input!" << endl;
        car.mode = -1;
        return input;
    }

    car.price = atoi(buf.c_str());

    getline(input, buf);

    cout << "Color: ";
    getline(input, car.color);
    //input >> car.color;

    cout << "[0] - new, [1] - old: ";
    input >> buf;

    if (!is_int(buf))
    {
        cerr << "Incorrect input!" << endl;
        car.mode = -1;
        return input;
    }

    car.mode = atoi(buf.c_str());

    if (car.mode == 1)
    {
        cout << "Year: ";
        input >> buf;

        if (!is_int(buf))
        {
            cerr << "Incorrect input!" << endl;
            car.mode = -1;
            return input;
        }

        car.cond.old_car.year = atoi(buf.c_str());

        cout << "Mileage: ";
        input >> buf;

        if (!is_int(buf))
        {
            cerr << "incorrect input!" << endl;
            car.mode = -1;
            return input;
        }

        car.cond.old_car.mileage = atoi(buf.c_str());

        cout << "Count fix: ";
        input >> buf;

        if (!is_int(buf))
        {
            cerr << "incorrect input!" << endl;
            car.mode = -1;
            return input;
        }

        car.cond.old_car.count_fix = atoi(buf.c_str());

        cout << "Count men: ";
        input >> buf;

        if (!is_int(buf))
        {
            cerr << "incorrect input!" << endl;
            car.mode = -1;
            return input;
        }

        car.cond.old_car.count_men = atoi(buf.c_str());
    }
    else if (car.mode == 0)
    {
        cout << "Guarantee: ";

        input >> buf;

        if (!is_int(buf))
        {
            cerr << "incorrect input!" << endl;
            car.mode = -1;
            return input;
        }

        car.cond.new_car.guarantee = atoi(buf.c_str());
    }
    else
    {
        cerr << "incorrect input!" << endl;
        return input;
    }

    return input;
}

/*
 * Ввод машины из файла
 */

ifstream& operator >> (ifstream &input, struct Car &car)
{
    string buffer;
    getline(input, buffer);

    if (buffer.size() == 0)
    {
        car.mode = -1;
        return input;
    }

    vector<string> split(0);
    string str;

    for (int i = 0; i < buffer.size(); i++)
    {
        if (buffer[i] == ';')
        {
            split.push_back(str);
            str = "";
            continue;
        }

        str += buffer[i];
    }

    car.model = split[0];
    car.country = split[1];
    car.price = atoi(split[2].c_str());
    car.color = split[3];
    car.mode = atoi(split[4].c_str());

    if (car.mode)
    {
        car.cond.old_car.year = atoi(split[5].c_str());
        car.cond.old_car.mileage = atoi(split[6].c_str());
        car.cond.old_car.count_fix = atoi(split[7].c_str());
        car.cond.old_car.count_men = atoi(split[8].c_str());
    }
    else
    {
        car.cond.new_car.guarantee = atoi(split[5].c_str());
    }

    cout << "Input car success!" << endl;

    return input;
}

/*
 * Ввод таблицы
 */

istream& operator >> (istream &input, vector<struct Car> &data)
{
    for (int i = 0; i < data.size(); i++)
    {
        input >> data[i];
    }

    return input;
}

/*
 * Вывод машины в таблицу
 */

ostream& operator << (ostream &output, struct Car car)
{
    output << setw(30) << left << car.model
           << setw(15) << left << car.country
           << setw(15) << left << car.price
           << setw(10) << left << car.color;

    if (car.mode)
    {
        output << setw(6) << left << "Old "
               << setw(6) << left << "Year: "
               << setw(7) << left << car.cond.old_car.year
               << setw(9) << left << "Mileage: "
               << setw(9) << left << car.cond.old_car.mileage
               << setw(11) << left << "Count fix: "
               << setw(4) << left << car.cond.old_car.count_fix
               << setw(11) << left << "Count men: "
               << setw(4) << left << car.cond.old_car.count_men
               << endl;
    }
    else
    {
        output << setw(6) << left << "New"
               << setw(11) << left << "Guarantee: "
               << setw(4) << left << car.cond.new_car.guarantee
               << endl;
    }

    return output;
}

/*
 * Вывод табицы
 */

ostream& operator << (ostream &output, vector<struct Car> &data)
{
    if (data.size() == 0)
    {
        output << "Таблица пустая!" << endl;
        return output;
    }

    output << setw(5) << left << "Num"
           << setw(30) << left << "Model"
           << setw(15) << left << "Country"
           << setw(15) << left << "Price"
           << setw(10) << left << "Color"
           << setw(40) << left << "Condution" << endl;

    for (int i = 0; i < data.size(); i++)
    {
        output << setw(5) << left << i + 1 << data[i];
    }

    output << endl;

    return output;
}

/*
 * Вывод таблицы ключей
 */

ostream& operator << (ostream &output, vector<struct Key> &keys)
{
    if (keys.size() == 0)
    {
        cerr << "Пустая таблица!" << endl;
        return output;
    }

    output << "Num\tId\tPrice" << endl;

    for (int i = 0; i < keys.size(); i++)
    {
        output << i + 1 << "\t"
               << keys[i].id << "\t"
               << keys[i].price << endl;
    }

    return output;
}

/*
 * Добавление машины в таблицу
 */

vector<struct Car>& operator += (vector<struct Car> &data, struct Car car)
{
    data.push_back(car);
    return data;
}

/*
 * Записывает таблицу из файла
 */

vector<struct Car> data_file(vector<struct Key> &keys)
{
    vector<struct Car> data(0);

    ifstream file;
    file.open("data.txt", ios::out);

    while (true)
    {
        if (file.eof())
            break;

        struct Car car;
        car.mode = 0;

        file >> car;

        if (car.mode != -1)
            data += car;
    }

    keys.clear();

    for (int i = 0; i < data.size(); i++)
    {
        struct Key key;
        key.id = i + 1;
        key.price = data[i].price;
        keys.push_back(key);
    }

    file.close();
    return data;
}

/*
 * Запись в файл
 */

void write_to_file(vector<struct Car> data)
{
    ofstream file;
    file.open("data.txt", ios::in | ios::trunc);

    for (int i = 0; i < data.size(); i++)
    {
        file << data[i].model << ";";
        file << data[i].country << ";";
        file << data[i].price << ";";
        file << data[i].color << ";";
        file << data[i].mode << ";";

        if (data[i].mode)
        {
            file << data[i].cond.old_car.year << ";";
            file << data[i].cond.old_car.mileage << ";";
            file << data[i].cond.old_car.count_fix << ";";
            file << data[i].cond.old_car.count_men << ";";
        }
        else
        {
            file << data[i].cond.new_car.guarantee << ";";
        }

        file << endl;
    }

    file.close();
}

/*
 * Сортировка пузырьком таблицы
 */

void sort_data(vector<struct Car> &data)
{
    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data.size() - i - 1; j++)
            if (data[j].price > data[j + 1].price)
                swap(data[j], data[j + 1]);
}

bool comparator_car(struct Car a, struct Car b)
{
    return a.price < b.price;
}

void sorting_data(vector<struct Car> &data, vector<struct Key> &keys)
{
    clock_t start, finish;
    vector<struct Car> data_copy = data;

    start = clock();
    sort_data(data_copy);
    finish = clock();

    cout << "Сортировка пузырьком: " << finish - start << " тиков" << endl;

    start = clock();
    sort(data.begin(), data.end(), comparator_car);
    finish = clock();

    cout << "Быстрая сортировка: " << finish - start << " тиков" << endl;

    keys.clear();

    for (int i = 0; i < data.size(); i++)
    {
        struct Key key;
        key.id = i + 1;
        key.price = data[i].price;
        keys.push_back(key);
    }
}

/*
 * Сортировка пузырьком ключей
 */

void sort_keys(vector<struct Key> &keys)
{
    for (int i = 0; i < keys.size(); i++)
        for (int j = 0; j < keys.size() - i - 1; j++)
            if (keys[j].price > keys[j + 1].price)
                swap(keys[j], keys[j + 1]);
}

bool comparator_key(struct Key &a, struct Key &b)
{
    return a.price < b.price;
}

void sorting_keys(vector<struct Key> &keys)
{
    clock_t start, finish, time1, time2;
    vector<struct Key> keys_copy = keys;

    start = clock();
    sort_keys(keys_copy);
    finish = clock();

    time1 = finish - start;

    cout << "Сортировка пузырьком: " << time1 << " тиков" << endl;

    start = clock();
    sort(keys.begin(), keys.end(), comparator_key);
    finish = clock();

    time2 = finish - start;

    cout << "Быстрая сортировка: " << time2 << " тиков" << endl;
}


/*
 * Вывод таблицы с ключами
 */

void print_with_keys(vector<struct Car> data, vector<struct Key> keys)
{
    if (data.size() == 0)
    {
        cerr << "Пустая таблица!" << endl;
        return;
    }

    cout << setw(5) << left << "Num"
         << setw(30) << left << "Model"
         << setw(15) << left << "Country"
         << setw(15) << left << "Price"
         << setw(10) << left << "Color"
         << setw(40) << left << "Condution" << endl;


    for (int i = 0; i < keys.size(); i++)
    {
        cout << setw(5) << left << keys[i].id << data[keys[i].id - 1];
    }
}

int remove_car(vector<struct Car> &data, vector<struct Key> &keys)
{
    int id;
    string buf;

    cout << "Input id: ";
    cin >> buf;

    if (is_int(buf))
        id = atoi(buf.c_str());
    else
    {
        cerr << "Incorrect input!" << endl;
        return INCORRECT_INPUT;
    }

    if (id > data.size() || id <= 0)
    {
        cerr << "Incorrect id!" << endl;
        return INCORRECT_INPUT;
    }

    data.erase(data.begin() + id - 1);
 
    keys.clear();

    for (int i = 0; i < data.size(); i++)
    {
        struct Key key;
        key.id = i + 1;
        key.price = data[i].price;
        keys.push_back(key);
    }

    return SUCCESS;
}

int add_car(vector<struct Car> &data, vector<struct Key> &keys)
{
    struct Car car;
    car.mode = 0;
    car.price = 0;

    cin >> car;

    if (car.mode == -1)
    {
        return INCORRECT_INPUT;
    }

    data.push_back(car);
    return SUCCESS;
}

/*
 * Поиск
 */

int search(vector<struct Car> data)
{
    cout << "Input model: ";

    string buf;
    cin >> buf;

    cout << "Input min price: ";

    string a;
    cin >> a;

    if (!is_int(a))
    {
        cerr << "Incorrect input!" << endl;
        return INCORRECT_INPUT;
    }

    cout << "Input max price: ";

    string b;
    cin >> b;

    if (!is_int(b))
    {
        cerr << "Incorrect input!" << endl;
        return INCORRECT_INPUT;
    }

    int min = atoi(a.c_str());
    int max = atoi(b.c_str());

    int count = 0;

    for (int i = 0; i < data.size(); i++)
    {
        if (data[i].mode)
        {
            int k = data[i].model.find(buf);

            if (k >= 0 && data[i].cond.old_car.count_fix == 0 && data[i].cond.old_car.count_men == 1 && data[i].price >= min && data[i].price <= max)
            {
                cout << data[i];
                count++;
            }
        }
    }

    if (count == 0)
    {
        cout << "Нет машин, удовлетворяющих этим запросам." << endl;
    }

    return SUCCESS;
}

void print_menu()
{
    system("read -p \"Нажмите enter, чтобы продолжить\"");

    system("clear");

    cout << "~~~~~ MENU ~~~~~"                                                << endl;
    cout << "[1] - Загрузить файл"                                            << endl;
    cout << "[2] - Сохранить в файл"                                          << endl;
    cout << "[3] - Добавить новую запись"                                     << endl;
    cout << "[4] - Удалить запись"                                            << endl;
    cout << "[5] - Отсортировать записи таблицы"                              << endl;
    cout << "[6] - Вывести таблицу"                                           << endl;
    cout << "[7] - Отсортировать таблицу ключей"                              << endl;
    cout << "[8] - Печать ключей"                                             << endl;
    cout << "[9] - Печать таблицы в последоветельности ключей"                << endl;
    cout << "[10] - Поиск старых автомобилей указанной марки в диапазоне цен" << endl;
    cout << ""                                                                << endl;
    cout << "Для выхода введите любой другой символ"                          << endl;
}

void menu(vector<struct Car> &data, vector<struct Key> &keys)
{
    while (true)
    {
        print_menu();

        int choise;
        cin >> choise;

        struct Car car;

        switch (choise)
        {
            case 1:
                data = data_file(keys);
                break;

            case 2:
                write_to_file(data);
                break;

            case 3:
                add_car(data, keys);
                break;

            case 4:
                remove_car(data, keys);
                break;

            case 5:
                sorting_data(data, keys);
                break;

            case 6:
                cout << data;
                break;

            case 7:
                sorting_keys(keys);
                break;

            case 8:
                cout << keys;
                break;

            case 9:
                print_with_keys(data, keys);
                break;

            case 10:
                search(data);
                break;

            default:
                return;
        }
    }
}

int main(void)
{
    vector<struct Key> keys(0);
    vector<struct Car> data(0);

    menu(data, keys);

    return SUCCESS;
}
