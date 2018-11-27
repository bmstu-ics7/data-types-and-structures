#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv)
{
    srand(time(NULL));

    if (argc != 4)
        return -1;

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int percent = atoi(argv[3]);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m ;j++)
            if (rand() % 100 < percent)
                cout << "0 ";
            else
                cout << "1 ";

        cout << endl;
    }

}
