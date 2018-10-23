/*
 * Смоделировать операцию деления действительного числа в форме  ±m.n Е ±K,
 * где суммарная длина мантиссы (m+n) - до 30 значащих цифр, а величина
 * порядка K - до 5 цифр, на целое число длиной до 30 десятичных цифр. Результат
 * выдать в форме  ±0.m1 Е ±K1, где m1 - до 30 значащих цифр, а K1 - до 5 цифр.
 */

#include <iostream>
#include <cstring>
#include <vector>

#define SUCCESS 0
#define INPUT_ERROR -1
#define DIVISION_BY_ZERO -2
#define OVERFLOW -3

using namespace std;

void inter(string *a)
{
    for (int i = 0 ; i < (*a).size(); i++)
    {
        if ((*a)[i] == 'E')
            (*a)[i] = 'e';
        
        if ((*a)[i] == ',')
            (*a)[i] = '.';
        
        //if ((*a)[i] == 'e')
    }
    
    if ((*a)[0] == '.')
        (*a).insert(0, "0");
    
    if ((*a).size() > 1 &&
        ((*a)[0] == '+' || (*a)[0] == '-') && (*a)[1] == '.')
        (*a).insert(1, "0");
}

void del_null(vector<int>& a)
{
    int count = 0;
    
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] == 0)
            count++;
        else
            break;
    }
    
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < a.size() - 1; j++)
            swap(a[j], a[j + 1]);
        
        a.pop_back();
    }
}

bool check_value(string s)
{
    int count_e = 0, count_dots = 0;
    int index_e = -1;
    
    for (int i = 0; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
        {
            switch (s[i])
            {
                case '+':
                case '-':
                    // 12-12e12
                    if (i > 0 && s[i - 1] != 'e')
                        return false;
                    
                    // 12e12-
                    if (i == s.size() - 1)
                        return false;
                    
                    break;
                case '.':
                    if (++count_dots > 1)
                        return false;
                    
                    if (count_e == 1)
                        return false;
                    
                    break;
                    
                case 'e':
                    if (i == 0 || i == s.size() - 1)
                        return false;
                    
                    if (++count_e > 1)
                        return false;
                    
                    index_e = i;
                    
                    break;
                    
                default: return false;
            }
        }
    }
    
    return true;
}

bool check_count(string s)
{
    int index_e = -1;
    int count_dots = 0;
    int count_symb = 0;
    
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == 'e')
            index_e = i;
        
        if (s[i] == '.')
            count_dots++;
        
        if (s[i] == '+' || s[i] == '-')
            count_symb++;
    }
    
    if (count_dots > 1)
        return false;
    
    if (index_e != -1)
    {
        if (s[0] == '0' && s[1] == '.' && index_e > 31)
            return true;
        
        if (!isdigit(s[0]) && index_e > 32 && count_dots)
            return false;
        
        if (isdigit(s[0]) && index_e > 31 && count_dots)
            return false;
        
        if (!isdigit(s[0]) && index_e > 31 && !count_dots)
            return false;
        
        if (isdigit(s[0]) && index_e > 30 && !count_dots)
            return false;
        
        if (s.size() - index_e > 7 && !isdigit(s[index_e + 1]))
            return false;
        
        if (s.size() - index_e > 6 && isdigit(s[index_e + 1]))
            return false;
    }
    else
    {
        if (s[0] == '0' && s[1] == '.' && s.size() > 31)
            return true;
        
        if (s.size() - count_dots - count_symb > 30)
            return false;
    }
    
    return true;
}

bool check_float(string s)
{
    return check_value(s) && check_count(s);
}

bool check_int(string s)
{
    if (s.size() > 30 && isdigit(s[0]))
        return false;
    
    if (s.size() > 31 && !isdigit(s[0]))
        return false;
    
    for (int i = 0; i < s.size(); i++)
        if (!isdigit(s[i]))
        {
            if (!(i == 0 && (s[i] == '+' || s[i] == '-')))
                return false;
        }
    
    return true;
}

int get_pow(string a)
{
    int pow = 0;
    int index_e = a.size();
    
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] == 'e' || a[i] == 'E')
            index_e = i;
        
        if (i > index_e && isdigit(a[i]))
            pow = pow * 10 + (int)a[i] - 48;
    }
    
    if (a[index_e + 1] == '-')
        pow *= -1;
    
    return pow;
}

vector<int> float_to_intarr(string a, int* pow, int* answ)
{
    int size = -1;
    
    if (a[0] == '-')
        *answ = -1;
    else
        *answ = 1;
    
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] == '.' && a[i + 1] != 'e')
        {
            swap(a[i], a[i + 1]);
            (*pow)--;
        }
        
        if (a[i] == '.' || a[i] == 'e')
        {
            size = i;
            break;
        }
    }
    
    if (size == -1)
    {
        size = a.size();
    }
    
    vector<int> arr(size);
    
    if (!isdigit(a[0]))
    {
        arr.pop_back();
        
        for (int i = 1; i < size; i++)
        {
            arr[i - 1] = ((int)a[i]) - 48;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            arr[i] = ((int)a[i]) - 48;
        }
    }
    
    return arr;
}

vector<int> int_to_intarr(string a, int* answ)
{
    if (a[0] == '-')
        *answ = -1;
    else
        *answ = 1;
    
    vector<int> arr(a.size());
    
    if (!isdigit(a[0]))
    {
        arr.pop_back();
        
        for (int i = 1; i < a.size(); i++)
            arr[i - 1] = (int)a[i] - 48;
    }
    else
    {
        for (int i = 0; i < a.size(); i++)
            arr[i] = (int)a[i] - 48;
    }
    
    return arr;
}

bool decrement(vector<int>& num1, vector<int>& num2)
{
    if (num2.size() > num1.size())
        return false;
    
    int size = num1.size();
    vector<int> copy(size);
    
    for (int i = 0; i < size; i++)
        copy[i] = num1[i];
    
    for (int i = size - 1, j = num2.size() - 1; i >= 0; i--, j--)
    {
        if (j >= 0)
            copy[i] -= num2[j];
        
        if (copy[i] < 0 && i > 0)
        {
            copy[i] += 10;
            copy[i - 1]--;
        }
    }
    
    if (copy[0] < 0)
        return false;
    
    for (int i = 0; i < size; i++)
        num1[i] = copy[i];
    
    return true;
}

int print(vector<int> a, int pow, int answ)
{
    pow += a.size();
    
    if (pow > 99999 || pow < -99999)
    {
        cerr << "Overflow!" << endl;
        return OVERFLOW;
    }
    
    if (answ == 1)
        cout << '+';
    else
        cout << '-';
    
    cout << "0.";
    
    int start = 0;
    
    for (int i = 0; i < a.size(); i++)
        if (a[i] == 0)
        {
            start = i + 1;
            pow--;
        }
        else
            break;
    
    if (a.size() - start > 30)
    {
        for (int i = a.size() - 1; i >= start + 29; i--)
        {
            if (a[i] >= 5)
            {
                a[i - 1]++;
            }
        }
    }
    
    if (start + 28 <= a.size() && a[start + 28] > 9)
    {
        for (int i = start + 28; i >= 0; i--)
        {
            if (i - 1 >= 0 && a[i] > 9)
            {
                a[i] = 0;
                a[i - 1]++;
            }
            else
                break;
        }
    }
    
    for (int i = start; i < start + 29 && i < a.size(); i++)
        cout << a[i];
    
    if (start >= a.size())
        cout << "0";
    
    cout << "e";
    
    if (pow >= 0)
        cout << "+";
    
    cout << pow << endl;
    
    return SUCCESS;
}


int division(string first, string second)
{
    int answ1 = 0, answ2 = 0;
    int pow = get_pow(first);
    
    vector<int> first_arr = float_to_intarr(first, &pow, &answ1);
    vector<int> second_arr = int_to_intarr(second, &answ2);
    
    int size = first_arr.size();
    for (int i = 0; i < 31 - size; i++)
        first_arr.push_back(0);
    
    pow -= 31 - size;
    
    int div_pow = first_arr.size() - second_arr.size();
    pow += div_pow;
    
    for (int i = 0; i < div_pow; i++)
        second_arr.push_back(0);
    
    vector<int> res(1);
    
    del_null(second_arr);
    
    if (second_arr.empty())
    {
        cerr << "Division by zero!" << endl;
        return DIVISION_BY_ZERO;
    }
    
    while (true)
    {
        while (decrement(first_arr, second_arr))
        {
            if (++res[res.size() - 1] > 9)
                break;
        }
        
        second_arr.pop_back();
        pow--;
        
        del_null(first_arr);
        
        vector<int> copy = first_arr;
        
        if (first_arr.size() == 0)
            break;
        
        if (second_arr.size() == 0)
            break;
        
        if (res.size() >= 40)
            break;
        
        if (res[0] != 0)
            res.push_back(0);
    }
    
    pow++;
    
    return print(res, pow, answ1 * answ2);
}

istream& operator >> (istream &input, vector<int> &a)
{
    cout << "Input 10 numbers: ";
    for (int i = 0; i < a.size(); i++)
        input >> a[i];
    return input;
}

ostream& operator << (ostream &output, vector<int> a)
{
    cout << "10 numbers: ";
    for (int i = 0; i < a.size(); i++)
        output << a[i];
    output <<endl;
    return output;
}

int main(void)
{
    string first, second;
    
    cout << "--Template input--: ±______________________________e±_____" << endl;
    cout << "Input float number: ";
    cin >> first;
    cout << endl;
    
    if (!check_float(first))
    {
        cerr << "Input error!" << endl;
        return INPUT_ERROR;
    }
    
    cout << "---Template input---: ±______________________________" << endl;
    cout << "Input integer number: ";
    cin >> second;
    cout << endl;
    
    if (!check_int(second))
    {
        cerr << "Input error!" << endl;
        return INPUT_ERROR;
    }
    
    int code = division(first, second);
    return code;
}
