/*
 * Вариант 5
 * Разреженная (содержащая много нулей) матрица хранится в форме 3-х объектов:
 * - вектор A содержит значения ненулевых элементов;
 * - вектор JA содержит номера столбцов для элементов вектора A;
 * - связный список IA, в элементе Nk которого находится номер компонент
 * в A и JA, с которых начинается описание строки Nk матрицы A.
 *
 * 1. Смоделировать операцию умножения вектора-строки и матрицы, хранящихся в этой форме, с получением результата в той же форме.
 * 2. Произвести операцию умножения, применяя стандартный алгоритм работы с матрицами.
 * 3. Сравнить время выполнения операций и объем памяти при использовании этих 2-х алгоритмов при различном проценте заполнения матриц.
 */

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <fstream>

#define SUCCESS 0
#define INCORRECT_INPUT -1

using namespace std;

string buf;

typedef struct my_list my_list;
typedef struct Matrix Matrix;
typedef struct Vector Vector;

struct my_list
{
    int data;                       // элемент списка
    my_list *next;                  // следующий элемент
    my_list *prev;                  // предыдущий элемент
};

struct Matrix
{
    int n;                          // количество строк
    int m;                          // количество столбцов
    vector<int> values;             // значения ненулевых элементов
    vector<int> number_of_columns;  // номера столбцов
    my_list *first_elements;        // номера первых элементов строк

    /*~Matrix()
    {
        my_list *temp = first_elements;

        for (; temp != NULL; temp = temp->next)
            free(temp->prev);

        free(temp);
    }*/
};

struct Vector
{
    int n;                          // Количество элементов
    vector<int> values;             // Значения вектора
    vector<int> columns;            // Номера столбцов
};

void list_append(my_list **head, int data)
{
    if (head == NULL)
        return;

    if (*head == NULL)
    {
        *head = (my_list*)malloc(sizeof(my_list));
        (*head)->data = data;
        (*head)->next = NULL;
        return;
    }

    my_list *temp = *head;
    for (; temp->next != NULL; temp = temp->next);

    my_list *new_element = (my_list*)malloc(sizeof(my_list));

    new_element->data = data;
    new_element->next = NULL;
    new_element->prev = temp;
    temp->next = new_element;
}

bool is_int(string str)
{
    for (int i = 0; i < str.size(); i++)
        if (!isdigit(str[i]))
            return false;

    return true;
}

int cin_matrix(Matrix &matrix, const int count);

int generate_matrix(Matrix &matrix, const int count);

int input_matrix(Matrix &matrix)
{
    cout << "Количество строк: ";
    getline(cin, buf);

    if (!is_int(buf))
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    matrix.n = atoi(buf.c_str());

    if (matrix.n <= 0)
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    cout << "Количество столбцов: ";
    getline(cin, buf);

    if (!is_int(buf))
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    matrix.m = atoi(buf.c_str());

    cout << "Количество ненулевых элементов: ";
    getline(cin, buf);

    if (!is_int(buf))
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    int count = atoi(buf.c_str());

    if (count > matrix.n * matrix.m)
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    cout << "Как ввести элементы?" << endl
         << "[1] - Вручную" << endl
         << "[2] - Автозаполнение" << endl;

    getline(cin, buf);

    if (!is_int(buf))
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    switch(atoi(buf.c_str()))
    {
        case 1:
            return cin_matrix(matrix, count);
        case 2:
            return generate_matrix(matrix, count);
        default:
            cerr << "Некорректные данные!" << endl;
            return INCORRECT_INPUT;
    }
}

void insert_matrix_elements(Matrix &matrix, vector<int> lines)
{
    matrix.first_elements = NULL;

    for (int i = 0; i < matrix.n; i++)
    {
        bool check = false;

        for (int j = 0; j < lines.size(); j++)
        {
            if (lines[j] == i)
            {
                list_append(&(matrix.first_elements), j);
                check = true;
                break;
            }
        }

        if (!check)
        {
            my_list *temp = matrix.first_elements;
            for (; temp->next != NULL; temp = temp->next);
            list_append(&(matrix.first_elements), -1);
        }
     }

    for (my_list *temp = matrix.first_elements; temp != NULL; temp = temp->next)
    {
        if (temp->data == -1)
        {
            if (temp->next != NULL)
                temp->data = temp->next->data;
            else
                temp->data = matrix.values.size();
        }
    }
}

int cin_matrix(Matrix &matrix, const int count)
{
    vector<int> lines(0);

    for (int i = 0; i < count; i++)
    {
        getline(cin, buf);

        vector<string> split(0);
        string str;

        while (buf[0] == ' ')
        {
            buf.erase(0, 1);
        }

        for (int i = 0; i < buf.size(); i++)
        {
            if (buf[i] == ' ')
            {
                split.push_back(str);
                str = "";
                continue;
            }

            str += buf[i];

            if (i == buf.size() - 1)
                split.push_back(str);
        }

        if (split.size() != 3)
        {
            cerr << "Некорректные данные!" << endl;
            return INCORRECT_INPUT;
        }

        if (!is_int(split[0]) || !is_int(split[1]) || !is_int(split[2]))
        {
            cerr << "Некорректные данные!" << endl;
            return INCORRECT_INPUT;
        }

        if (atoi(split[0].c_str()) >= matrix.n || atoi(split[1].c_str()) >= matrix.m)
        {
            cerr << "Некорректные данные!" << endl;
            return INCORRECT_INPUT;
        }

        lines.push_back(atoi(split[0].c_str()));
        matrix.number_of_columns.push_back(atoi(split[1].c_str()));
        matrix.values.push_back(atoi(split[2].c_str()));
    }

    insert_matrix_elements(matrix, lines);

    return SUCCESS;
}

int generate_matrix(Matrix &matrix, const int count)
{
    vector<int> val(0);

    for (int i = 0; i < count; i++)
    {
        bool was = true;
        int num;

        while (was)
        {
            num = rand() % count;

            was = false;
            for (int j = 0; j < val.size(); j++)
                if (num == val[j])
                {
                    was = true;
                    break;
                }
        }

        val.push_back(num);
    }

    int count_in_line = count / matrix.n;
    int count_with_plus = 0;

    if (count > matrix.n)
            count_with_plus = count % matrix.n;

    matrix.first_elements = NULL;

    int number = 0;

    if (count < matrix.n)
    {
        count_in_line = 1;
        number = count;
    }
    else
        number = matrix.n;

    for (int i = 0; i < number; i++)
    {
        int n = count_in_line;
        if (i < count_with_plus)
            n += 1;

        vector<int> col_in_line(0);
        for (int j = 0; j < n; j++)
        {
            int num;
            bool was = true;

            while (was)
            {
                was = false;
                num = rand() % matrix.m;

                for (int k = 0; j < col_in_line.size(); j++)
                    if (num == col_in_line[k])
                    {
                        was = true;
                        break;
                    }
            }

            for (int i = 0; i < col_in_line.size(); i++)
                for (int j = 0; j < col_in_line.size() - i - 1; j++)
                        if (col_in_line[j] > col_in_line[j + 1])
                            swap(col_in_line[j], col_in_line[j + 1]);

            col_in_line.push_back(num);
            cout << "deb " << num << endl;
        }

        list_append(&(matrix.first_elements), matrix.number_of_columns.size());
        for (int j = 0; j < col_in_line.size(); j++)
            matrix.number_of_columns.push_back(col_in_line[j]);
    }

    for (int i = count; i < matrix.n; i++)
    {
        list_append(&(matrix.first_elements), count);
    }

    matrix.values = val;

    return SUCCESS;
}

int cin_vector(Vector &vector, int count);

int generate_vector(Vector &vector, int count);

int input_vector(Vector &matrix)
{
    cout << "Количество элементов вектора: ";
    getline(cin, buf);

    if (!is_int(buf))
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    matrix.n = atoi(buf.c_str());

    cout << "Количество ненулевых элементов: ";
    getline(cin, buf);

    if (!is_int(buf))
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    int count = atoi(buf.c_str());

    cout << "Как ввести элементы?" << endl
         << "[1] - Вручную" << endl
         << "[2] - Автозаполнение" << endl;

    getline(cin, buf);

    if (!is_int(buf))
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    switch(atoi(buf.c_str()))
    {
        case 1:
            return cin_vector(matrix, count);
        case 2:
            return generate_vector(matrix, count);
        default:
            cerr << "Некорректные данные!" << endl;
            return INCORRECT_INPUT;
    }


    return SUCCESS;
}

int cin_vector(Vector &matrix, int count)
{
    for (int i = 0; i < count; i++)
    {
        getline(cin, buf);

        while (buf[0] == ' ')
        {
            buf.erase(0, 1);
        }

        vector<string> split(0);
        string str;

        for (int i = 0; i < buf.size(); i++)
        {
            if (buf[i] == ' ')
            {
                split.push_back(str);
                str = "";
                continue;
            }

            str += buf[i];

            if (i == buf.size() - 1)
                split.push_back(str);
        }

        if (split.size() != 2)
        {
            cerr << "Некорректные данные!" << endl;
            return INCORRECT_INPUT;
        }

        if (!is_int(split[0]) || !is_int(split[1]))
        {
            cerr << "Некорректные данные!" << endl;
            return INCORRECT_INPUT;
        }

        if (atoi(split[0].c_str()) >= matrix.n)
        {
            cerr << "Некорректные данные!" << endl;
            return INCORRECT_INPUT;
        }

        matrix.columns.push_back(atoi(split[0].c_str()));
        matrix.values.push_back(atoi(split[1].c_str()));
    }

    return SUCCESS;
}

int generate_vector(Vector &matrix, int count)
{
    vector<int> val(0);

    for (int i = 0; i < count; i++)
    {
        bool was = true;
        int num;

        while (was)
        {
            num = rand() % count;

            was = false;
            for (int j = 0; j < val.size(); j++)
                if (num == val[j])
                {
                    was = true;
                    break;
                }
        }

        val.push_back(num);
    }

    matrix.values = val;

    vector<int> col(0);

    for (int i = 0; i < count; i++)
    {
        bool was = true;
        int num;

        while (was)
        {
            num = rand() % matrix.n;

            was = false;
            for (int j = 0; j < col.size(); j++)
                if (num == col[j])
                {
                    was = true;
                    break;
                }
        }

        col.push_back(num);
    }

    for (int i = 0; i < count; i++)
        for (int j = 0; j < count - i - 1; j++)
            if (col[j] > col[j + 1])
                swap(col[j], col[j + 1]);

    matrix.columns = col;

    return SUCCESS;
}

ostream& operator << (ostream &output, const Matrix matrix)
{
    output << "Матрица: " << endl;

    output << "Размерность: " << matrix.n << ' ' << matrix.m << endl;

    output << "Значения: ";
    for (int i = 0; i < matrix.values.size(); i++)
        output << matrix.values[i] << " ";
    output << endl;

    output << "Столбцы: ";
    for (int i = 0; i < matrix.values.size(); i++)
        output << matrix.number_of_columns[i] << " ";
    output << endl;

    output << "Номера первых элементов: ";
    for (my_list *temp = matrix.first_elements; temp != NULL; temp = temp->next)
        output << temp->data << " ";
    output << endl;

    return output;
}

ostream& operator << (ostream &output, const Vector vector)
{
    output << "Вектор: " << endl;

    output << "Размерность: " << vector.n << endl;

    output << "Значения: ";
    for (int i = 0; i < vector.values.size(); i++)
        output << vector.values[i] << " ";
    output << endl;

    output << "Позиции:  ";
    for (int i = 0; i < vector.columns.size(); i++)
        output << vector.columns[i] << " ";
    output << endl;

    return output;
}

ostream& operator << (ostream &output, const vector< vector<int> > matrix)
{
    output << "Матрица: " << endl;

    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
            output << matrix[i][j] << ' ';

        output << endl;
    }

    return output;
}

ostream& operator << (ostream &output, const vector<int> vec)
{
    output << "Вектор: ";

    for (int i = 0; i < vec.size(); i++)
        output << vec[i] << ' ';

    output << endl;

    return output;
}

Vector operator * (const Vector vector, const Matrix matrix)
{
    Vector result;

    result.n = matrix.m;

    for (int i = 0; i < matrix.m; i++) // Пробег по столбцам матрицы
    {
        my_list *temp = matrix.first_elements;

        int sum = 0; // i-й элемент результата

        for (int j = 0; j < matrix.n; j++) // Пробег по столбцу матрицы и строке вектора
        {
            int from = temp->data;                                                     // Начало строки
            int to = temp->next == NULL ? matrix.values.size() : temp->next->data;     // Конец строки

            for (int k = 0; k < vector.n; k++) // Поиск в векторе элемента
                if (vector.columns[k] == j) // Нашли элемент
                {
                    for (int l = from; l < to; l++) // Поиск в строке
                    {
                        if (i == matrix.number_of_columns[l]) // Нашли элемент столбца
                        {
                            sum += matrix.values[l] * vector.values[k]; // Нашли два элемента для результата
                            break;
                        }
                    }

                    break;
                }

            temp = temp->next; // Переход на следующую строку
        }

        if (sum != 0)
        {
            result.values.push_back(sum);
            result.columns.push_back(i);
        }
    }

    return result;
}

vector<int> operator * (const vector<int> vec, const vector< vector<int> > matrix)
{
    vector<int> result(0);

    for (int i = 0; i < matrix[0].size(); i++)
    {
        int sum = 0;

        for (int j = 0; j < vec.size(); j++)
            sum += vec[j] * matrix[j][i];

        result.push_back(sum);
    }

    return result;
}

vector< vector<int> > convert_to_default(const Matrix matrix)
{
    vector< vector<int> > result;

    for (int i = 0; i < matrix.n; i++)
    {
        vector<int> line;
        for (int j = 0; j < matrix.m; j++)
            line.push_back(0);

        result.push_back(line);
    }

    my_list *temp = matrix.first_elements;
    for (int i = 0; i < matrix.n; i++)
    {
        int from = temp->data;
        int to = temp->next == NULL ? matrix.values.size() : temp->next->data;

        for (int j = from; j < to; j++)
        {
            result[i][matrix.number_of_columns[j]] = matrix.values[j];
        }

        temp = temp->next;
    }

    return result;
}

vector<int> convert_to_default(const Vector vec)
{
    vector<int> result(0);

    for (int i = 0; i < vec.n; i++)
        result.push_back(0);

    for (int i = 0; i < vec.values.size(); i++)
        result[vec.columns[i]] = vec.values[i];

    return result;
}

int hand_made()
{
    Matrix matrix;
    Vector vec;

    vector< vector<int> > default_matrix;
    vector<int> default_vector;

    if (input_matrix(matrix) != SUCCESS)
        return INCORRECT_INPUT;

    cout << matrix << endl;

    if (input_vector(vec) != SUCCESS)
        return INCORRECT_INPUT;

    cout << vec << endl;

    default_matrix = convert_to_default(matrix);
    default_vector = convert_to_default(vec);

    cout << default_matrix << endl;
    cout << default_vector << endl;

    clock_t start = clock();
    Vector aswr = vec * matrix;
    clock_t end = clock();

    clock_t default_start = clock();
    cout << default_vector * default_matrix << endl;
    clock_t default_end = clock();

    cout << "Хранение разряженной: " << end - start << " тиков" << endl;
    cout << "Обычная матрица: " << default_end - default_start << " тиков" << endl;

    return SUCCESS;
}

int input_files(ifstream &file, Matrix &matrix, Vector &vec)
{
    if (!file.is_open())
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    int count = 0;

    file >> matrix.n;
    file >> matrix.m;
    file >> count;

    vector<int> lines(0);

    for (int i = 0; i < count; i++)
    {
        int a, b, c;
        file >> a >> b >> c;

        lines.push_back(a);
        matrix.number_of_columns.push_back(b);
        matrix.values.push_back(c);
    }

    insert_matrix_elements(matrix, lines);

    file >> vec.n;
    file >> count;

    for (int i = 0; i < count; i++)
    {
        int a, b;
        file >> a >> b;

        vec.columns.push_back(a);
        vec.values.push_back(b);
    }

    return SUCCESS;
}

int use_files()
{
    Matrix matrix;
    Vector vec;

    vector< vector<int> > default_matrix;
    vector<int> default_vector;

    cout << "Файлы: " << endl;
    system("ls *.in");

    getline(cin, buf);

    ifstream file;
    file.open(buf);

    if (input_files(file, matrix, vec) != SUCCESS)
        return INCORRECT_INPUT;

    file.close();

    cout << matrix << endl;
    cout << vec << endl;

    default_matrix = convert_to_default(matrix);
    default_vector = convert_to_default(vec);

    cout << default_matrix << endl;
    cout << default_vector << endl;

    clock_t start = clock();
    Vector answ = vec * matrix;
    clock_t end = clock();

    clock_t default_start = clock();
    cout << default_vector * default_matrix << endl;
    clock_t default_end = clock();

    cout << "Хранение разряженной: " << end - start << " тиков" << endl;
    cout << "Обычная матрица: " << default_end - default_start << " тиков" << endl;


    return SUCCESS;
}

int main(void)
{
    srand(time(NULL));

    cout << "[1] - Ввести данные" << endl
         << "[2] - Использовать заранее подготовленные данные" << endl;

    getline(cin, buf);

    if (!is_int(buf))
    {
        cerr << "Некорректные данные!" << endl;
        return INCORRECT_INPUT;
    }

    switch(atoi(buf.c_str()))
    {
        case 1:
            return hand_made();
        case 2:
            return use_files();
        default:
            cerr << "Некорректные данные!" << endl;
            return INCORRECT_INPUT;
    }

    return SUCCESS;
}
