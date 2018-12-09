#include <iostream>
#include <time.h>
#include <vector>

#define SUCCESS 0

#define SIZE 10000

using namespace std;

vector<void*> fragmentation;
bool search_memory = false;

template <class T>
struct my_list
{
    T data;
    my_list<T> *next;
};

template <class T>
struct queue_array
{
    T data[SIZE];       // Массив с данными
    T *pin;             // Хвост
    T *pout;            // Голова
    int count;          // Количество элементов в очереди

    queue_array()
    {
        pin = NULL;
        pout = NULL;
    }

    void add(T data_el)
    {
        if (pin == NULL && pout == NULL)
        {
            pin = data;
            pout = data;
            *pin = data_el;

            count = 1;

            return;
        }

        if (count == SIZE)
        {
            cerr << "Очередь переполнилась!" << endl;
            return;
        }

        if (pin == data + SIZE - 1)
            pin = data;
        else
            pin++;

        *pin = data_el;

        count++;
    }

    void pop()
    {
        if (pout == NULL && pin == NULL)
        {
            return;
        }

        if (pout == pin)
        {
            pout = NULL;
            pin = NULL;

            count = 0;

            return;
        }

        if (pout == data + SIZE - 1)
            pout = data;
        else
            pout++;

        count--;
    }

    int size()
    {
        return count;
    }

    T take_head()
    {
        return *pout;
    }
};

template <class T>
struct queue_list
{
    my_list<T> *pin;    // Хвост
    my_list<T> *pout;   // Голова
    int count;          // Количество элементов в очереди

    queue_list()
    {
        pin = NULL;
        pout = NULL;
        count = 0;
    }

    void add(T data)
    {
        my_list<T> *temp = new my_list<T>;
        temp->data = data;

        if (pin == NULL && pout == NULL)
        {
            temp->next = NULL;
            pin = temp;
            pout = temp;
            return;
        }

        temp->next = NULL;
        pin->next = temp;
        pin = temp;

        if (search_memory)
        {
            for (int i = 0; i < fragmentation.size(); i++)
                if (pin == fragmentation[i])
                {
                    fragmentation.erase(fragmentation.begin() + i);
                    break;
                }
        }

        count++;
    }

    void pop()
    {
        if (pout == NULL)
            return;

        my_list<T> *temp = pout;
        pout = pout->next;
        T data = temp->data;

        if (search_memory)
            fragmentation.push_back(temp);

        delete temp;

        count--;
    }

    int size()
    {
        return count;
    }

    T take_head()
    {
        return pout->data;
    }

    void free_list()
    {
        my_list<T> temp = pin;

        while (temp != NULL)
        {
            my_list<T> el = temp;
            temp = temp->next;
            delete temp;
        }
    }
};

template <class T>
void test_queue(T queue)
{
    float time = 0;

    int count = 0;
    int count_type2 = 0;

    float t1_last = 0, t1 = rand() % 6; // приход типа 1
    float t2_last = 0, t2 = rand() % 5; // проход типа 4
    float t3_last = 0, t3 = rand() % 5; // проход типа 2
    float t4 = rand() % 3 + 1; // приход типа 2

    unsigned long long sum_size = 0;
    int in_count = 0, out_count = 0;
    unsigned long long  all_time_in_queue = 0;
    int time_in_OA = 0;

    bool was_check = true;
    bool type2 = false; // время типа 2

    for (; count < 1000; time += 0.001, sum_size += queue.size())
    {
        if (time - t1_last >= t1)
        {
            queue.add(time);
            t1_last = time;
            t1 = rand() % 6001 / 1000;

            in_count++;
        }

        if (!type2 && time - t2_last >= t2)
        {
            if (queue.size() > 0)
            {
                time_in_OA += t2;
                all_time_in_queue += time - queue.take_head();

                queue.pop();
                t2_last = time;
                t2 = rand() % 5001 / 1000;

                count++;
                out_count++;

                was_check = true;

                t4--;

                if (t4 == 0)
                {
                    type2 = true;
                    t3_last = time;
                    t3 = rand() % 5;
                }
            }
        }

        if (type2 && time - t3_last >= t3)
        {
            time_in_OA += t3;

            type2 = false;
            count_type2++;

            t2_last = time;
            t2 = rand() % 5001 / 1000;

            t4 = (rand() % 4001 + 1000) / 1000;
        }

        if (count % 100 == 0 && count > 0 && was_check)
        {
            cout << "Время: " << time << endl;
            cout << "Количество обработанных заявок: " << count << endl;
            cout << "Текущая длина очереди: " << queue.size() << endl;
            cout << "Средняя длина очереди: " << (float)sum_size / time << endl;
            cout << "Количество вошедших заявок: " << in_count << endl;
            cout << "Количество вышедших заявок: " << out_count << endl;
            cout << "Среднее время пребывания заявок в очереди: " << (float)all_time_in_queue / count;
            cout << endl << endl;

            was_check = false;
        }
    }

    cout << "Общее время моделирования: " << time << endl;
    cout << "Время простоя аппарата: " << time - time_in_OA << endl;
    cout << "Количество вошедших заявок первого типа: " << in_count << endl;
    cout << "Количество вышедших заявок первого типа: " << out_count << endl;
    cout << "Количество обращений заявок второго типа: " << count_type2 << endl;
}

ostream& operator << (ostream &output, const vector<void*> vec)
{
    for (int i = 0; i < vec.size(); i++)
        output << vec[i] << endl;

    return output;
}

void choice_search_memory()
{
    cout << "Выводить адреса памяти? (фрагментация)" << endl;
    cout << "[1] - Да" << endl;
    cout << "[2] - Нет" << endl;
    cout << ">";

    int choice;
    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2))
    {
        string buf;
        getline(cin, buf);

        cout << ">";
    }

    if (choice == 1)
        search_memory = true;

    cout << endl;
}

int main(void)
{
    srand(time(NULL));

    choice_search_memory();

    queue_array<int> array;
    queue_list<int> list;

    cout << "\e[1mРеализация массивом:\e[0m" << endl;
    test_queue< queue_array<int> >(array);
    cout << endl << endl;

    cout << "\e[1mРеализация списком:\e[0m" << endl;
    test_queue< queue_list<int> >(list);
    cout << endl << endl;

    if (search_memory)
    {
        cout << "\e[1mФрагментация памяти:\e[0m" << endl;
        cout << fragmentation << endl;
    }

    return SUCCESS;
}
